﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
    Copyright (C) 2016-2021 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "CSV.H"
#include "DynamicList.H"
#include "ListOps.H"

// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //


 namespace Foam{
template<class Type>
labelList Function1Types::CSV<Type>::getComponentColumns
(
    const word& name,
    const dictionary& dict
)
{
    // Writing of columns was forced to be ASCII,
    // do the same when reading

    labelList cols;

    ITstream& is = dict.lookup(name);
    is.format(IOstream::ASCII);
    is >> cols;
    dict.checkITstream(is, name);

    if (cols.size() != pTraits<Type>::nComponents)
    {
        FatalIOErrorInFunction(dict)
            << name << " with " << cols
            << " does not have the expected length "
            << pTraits<Type>::nComponents << nl
            << exit(FatalIOError);
    }

    return cols;
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<>
label Function1Types::CSV<label>::readValue
(
    const List<string>& strings
) const
{
    return readLabel(strings[componentColumns_[0]]);
}


template<>
scalar Function1Types::CSV<scalar>::readValue
(
    const List<string>& strings
) const
{
    return readScalar(strings[componentColumns_[0]]);
}


template<class Type>
Type Function1Types::CSV<Type>::readValue
(
    const List<string>& strings
) const
{
    Type result;

    for (label i = 0; i < pTraits<Type>::nComponents; ++i)
    {
        result[i] = readScalar(strings[componentColumns_[i]]);
    }

    return result;
}


template<class Type>
void Function1Types::CSV<Type>::read()
{
    fileName expandedFile(fName_);
    autoPtr<ISstream> isPtr(fileHandler().NewIFstream(expandedFile.expand()));
    ISstream& is = isPtr();

    if (!is.good())
    {
        FatalIOErrorInFunction(is)
            << "Cannot open CSV file for reading."
            << exit(FatalIOError);
    }

    const label maxEntry =
        max(refColumn_, componentColumns_[findMax(componentColumns_)]);

    string line;
    label lineNo = 0;

    // Skip header
    for (label i = 0; i < nHeaderLine_; ++i)
    {
        is.getLine(line);
        ++lineNo;
    }

    DynamicList<Tuple2<scalar, Type>> values;
    DynamicList<string> strings(maxEntry+1);  // reserve

    while (is.good())
    {
        is.getLine(line);
        ++lineNo;

        strings.clear();

        std::size_t pos = 0;

        for
        (
            label n = 0;
            (pos != std::string::npos) && (n <= maxEntry);
            ++n
        )
        {
            if (mergeSeparators_)
            {
                bool found = false;
                while (!found)
                {
                    const auto nPos = line.find(separator_, pos);

                    if ((nPos != std::string::npos) && (nPos - pos == 0))
                    {
                        pos = nPos + 1;
                    }
                    else
                    {
                        found = true;
                    }
                }
            }

            const auto nPos = line.find(separator_, pos);

            if (nPos == std::string::npos)
            {
                strings.append(line.substr(pos));
                pos = nPos;
            }
            else
            {
                strings.append(line.substr(pos, nPos - pos));
                pos = nPos + 1;
            }
        }

        if (strings.size() <= 1)
        {
            break;
        }

        if (strings.size() <= maxEntry)
        {
            FatalErrorInFunction
                << "Not enough columns near line " << lineNo
                << ". Require " << (maxEntry+1) << " but found "
                << strings << nl
                << exit(FatalError);
        }

        scalar x = readScalar(strings[refColumn_]);
        Type value = readValue(strings);

        values.append(Tuple2<scalar,Type>(x, value));
    }

    this->table_.transfer(values);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
Function1Types::CSV<Type>::CSV
(
    const word& entryName,
    const dictionary& dict,
    const objectRegistry* obrPtr,
    const fileName& fName
)
:
    TableBase<Type>(entryName, dict, obrPtr),
    nHeaderLine_(dict.get<label>("nHeaderLine")),
    refColumn_(dict.get<label>("refColumn")),
    componentColumns_(getComponentColumns("componentColumns", dict)),
    separator_(dict.getOrDefault<string>("separator", ",")[0]),
    mergeSeparators_(dict.get<bool>("mergeSeparators")),
    fName_(fName.empty() ? dict.get<fileName>("file") : fName)
{
    read();

    TableBase<Type>::initialise();
}


template<class Type>
Function1Types::CSV<Type>::CSV(const CSV<Type>& csv)
:
    TableBase<Type>(csv),
    nHeaderLine_(csv.nHeaderLine_),
    refColumn_(csv.refColumn_),
    componentColumns_(csv.componentColumns_),
    separator_(csv.separator_),
    mergeSeparators_(csv.mergeSeparators_),
    fName_(csv.fName_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
const fileName& Function1Types::CSV<Type>::fName() const
{
    return fName_;
}


template<class Type>
void Function1Types::CSV<Type>::writeEntries(Ostream& os) const
{
    // Note: for TableBase write the dictionary entries it needs but not
    // the values themselves
    TableBase<Type>::writeEntries(os);

    os.writeEntry("nHeaderLine", nHeaderLine_);
    os.writeEntry("refColumn", refColumn_);

    // Force writing labelList in ASCII
    const auto oldFmt = os.format(IOstream::ASCII);
    os.writeEntry("componentColumns", componentColumns_);
    os.format(oldFmt);

    os.writeEntry("separator", string(separator_));
    os.writeEntry("mergeSeparators", mergeSeparators_);
    os.writeEntry("file", fName_);
}


template<class Type>
void Function1Types::CSV<Type>::writeData(Ostream& os) const
{
    Function1<Type>::writeData(os);
    os.endEntry();

    os.beginBlock(word(this->name() + "Coeffs"));
    writeEntries(os);
    os.endBlock();
}


// ************************************************************************* //

 } // End namespace Foam
