﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2021 OpenCFD Ltd.
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

#include "functionObjectProperties.H"
#include "SHA1.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


 namespace Foam{
const word functionObjects::properties::resultsName_ =
    SHA1("results").str();


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

functionObjects::properties::properties
(
    const IOobject& io
)
:
    IOdictionary(io)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

wordList functionObjects::properties::objectNames() const
{
    // TODO
    // - remove resultsName_ dict from results?
    // - or put objects into their own subdictionary?
    return sortedToc();
}


bool functionObjects::properties::hasObjectDict
(
    const word& objectName
) const
{
    return found(objectName);
}


dictionary& functionObjects::properties::getObjectDict
(
    const word& objectName
)
{
    if (!found(objectName))
    {
        add(objectName, dictionary());
    }

    return subDict(objectName);
}


void functionObjects::properties::clearTrigger()
{
    remove("triggerIndex");
}


label functionObjects::properties::getTrigger() const
{
    // Like getOrDefault, but without reporting missing entry (noisy)
    label idx = labelMin;
    readIfPresent("triggerIndex", idx);
    return idx;
}


bool functionObjects::properties::setTrigger
(
    const label triggeri,
    bool increaseOnly
)
{
    const label currTriggeri = getTrigger();

    if (increaseOnly ? (triggeri > currTriggeri) : (triggeri != currTriggeri))
    {
        set("triggerIndex", triggeri);
        return true;
    }

    // TBD: any special handling for triggeri == labelMin - eg, clearTrigger()

    return false;
}


bool functionObjects::properties::foundObjectProperty
(
    const word& objectName,
    const word& entryName
) const
{
    if (found(objectName))
    {
        const dictionary& baseDict = subDict(objectName);
        return baseDict.found(entryName);
    }

    return false;
}


bool functionObjects::properties::getObjectDict
(
    const word& objectName,
    const word& entryName,
    dictionary& dict
) const
{
    if (found(objectName))
    {
        const dictionary& baseDict = subDict(objectName);

        if (baseDict.found(entryName) && baseDict.isDict(entryName))
        {
            dict = baseDict.subDict(entryName);
            return true;
        }
    }

    return false;
}


bool functionObjects::properties::hasResultObject
(
    const word& objectName
) const
{
    if (found(resultsName_))
    {
        return subDict(resultsName_).found(objectName);
    }

    return false;
}


wordList functionObjects::properties::objectResultNames() const
{
    if (found(resultsName_))
    {
        return subDict(resultsName_).sortedToc();
    }

    return wordList();
}


bool functionObjects::properties::hasResultObjectEntry
(
    const word& objectName,
    const word& entryName
) const
{
    if (found(resultsName_))
    {
        const dictionary& resultsDict = subDict(resultsName_);

        if (resultsDict.found(objectName))
        {
            const dictionary& objectDict = resultsDict.subDict(objectName);

            for (const entry& dEntry : objectDict)
            {
                const dictionary& dict = dEntry.dict();

                if (dict.found(entryName))
                {
                    return true;
                }
            }
        }
    }

    return false;
}


word functionObjects::properties::objectResultType
(
    const word& objectName,
    const word& entryName
) const
{
    if (found(resultsName_))
    {
        const dictionary& resultsDict = subDict(resultsName_);

        if (resultsDict.found(objectName))
        {
            const dictionary& objectDict = resultsDict.subDict(objectName);

            for (const entry& dEntry : objectDict)
            {
                const dictionary& dict = dEntry.dict();

                if (dict.found(entryName))
                {
                    return dict.dictName();
                }
            }
        }
    }

    return word::null;
}


wordList functionObjects::properties::objectResultEntries
(
    const word& objectName
) const
{
    DynamicList<word> result;

    if (found(resultsName_))
    {
        const dictionary& resultsDict = subDict(resultsName_);

        if (resultsDict.found(objectName))
        {
            const dictionary& objectDict = resultsDict.subDict(objectName);

            for (const entry& dEntry : objectDict)
            {
                const dictionary& dict = dEntry.dict();

                result.append(dict.toc());
            }
        }
    }

    wordList entries;
    entries.transfer(result);

    return entries;
}


void functionObjects::properties::writeResultEntries
(
    const word& objectName,
    Ostream& os
) const
{
    if (found(resultsName_))
    {
        const dictionary& resultsDict = subDict(resultsName_);

        if (resultsDict.found(objectName))
        {
            const dictionary& objectDict = resultsDict.subDict(objectName);

            for (const word& dataFormat : objectDict.sortedToc())
            {
                os  << "    Type: " << dataFormat << nl;

                const dictionary& resultDict = objectDict.subDict(dataFormat);

                for (const word& result : resultDict.sortedToc())
                {
                    os << "        " << result << nl;
                }
            }
        }
    }
}


void functionObjects::properties::writeAllResultEntries
(
    Ostream& os
) const
{
    if (found(resultsName_))
    {
        const dictionary& resultsDict = subDict(resultsName_);

        for (const word& objectName : resultsDict.sortedToc())
        {
            os  << "Object: " << objectName << endl;

            writeResultEntries(objectName, os);
        }
    }
}


// ************************************************************************* //

 } // End namespace Foam
