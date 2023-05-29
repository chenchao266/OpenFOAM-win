/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2015 OpenFOAM Foundation
    Copyright (C) 2015-2021 OpenCFD Ltd.
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

#include "stateFunctionObject.H"
#include "Time1.h"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


 namespace Foam{
const word functionObjects::stateFunctionObject::resultsName_ =
    "results";


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

const functionObjects::properties&
functionObjects::stateFunctionObject::stateDict() const
{
    return time_.functionObjects().propsDict();
}


functionObjects::properties&
functionObjects::stateFunctionObject::stateDict()
{
    return const_cast<functionObjects::properties&>
    (
        time_.functionObjects().propsDict()
    );
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

functionObjects::stateFunctionObject::stateFunctionObject
(
    const word& name,
    const Time& runTime
)
:
    timeFunctionObject(name, runTime)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

dictionary& functionObjects::stateFunctionObject::propertyDict()
{
    return stateDict().getObjectDict(name());
}


void functionObjects::stateFunctionObject::clearTrigger()
{
    return stateDict().clearTrigger();
}


label functionObjects::stateFunctionObject::getTrigger() const
{
    return stateDict().getTrigger();
}


bool functionObjects::stateFunctionObject::setTrigger
(
    const label triggeri,
    bool increaseOnly
)
{
    return stateDict().setTrigger(triggeri, increaseOnly);
}


bool functionObjects::stateFunctionObject::foundProperty
(
    const word& entryName
) const
{
    return stateDict().foundObjectProperty(name(), entryName);
}


bool functionObjects::stateFunctionObject::getDict
(
    const word& entryName,
    dictionary& dict
) const
{
    return stateDict().getObjectDict(name(), entryName, dict);
}


bool functionObjects::stateFunctionObject::getObjectDict
(
    const word& objectName,
    const word& entryName,
    dictionary& dict
) const
{
    return stateDict().getObjectDict(objectName, entryName, dict);
}


word functionObjects::stateFunctionObject::resultType
(
    const word& entryName
) const
{
    return stateDict().objectResultType(name(), entryName);
}


word functionObjects::stateFunctionObject::objectResultType
(
    const word& objectName,
    const word& entryName
) const
{
    return stateDict().objectResultType(objectName, entryName);
}


wordList
functionObjects::stateFunctionObject::objectResultEntries() const
{
    return stateDict().objectResultEntries(name());
}


wordList
functionObjects::stateFunctionObject::objectResultEntries
(
    const word& objectName
) const
{
    return stateDict().objectResultEntries(objectName);
}


void functionObjects::stateFunctionObject::writeResultEntries
(
    Ostream& os
) const
{
    return stateDict().writeResultEntries(name(), os);
}


void functionObjects::stateFunctionObject::writeResultEntries
(
    const word& objectName,
    Ostream& os
) const
{
    return stateDict().writeResultEntries(objectName, os);
}


void functionObjects::stateFunctionObject::writeAllResultEntries
(
    Ostream& os
) const
{
    return stateDict().writeAllResultEntries(os);
}


// ************************************************************************* //

 } // End namespace Foam
