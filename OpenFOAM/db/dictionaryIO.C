﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
    Copyright (C) 2016-2020 OpenCFD Ltd.
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

#include "dictionary2.H"
#include "IFstream.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


 namespace Foam{
dictionary::dictionary
(
    const fileName& name,
    const dictionary& parentDict,
    Istream& is,
    bool keepHeader
)
:
    name_(fileName::concat(parentDict.name(), name, '.')),
    parent_(parentDict)
{
    read(is, keepHeader);
}


dictionary::dictionary(Istream& is)
:
    dictionary(is, false)
{}


dictionary::dictionary(Istream& is, bool keepHeader)
:
    name_(is.name()),
    parent_(dictionary::null)
{
    // Reset input mode as this is a "top-level" dictionary
    entry::resetInputMode();

    read(is, keepHeader);
}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

autoPtr<dictionary> dictionary::New(Istream& is)
{
    return autoPtr<dictionary>::New(is);
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

bool dictionary::read(Istream& is, bool keepHeader)
{
    // Normally remove FoamFile header when read, but avoid this if it already
    // existed prior to the current read.
    // We would otherwise lose it with every top-level '#include ...'

    keepHeader = keepHeader || hashedEntries_.found("FoamFile");

    // Check for empty dictionary
    if (is.eof())
    {
        return true;
    }

    if (!is.good())
    {
        FatalIOErrorInFunction(is)
            << "Istream not OK for reading dictionary " << name()
            << exit(FatalIOError);

        return false;
    }

    // The expected end character
    int endChar = token::END_BLOCK;
    token currToken(is);

    if (currToken == token::END_BLOCK)
    {
        FatalIOErrorInFunction(is)
            << "Dictionary input cannot start with '}'" << nl
            << exit(FatalIOError);
    }
    else if (currToken != token::BEGIN_BLOCK)
    {
        is.putBack(currToken);
        endChar = 0;
    }

    while
    (
        !is.eof()
     && entry::New(*this, is, entry::GLOBAL, endChar)
    )
    {}

    if (!keepHeader)
    {
        remove("FoamFile");
    }

    if (is.bad())
    {
        InfoInFunction
            << "Istream not OK after reading dictionary " << name()
            << endl;

        return false;
    }

    return true;
}


bool dictionary::read(Istream& is)
{
    return this->read(is, false);
}


// * * * * * * * * * * * * * * Istream Operator  * * * * * * * * * * * * * * //

Istream& operator>>(Istream& is, dictionary& dict)
{
    // Reset input mode assuming this is a "top-level" dictionary
    entry::resetInputMode();

    dict.clear();
    dict.name() = is.name();
    dict.read(is);

    return is;
}


// * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * * //

void dictionary::writeEntry(Ostream& os) const
{
    os.beginBlock(dictName());
    writeEntries(os);
    os.endBlock();
}


void dictionary::writeEntry(const keyType& kw, Ostream& os) const
{
    os.beginBlock(kw);
    writeEntries(os);
    os.endBlock();
}


void dictionary::writeEntries(Ostream& os, const bool extraNewLine) const
{
    for (const entry& e : *this)
    {
        // Write entry
        os  << e;

        // Add extra new line between entries for "top-level" dictionaries,
        // but not after the last entry (looks ugly).
        if (extraNewLine && parent() == dictionary::null && e != *last())
        {
            os  << nl;
        }

        // Check stream before going to next entry.
        if (!os.good())
        {
            WarningInFunction
                << "Cannot write entry " << e.keyword()
                << " for dictionary " << name()
                << endl;
        }
    }
}


void dictionary::write(Ostream& os, const bool subDict) const
{
    if (subDict)
    {
        os  << nl;
        os.beginBlock();
    }

    writeEntries(os, !subDict);

    if (subDict)
    {
        os.endBlock();
    }
}


Ostream& operator<<(Ostream& os, const dictionary& dict)
{
    dict.write(os, true);
    return os;
}


// ************************************************************************* //

 } // End namespace Foam
