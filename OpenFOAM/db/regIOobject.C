/*---------------------------------------------------------------------------*\
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

#include "regIOobject.H"
#include "Time.T.H"
#include "polyMesh.H"
#include "registerSwitch.H"
#include "fileOperation.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //
using namespace Foam;
namespace Foam
{
    defineTypeNameAndDebug(regIOobject, 0);
}

float regIOobject::fileModificationSkew
(
    debug::floatOptimisationSwitch("fileModificationSkew", 30)
);
registerOptSwitch
(
    "fileModificationSkew",
    float,
    regIOobject::fileModificationSkew
);


bool regIOobject::masterOnlyReading = false;


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

regIOobject::regIOobject(const IOobject& io, const bool isTime) :    IOobject(io),
    registered_(false),
    ownedByRegistry_(false),
    watchIndices_(),
    eventNo_                // Do not get event for top level Time database
    (
        isTime
      ? 0
      : db().getEvent()
    )
{
    // Register with objectRegistry if requested
    if (registerObject())
    {
        checkIn();
    }
}


regIOobject::regIOobject(const regIOobject& rio) :    IOobject(rio),
    registered_(false),
    ownedByRegistry_(false),
    watchIndices_(rio.watchIndices_),
    eventNo_(db().getEvent())
{
    // Do not register copy with objectRegistry
}


regIOobject::regIOobject(const regIOobject& rio, bool registerCopy) :    IOobject(rio),
    registered_(false),
    ownedByRegistry_(false),
    watchIndices_(),
    eventNo_(db().getEvent())
{
    if (registerCopy && rio.registered_)
    {
        const_cast<regIOobject&>(rio).checkOut();
        checkIn();
    }
}


regIOobject::regIOobject
(
    const word& newName,
    const regIOobject& rio,
    bool registerCopy
) :    IOobject(newName, rio.instance(), rio.local(), rio.db()),
    registered_(false),
    ownedByRegistry_(false),
    watchIndices_(),
    eventNo_(db().getEvent())
{
    if (registerCopy)
    {
        checkIn();
    }
}


regIOobject::regIOobject
(
    const IOobject& io,
    const regIOobject& rio
) :    IOobject(io),
    registered_(false),
    ownedByRegistry_(false),
    watchIndices_(),
    eventNo_(db().getEvent())
{
    if (registerObject())
    {
        checkIn();
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

regIOobject::~regIOobject()
{
    if (objectRegistry::debug)
    {
        Pout<< "Destroying regIOobject called " << name()
            << " of type " << type()
            << " in directory " << path()
            << endl;
    }

    // Check out of objectRegistry if not owned by the registry

    if (!ownedByRegistry_)
    {
        checkOut();
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool regIOobject::checkIn()
{
    if (!registered_)
    {
        // multiple checkin of same object is disallowed - this would mess up
        // any mapping
        registered_ = db().checkIn(*this);

        // check-in on defaultRegion is allowed to fail, since subsetted meshes
        // are created with the same name as their originating mesh
        if (!registered_ && debug && name() != polyMesh::defaultRegion)
        {
            if (debug == 2)
            {
                // for ease of finding where attempted duplicate check-in
                // originated
                FatalErrorInFunction
                    << "failed to register object " << objectPath()
                    << " the name already exists in the objectRegistry" << endl
                    << "Contents:" << db().sortedToc()
                    << abort(FatalError);
            }
            else
            {
                WarningInFunction
                    << "failed to register object " << objectPath()
                    << " the name already exists in the objectRegistry"
                    << endl;
            }
        }
    }

    return registered_;
}


bool regIOobject::checkOut()
{
    if (registered_)
    {
        registered_ = false;

        forAllReverse(watchIndices_, i)
        {
            fileHandler().removeWatch(watchIndices_[i]);
        }
        watchIndices_.clear();
        return db().checkOut(*this);
    }

    return false;
}


label regIOobject::addWatch(const fileName& f)
{
    label index = -1;

    if
    (
        registered_
     && readOpt() == MUST_READ_IF_MODIFIED
     && time().runTimeModifiable()
    )
    {
        index = fileHandler().findWatch(watchIndices_, f);

        if (index == -1)
        {
            index = watchIndices_.size();
            watchIndices_.append(fileHandler().addWatch(f));
        }
    }
    return index;
}


void regIOobject::addWatch()
{
    if
    (
        registered_
     && readOpt() == MUST_READ_IF_MODIFIED
     && time().runTimeModifiable()
    )
    {
        fileName f = filePath();
        if (!f.size())
        {
            // We don't have this file but would like to re-read it.
            // Possibly if master-only reading mode.
            f = objectPath();
        }

        label index = fileHandler().findWatch(watchIndices_, f);
        if (index != -1)
        {
            FatalErrorIn("regIOobject::addWatch()")
                << "Object " << objectPath() << " of type " << type()
                << " already watched with index " << watchIndices_[index]
                << abort(FatalError);
        }

        // If master-only reading only the master will have all dependencies
        // so scatter these to slaves
        bool masterOnly =
            global()
         && (
                regIOobject::fileModificationChecking == timeStampMaster
             || regIOobject::fileModificationChecking == inotifyMaster
            );

        if (masterOnly && Pstream::parRun())
        {
            // Get master watched files
            fileNameList watchFiles;
            if (Pstream::master())
            {
                watchFiles.setSize(watchIndices_.size());
                forAll(watchIndices_, i)
                {
                    watchFiles[i] = fileHandler().getFile(watchIndices_[i]);
                }
            }
            Pstream::scatter(watchFiles);

            if (!Pstream::master())
            {
                // unregister current ones
                forAllReverse(watchIndices_, i)
                {
                    fileHandler().removeWatch(watchIndices_[i]);
                }

                watchIndices_.clear();
                forAll(watchFiles, i)
                {
                    watchIndices_.append(fileHandler().addWatch(watchFiles[i]));
                }
            }
        }

        watchIndices_.append(fileHandler().addWatch(f));
    }
}


bool regIOobject::upToDate(const regIOobject& a) const
{
    if (a.eventNo() >= eventNo_)
    {
        return false;
    }
    else
    {
        return true;
    }
}


bool regIOobject::upToDate
(
    const regIOobject& a,
    const regIOobject& b
) const
{
    if
    (
        a.eventNo() >= eventNo_
     || b.eventNo() >= eventNo_
    )
    {
        return false;
    }
    else
    {
        return true;
    }
}


bool regIOobject::upToDate
(
    const regIOobject& a,
    const regIOobject& b,
    const regIOobject& c
) const
{
    if
    (
        a.eventNo() >= eventNo_
     || b.eventNo() >= eventNo_
     || c.eventNo() >= eventNo_
    )
    {
        return false;
    }
    else
    {
        return true;
    }
}


bool regIOobject::upToDate
(
    const regIOobject& a,
    const regIOobject& b,
    const regIOobject& c,
    const regIOobject& d
) const
{
    if
    (
        a.eventNo() >= eventNo_
     || b.eventNo() >= eventNo_
     || c.eventNo() >= eventNo_
     || d.eventNo() >= eventNo_
    )
    {
        return false;
    }
    else
    {
        return true;
    }
}


void regIOobject::setUpToDate()
{
    eventNo_ = db().getEvent();
}


void regIOobject::rename(const word& newName)
{
    // Check out of objectRegistry
    checkOut();

    IOobject::rename(newName);

    if (registerObject())
    {
        // Re-register object with objectRegistry
        checkIn();
    }
}


fileName regIOobject::filePath() const
{
    return localFilePath(type());
}


bool regIOobject::headerOk()
{
    // Note: Should be consistent with IOobject::typeHeaderOk(false)

    bool ok = true;

    fileName fName(filePath());

    ok = fileHandler().readHeader(*this, fName, type());

    if (!ok && IOobject::debug)
    {
        IOWarningInFunction(fName)
            << "failed to read header of file " << objectPath()
            << endl;
    }

    return ok;
}


void regIOobject::operator=(const IOobject& io)
{
    // Close any file
    isPtr_.clear();

    // Check out of objectRegistry
    checkOut();

    IOobject::operator=(io);

    if (registerObject())
    {
        // Re-register object with objectRegistry
        checkIn();
    }
}


// ************************************************************************* //
