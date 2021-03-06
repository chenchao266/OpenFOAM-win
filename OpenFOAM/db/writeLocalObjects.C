﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2016 OpenFOAM Foundation
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

#include "writeLocalObjects.H"
#include "stringListOps.H"
#include "dictionary.H"

// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //
using namespace Foam;
void functionObjects::writeLocalObjects::resetLocalObjectName
(
    const word& name
)
{
    localObjectNames_.clear();
    localObjectNames_.append(name);
}


void functionObjects::writeLocalObjects::resetLocalObjectNames
(
    const wordList& names
)
{
    localObjectNames_.clear();
    localObjectNames_.append(names);
}


wordList functionObjects::writeLocalObjects::objectNames()
{
    wordList names
    (
        subsetStrings(wordReListMatcher(writeObjectNames_), localObjectNames_)
    );

    return names;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

functionObjects::writeLocalObjects::writeLocalObjects
(
    const objectRegistry& obr,
    const Switch& log
) :    writeObjectsBase(obr, log),
    localObjectNames_()
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

functionObjects::writeLocalObjects::~writeLocalObjects()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const wordList&
functionObjects::writeLocalObjects::localObjectNames() const
{
    return localObjectNames_;
}


bool functionObjects::writeLocalObjects::read(const dictionary& dict)
{
    if (dict.found("objects"))
    {
        writeObjectsBase::read(dict);
    }
    else
    {
        resetWriteObjectName(wordRe(".*", wordRe::DETECT));
    }

    return true;
}


// ************************************************************************* //
