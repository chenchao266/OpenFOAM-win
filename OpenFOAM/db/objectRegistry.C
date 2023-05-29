/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
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

#include "objectRegistry.H"
#include "Time1.h"
#include "predicates.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(objectRegistry, 0);
}


// * * * * * * * * * * * * * * * Local Functions * * * * * * * * * * * * * * //

namespace Foam
{

// Templated implementation for erase() with iterator range.
// Prefer not to expose directly.
template<class InputIter>
static label eraseImpl(objectRegistry& obr, InputIter first, InputIter last)
{
    label changed = 0;

    for
    (
        const label nTotal = obr.size();
        changed < nTotal && first != last; // Terminate early
        ++first
    )
    {
        if (obr.erase(*first))
        {
            ++changed;
        }
    }

    return changed;
}

} // End namespace Foam


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


 namespace Foam{
bool objectRegistry::parentNotTime() const noexcept
{
    return (&parent_ != static_cast<const objectRegistry*>(&time_));
}


// * * * * * * * * * * * * * * * * Constructors *  * * * * * * * * * * * * * //

objectRegistry::objectRegistry(const Time& t, const label nObjects)
:
    regIOobject
    (
        IOobject
        (
            word::validate(t.caseName()),
            t.path(),
            t,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE,
            false
        ),
        true    // to flag that this is the top-level regIOobject
    ),
    HashTable<regIOobject*>(nObjects),
    time_(t),
    parent_(t),
    dbDir_(name()),
    event_(1)
{}


objectRegistry::objectRegistry(const IOobject& io, const label nObjects)
:
    regIOobject(io),
    HashTable<regIOobject*>(nObjects),
    time_(io.time()),
    parent_(io.db()),
    dbDir_(parent_.dbDir()/local()/name()),
    event_(1)
{
    writeOpt(IOobject::AUTO_WRITE);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

objectRegistry::~objectRegistry()
{
    objectRegistry::clear();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool objectRegistry::isTimeDb() const noexcept
{
    return (this == &static_cast<const objectRegistry&>(time_));
}


HashTable<wordHashSet> objectRegistry::classes() const
{
    return classesImpl(*this, predicates::always());
}


label objectRegistry::count(const char* clsName) const
{
    // No nullptr check - only called with string literals
    return count(static_cast<word>(clsName));
}


wordList objectRegistry::names() const
{
    return HashTable<regIOobject*>::toc();
}


wordList objectRegistry::sortedNames() const
{
    return HashTable<regIOobject*>::sortedToc();
}


wordList objectRegistry::names(const char* clsName) const
{
    // No nullptr check - only called with string literals
    return names(static_cast<word>(clsName));
}


wordList objectRegistry::sortedNames(const char* clsName) const
{
    // No nullptr check - only called with string literals
    return sortedNames(static_cast<word>(clsName));
}


const objectRegistry& objectRegistry::subRegistry
(
    const word& name,
    const bool forceCreate,
    const bool recursive
) const
{
    if (forceCreate && !foundObject<objectRegistry>(name, recursive))
    {
        objectRegistry* subObr = new objectRegistry
        (
            IOobject
            (
                name,
                time().constant(),
                *this,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            )
        );
        subObr->store();
    }

    return lookupObject<objectRegistry>(name, recursive);
}


label objectRegistry::getEvent() const
{
    label curEvent = event_++;

    if (event_ == labelMax)
    {
        if (objectRegistry::debug)
        {
            WarningInFunction
                << "Event counter has overflowed. "
                << "Resetting counter on all dependent objects." << nl
                << "This might cause extra evaluations." << endl;
        }

        // Reset event counter
        curEvent = 1;
        event_ = 2;

        // No need to reset dependent objects; overflow is now handled
        // in regIOobject::upToDate
    }

    return curEvent;
}


bool objectRegistry::checkIn(regIOobject* io) const
{
    if (!io) return false;

    if (objectRegistry::debug)
    {
        Pout<< "objectRegistry::checkIn : "
            << name() << " : checking in " << io->name()
            << " of type " << io->type()
            << endl;
    }

    objectRegistry& obr = const_cast<objectRegistry&>(*this);

    bool ok = obr.insert(io->name(), io);

    if (!ok && objectRegistry::debug)
    {
        WarningInFunction
            << name() << " : Attempt to checkIn object with name "
            << io->name() << " which was already checked in"
            << endl;
    }

    return ok;
}


bool objectRegistry::checkOut(regIOobject* io) const
{
    if (!io) return false;

    objectRegistry& obr = const_cast<objectRegistry&>(*this);

    iterator iter = obr.find(io->name());

    if (iter.found())
    {
        if (objectRegistry::debug)
        {
            Pout<< "objectRegistry::checkOut : "
                << name() << " : checking out " << io->name()
                << " of type " << io->type()
                << endl;
        }

        if (iter.val() != io)
        {
            if (objectRegistry::debug)
            {
                WarningInFunction
                    << name() << " : Attempt to checkOut copy of "
                    << iter.key()
                    << endl;
            }

            return false;
        }

        return obr.erase(iter);
    }


    if (objectRegistry::debug)
    {
        Pout<< "objectRegistry::checkOut : "
            << name() << " : could not find " << io->name() << " in registry"
            << endl;
    }

    return false;
}


bool objectRegistry::checkIn(regIOobject& io) const
{
    return checkIn(&io);
}


bool objectRegistry::checkOut(regIOobject& io) const
{
    return checkOut(&io);
}


bool objectRegistry::checkOut(const word& key) const
{
    return const_cast<objectRegistry&>(*this).erase(key);
}


void objectRegistry::clear()
{
    // Free anything owned by the registry, but first unset both
    // 'ownedByRegistry' and 'registered' flags to ensure that the
    // regIOobject destructor will not affect the registry

    for (iterator iter = begin(); iter != end(); ++iter)
    {
        regIOobject* ptr = iter.val();

        if (ptr && ptr->ownedByRegistry())
        {
            if (objectRegistry::debug)
            {
                Pout<< "objectRegistry::clear : " << ptr->name() << nl;
            }

            ptr->release(true);     // Relinquish ownership and registration
            delete ptr;             // Delete also clears fileHandler watches
        }
    }

    HashTable<regIOobject*>::clear();
}


void objectRegistry::clearStorage()
{
    objectRegistry::clear();
    HashTable<regIOobject*>::clearStorage();
}


bool objectRegistry::erase(const iterator& iter)
{
    // Remove from registry - see notes in objectRegistry::clear()

    if (iter.found())
    {
        regIOobject* ptr = const_cast<iterator&>(iter).val();

        const bool ok = HashTable<regIOobject*>::erase(iter);

        if (ptr && ptr->ownedByRegistry())
        {
            ptr->release(true);     // Relinquish ownership and registration
            delete ptr;             // Delete also clears fileHandler watches
        }

        return ok;
    }

    return false;
}


bool objectRegistry::erase(const word& key)
{
    return erase(find(key));
}


label objectRegistry::erase(std::initializer_list<word> keys)
{
    return eraseImpl(*this, keys.begin(), keys.end());
}


label objectRegistry::erase(const UList<word>& keys)
{
    return eraseImpl(*this, keys.begin(), keys.end());
}


void objectRegistry::rename(const word& newName)
{
    regIOobject::rename(newName);

    // Adjust dbDir_ as well
    const auto i = dbDir_.rfind('/');

    if (i == string::npos)
    {
        dbDir_ = newName;
    }
    else
    {
        dbDir_.replace(i+1, string::npos, newName);
    }
}


const regIOobject* objectRegistry::cfindIOobject
(
    const word& name,
    const bool recursive
) const
{
    const_iterator iter = cfind(name);

    if (iter.found())
    {
        return iter.val();
    }
    else if (recursive && this->parentNotTime())
    {
        return parent_.cfindIOobject(name, recursive);
    }

    return nullptr;
}


bool objectRegistry::found
(
    const word& name,
    const bool recursive
) const
{
    return cfindIOobject(name, recursive);
}


bool objectRegistry::modified() const
{
    for (const_iterator iter = cbegin(); iter != cend(); ++iter)
    {
        if ((*iter)->modified())
        {
            return true;
        }
    }

    return false;
}


void objectRegistry::readModifiedObjects()
{
    for (iterator iter = begin(); iter != end(); ++iter)
    {
        if (objectRegistry::debug)
        {
            Pout<< "objectRegistry::readModifiedObjects() : "
                << name() << " : Considering reading object "
                << iter.key() << endl;
        }

        (*iter)->readIfModified();
    }
}


bool objectRegistry::readIfModified()
{
    readModifiedObjects();
    return true;
}


bool objectRegistry::writeObject
(
    IOstreamOption streamOpt,
    const bool valid
) const
{
    bool ok = true;

    for (const_iterator iter = cbegin(); iter != cend(); ++iter)
    {
        if (objectRegistry::debug)
        {
            Pout<< "objectRegistry::write() : "
                << name() << " : Considering writing object "
                << iter.key()
                << " of type " << (*iter)->type()
                << " with writeOpt " << static_cast<int>((*iter)->writeOpt())
                << " to file " << (*iter)->objectPath()
                << endl;
        }

        if ((*iter)->writeOpt() != NO_WRITE)
        {
            ok = (*iter)->writeObject(streamOpt, valid) && ok;
        }
    }

    return ok;
}


// ************************************************************************* //

 } // End namespace Foam
