﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2017 OpenFOAM Foundation
     \\/     M anipulation  |
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

#include "baseIOdictionary.H"
#include "objectRegistry.H"
#include "Pstream.T.H"
#include "Time.T.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //
using namespace Foam;
namespace Foam
{
defineTypeNameAndDebug(baseIOdictionary, 0);

bool baseIOdictionary::writeDictionaries
(
    debug::infoSwitch("writeDictionaries", 0)
);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

baseIOdictionary::baseIOdictionary(const IOobject& io) :    regIOobject(io)
{
    dictionary::name() = IOobject::objectPath();
}


baseIOdictionary::baseIOdictionary
(
    const IOobject& io,
    const dictionary& dict
) :    regIOobject(io)
{
    dictionary::name() = IOobject::objectPath();
}


baseIOdictionary::baseIOdictionary
(
    const IOobject& io,
    Istream& is
) :    regIOobject(io)
{
    dictionary::name() = IOobject::objectPath();
}


// * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * * //

baseIOdictionary::~baseIOdictionary()
{}


// * * * * * * * * * * * * * * * Members Functions * * * * * * * * * * * * * //

const word& baseIOdictionary::name() const
{
    return regIOobject::name();
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

void baseIOdictionary::operator=(const baseIOdictionary& rhs)
{
    dictionary::operator=(rhs);
}


// ************************************************************************* //
