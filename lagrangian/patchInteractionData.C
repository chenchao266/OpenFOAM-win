﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011 OpenFOAM Foundation
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

#include "patchInteractionData.H"
#include "dictionaryEntry.H"
#include "PatchInteractionModel.H"
#include "_Ostream.H"

// * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * //

Foam::patchInteractionData::patchInteractionData()
:
    interactionTypeName_(),
    patchName_(),
    e_(0),
    mu_(0)
{}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

Foam::Istream& Foam::operator>>
(
    Istream& is,
    patchInteractionData& pid
)
{
    is.check(FUNCTION_NAME);

    const dictionaryEntry dictEntry(dictionary::null, is);
    const dictionary& dict = dictEntry.dict();

    pid.patchName_ = dictEntry.keyword();

    dict.readEntry("type", pid.interactionTypeName_);
    pid.e_ = dict.getOrDefault<scalar>("e", 1);
    pid.mu_ = dict.getOrDefault<scalar>("mu", 0);

    return is;
}

Foam::Ostream& Foam::operator<<(Ostream& os, const patchInteractionData& pid)
{
    return os;
}

bool Foam::operator!=(const patchInteractionData& pidl, const patchInteractionData& pidr)
{
    return !(pidl.interactionTypeName() == pidr.interactionTypeName() &&
        pidl.patchName() == pidr.patchName()&&
        pidl.e() == pidr.e()&&
        pidl.mu() == pidr.mu()
        );
}
bool Foam::operator==(const patchInteractionData& pidl, const patchInteractionData& pidr)
{
    return (pidl.interactionTypeName() == pidr.interactionTypeName() &&
        pidl.patchName() == pidr.patchName() &&
        pidl.e() == pidr.e() &&
        pidl.mu() == pidr.mu()
        );
}
// ************************************************************************* //
