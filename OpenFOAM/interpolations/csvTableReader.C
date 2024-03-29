﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
    Copyright (C) 2020-2021 OpenCFD Ltd.
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

#include "csvTableReader.H"
#include "fileOperation.H"
#include "DynamicList.H"
#include "ListOps.H"

// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //


 namespace Foam{
template<class Type>
labelList csvTableReader<Type>::getComponentColumns
(
    const word& name,
    std::initializer_list<std::pair<const char*,int>> compat,
    const dictionary& dict
)
{
    // Writing of columns was forced to be ASCII,
    // do the same when reading

    labelList cols;

    ITstream& is = dict.lookupCompat(name, compat);
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


 } // End namespace Foam
namespace Foam
{
    template<>
    label csvTableReader<label>::readValue
    (
        const List<string>& strings
    ) const
    {
        return readLabel(strings[componentColumns_[0]]);
    }


    template<>
    scalar csvTableReader<scalar>::readValue
    (
        const List<string>& strings
    ) const
    {
        return readScalar(strings[componentColumns_[0]]);
    }

} // End namespace Foam



 namespace Foam{
template<class Type>
Type csvTableReader<Type>::readValue
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


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
csvTableReader<Type>::csvTableReader(const dictionary& dict)
:
    tableReader<Type>(dict),
    headerLine_(dict.get<bool>("hasHeaderLine")),
    refColumn_(dict.getCompat<label>("refColumn", {{"timeColumn", 1912}})),
    componentColumns_
    (
        getComponentColumns("componentColumns", {{"valueColumns", 1912}}, dict)
    ),
    separator_(dict.getOrDefault<string>("separator", ",")[0])
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void csvTableReader<Type>::operator()
(
    const fileName& fName,
    List<Tuple2<scalar, Type>>& data
)
{
    autoPtr<ISstream> isPtr(fileHandler().NewIFstream(fName));
    ISstream& is = isPtr();

    const label maxEntry =
        max(refColumn_, componentColumns_[findMax(componentColumns_)]);

    string line;
    label lineNo = 0;

    // Skip header
    if (headerLine_)
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
            const auto nPos = line.find(separator_, pos);

            if (nPos == std::string::npos)
            {
                strings.append(line.substr(pos));
                pos = nPos;
            }
            else
            {
                strings.append(line.substr(pos, nPos-pos));
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

    data.transfer(values);
}


template<class Type>
void csvTableReader<Type>::operator()
(
    const fileName& fName,
    List<Tuple2<scalar, List<Tuple2<scalar, Type>>>>& data
)
{
    NotImplemented;
}


template<class Type>
void csvTableReader<Type>::write(Ostream& os) const
{
    tableReader<Type>::write(os);

    os.writeEntry("hasHeaderLine", headerLine_);
    os.writeEntry("refColumn", refColumn_);

    // Force writing labelList in ASCII
    const auto oldFmt = os.format(IOstream::ASCII);
    os.writeEntry("componentColumns", componentColumns_);
    os.format(oldFmt);

    os.writeEntry("separator", string(separator_));
}


// ************************************************************************* //

 } // End namespace Foam
