/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2016-2019 OpenCFD Ltd.
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

#include "word.H"
#include "_Ostream.H"
#include "token.H"
#include "keyType.H"
#include "IOstreams.H"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


 namespace Foam{
void Ostream::decrIndent()
{
    if (!indentLevel_)
    {
        std::cerr
            << "Ostream::decrIndent() : attempt to decrement 0 indent level\n";
    }
    else
    {
        --indentLevel_;
    }
}


Ostream& Ostream::write(const keyType& kw)
{
    return writeQuoted(kw, kw.isPattern());
}


Ostream& Ostream::writeKeyword(const keyType& kw)
{
    indent();
    writeQuoted(kw, kw.isPattern());

    if (indentSize_ <= 1)
    {
        write(char(token::SPACE));
        return *this;
    }

    label nSpaces = entryIndentation_ - label(kw.size());

    // Account for quotes surrounding pattern
    if (kw.isPattern())
    {
        nSpaces -= 2;
    }

    // Could also increment by indentSize_ ...
    if (nSpaces < 1)
    {
        nSpaces = 1;
    }

    while (nSpaces--)
    {
        write(char(token::SPACE));
    }

    return *this;
}


Ostream& Ostream::beginBlock(const keyType& kw)
{
    indent(); writeQuoted(kw, kw.isPattern()); write('\n');
    beginBlock();

    return *this;
}


Ostream& Ostream::beginBlock()
{
    indent(); write(char(token::BEGIN_BLOCK)); write('\n');
    incrIndent();

    return *this;
}


Ostream& Ostream::endBlock()
{
    decrIndent();
    indent(); write(char(token::END_BLOCK)); write('\n');

    return *this;
}


Ostream& Ostream::endEntry()
{
    write(char(token::END_STATEMENT)); write('\n');

    return *this;
}


// ************************************************************************* //

 } // End namespace Foam
