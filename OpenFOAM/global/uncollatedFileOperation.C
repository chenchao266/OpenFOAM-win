﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2017 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
 2017-09-xx FSD blueCFD Lda: Modifications for blueCFD-Core 2017-1
------------------------------------------------------------------------------
License
    This file is a derivative work of OpenFOAM.

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

Modifications
    This file has been modified by blueCAPE's unofficial mingw patches for
    OpenFOAM.
    For more information about these patches, visit:
        http://bluecfd.com/Core

    Modifications made:
      - These changes are based on Symscape's own patches for Windows,
        circa 2009.
      - The adaptation derived from the patches for blueCFD-Core 4.x (2016).
      - Further adaptation was done for re-adapting to OpenFOAM 5 collated file
        management.

\*---------------------------------------------------------------------------*/

#include "uncollatedFileOperation.H"
#include "Time.T.H"
#include "IFstream.H"
#include "OFstream.H"
#include "addToRunTimeSelectionTable.H"
#include "masterUncollatedFileOperation.H"
#include "decomposedBlockData.H"
#include "dummyISstream.H"

/* * * * * * * * * * * * * * * Static Member Data  * * * * * * * * * * * * * */
using namespace Foam;
namespace Foam
{
namespace fileOperations
{
    defineTypeNameAndDebug(uncollatedFileOperation, 0);
    addToRunTimeSelectionTable(fileOperation, uncollatedFileOperation, word);
}
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

fileName fileOperations::uncollatedFileOperation::filePathInfo
(
    const bool checkGlobal,
    const bool isFile,
    const IOobject& io
) const
{
    const word & diskFileName = io.uniqueFileName();

    if (io.instance().isAbsolute())
    {
        fileName objectPath = io.instance()/diskFileName;

        if (isFileOrDir(isFile, objectPath))
        {
            return objectPath;
        }
        else
        {
            return fileName::null;
        }
    }
    else
    {
        fileName path = io.path();
        fileName objectPath = path/diskFileName;

        if (isFileOrDir(isFile, objectPath))
        {
            return objectPath;
        }
        else
        {
            if
            (
                checkGlobal
             && io.time().processorCase()
             && (
                    io.instance() == io.time().system()
                 || io.instance() == io.time().constant()
                )
            )
            {
                // Constant & system can come from global case

                fileName parentObjectPath =
                    io.rootPath()/io.time().globalCaseName()
                   /io.instance()/io.db().dbDir()/io.local()/diskFileName;

                if (isFileOrDir(isFile, parentObjectPath))
                {
                    return parentObjectPath;
                }
            }

            // Check if parallel "procesors" directory
            if (io.time().processorCase())
            {
                fileName path = fileOperations::masterUncollatedFileOperation::
                processorsPath
                (
                    io,
                    io.instance()
                );
                fileName objectPath = path/diskFileName;

                if (isFileOrDir(isFile, objectPath))
                {
                    return objectPath;
                }
            }


            // Check for approximately same time. E.g. if time = 1e-2 and
            // directory is 0.01 (due to different time formats)
            if (!isDir(path))
            {
                word newInstancePath = io.time().findInstancePath
                (
                    instant(io.instance())
                );

                if (newInstancePath.size())
                {
                    fileName fName
                    (
                        io.rootPath()/io.caseName()
                       /newInstancePath/io.db().dbDir()/io.local()/diskFileName
                    );

                    if (isFileOrDir(isFile, fName))
                    {
                        return fName;
                    }
                }
            }
        }

        return fileName::null;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

fileOperations::uncollatedFileOperation::uncollatedFileOperation
(
    const bool verbose
)
{
    if (verbose)
    {
        Info<< "I/O    : " << typeName << endl;
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

fileOperations::uncollatedFileOperation::~uncollatedFileOperation()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool fileOperations::uncollatedFileOperation::mkDir
(
    const fileName& dir,
    mode_t mode
) const
{
    return mkDir(dir, mode);
}


bool fileOperations::uncollatedFileOperation::chMod
(
    const fileName& fName,
    mode_t mode
) const
{
    return chMod(fName, mode);
}


mode_t fileOperations::uncollatedFileOperation::mode
(
    const fileName& fName,
    const bool followLink
) const
{
    return mode(fName, followLink);
}


fileName::Type fileOperations::uncollatedFileOperation::type
(
    const fileName& fName,
    const bool followLink
) const
{
    return type(fName, followLink);
}


bool fileOperations::uncollatedFileOperation::exists
(
    const fileName& fName,
    const bool checkGzip,
    const bool followLink
) const
{
    return exists(fName, checkGzip, followLink);
}


bool fileOperations::uncollatedFileOperation::isDir
(
    const fileName& fName,
    const bool followLink
) const
{
    return isDir(fName, followLink);
}


bool fileOperations::uncollatedFileOperation::isFile
(
    const fileName& fName,
    const bool checkGzip,
    const bool followLink
) const
{
    return isFile(fName, checkGzip, followLink);
}


offset_t fileOperations::uncollatedFileOperation::fileSize
(
    const fileName& fName,
    const bool followLink
) const
{
    return fileSize(fName, followLink);
}


time_t fileOperations::uncollatedFileOperation::lastModified
(
    const fileName& fName,
    const bool followLink
) const
{
    return lastModified(fName, followLink);
}


double fileOperations::uncollatedFileOperation::highResLastModified
(
    const fileName& fName,
    const bool followLink
) const
{
    return highResLastModified(fName, followLink);
}


bool fileOperations::uncollatedFileOperation::mvBak
(
    const fileName& fName,
    const std::string& ext
) const
{
    return mvBak(fName, ext);
}


bool fileOperations::uncollatedFileOperation::rm
(
    const fileName& fName
) const
{
    return rm(fName);
}


bool fileOperations::uncollatedFileOperation::rmDir
(
    const fileName& dir
) const
{
    return rmDir(dir);
}


fileNameList fileOperations::uncollatedFileOperation::readDir
(
    const fileName& dir,
    const fileName::Type type,
    const bool filtergz,
    const bool followLink
) const
{
    return readDir(dir, type, filtergz, followLink);
}


bool fileOperations::uncollatedFileOperation::cp
(
    const fileName& src,
    const fileName& dst,
    const bool followLink
) const
{
    return cp(src, dst, followLink);
}


bool fileOperations::uncollatedFileOperation::ln
(
    const fileName& src,
    const fileName& dst
) const
{
    return ln(src, dst);
}


bool fileOperations::uncollatedFileOperation::mv
(
    const fileName& src,
    const fileName& dst,
    const bool followLink
) const
{
    return mv(src, dst, followLink);
}


fileName fileOperations::uncollatedFileOperation::filePath
(
    const bool checkGlobal,
    const IOobject& io,
    const word& typeName
) const
{
    if (debug)
    {
        Pout<< "uncollatedFileOperation::filePath :"
            << " objectPath:" << io.objectPath()
            << " checkGlobal:" << checkGlobal << endl;
    }

    fileName objPath(filePathInfo(checkGlobal, true, io));

    if (debug)
    {
        Pout<< "uncollatedFileOperation::filePath :"
            << " Returning from file searching:" << endl
            << "    objectPath:" << io.objectPath() << endl
            << "    filePath  :" << objPath << endl << endl;
    }
    return objPath;
}


fileName fileOperations::uncollatedFileOperation::dirPath
(
    const bool checkGlobal,
    const IOobject& io
) const
{
    if (debug)
    {
        Pout<< "uncollatedFileOperation::dirPath :"
            << " objectPath:" << io.objectPath()
            << " checkGlobal:" << checkGlobal << endl;
    }

    fileName objPath(filePathInfo(checkGlobal, false, io));

    if (debug)
    {
        Pout<< "uncollatedFileOperation::dirPath :"
            << " Returning from directory searching:" << endl
            << "    objectPath:" << io.objectPath() << endl
            << "    dirPath   :" << objPath << endl << endl;
    }
    return objPath;
}


fileNameList fileOperations::uncollatedFileOperation::readObjects
(
    const objectRegistry& db,
    const fileName& instance,
    const fileName& local,
    word& newInstance
) const
{
    if (debug)
    {
        Pout<< "uncollatedFileOperation::readObjects :"
            << " db:" << db.objectPath()
            << " instance:" << instance << endl;
    }

    //- Use non-time searching version
    fileNameList objectNames
    (
        fileOperation::readObjects(db, instance, local, newInstance)
    );

    if (newInstance.empty())
    {
        // Find similar time
        fileName newInst = db.time().findInstancePath(instant(instance));
        if (!newInst.empty() && newInst != instance)
        {
            // Try with new time
            objectNames = fileOperation::readObjects
            (
                db,
                newInst,
                local,
                newInstance
            );
        }
    }

    if (debug)
    {
        Pout<< "uncollatedFileOperation::readObjects :"
            << " newInstance:" << newInstance
            << " objectNames:" << objectNames << endl;
    }

    return objectNames;
}


bool fileOperations::uncollatedFileOperation::readHeader
(
    IOobject& io,
    const fileName& fName,
    const word& typeName
) const
{
    if (fName.empty())
    {
        if (IOobject::debug)
        {
            InfoInFunction
                << "file " << io.objectPath() << " could not be opened"
                << endl;
        }

        return false;
    }

    autoPtr<ISstream> isPtr(NewIFstream(fName));

    if (!isPtr.valid() || !isPtr->good())
    {
        return false;
    }

    bool ok = io.readHeader(isPtr());

    if (io.headerClassName() == decomposedBlockData::typeName)
    {
        // Read the header inside the container (master data)
        ok = decomposedBlockData::readMasterHeader(io, isPtr());
    }

    return ok;
}


autoPtr<ISstream>
fileOperations::uncollatedFileOperation::readStream
(
    regIOobject& io,
    const fileName& fName,
    const word& typeName,
    const bool valid
) const
{
    autoPtr<ISstream> isPtr;

    if (!valid)
    {
        isPtr = autoPtr<ISstream>(new dummyISstream());
        return isPtr;
    }

    if (fName.empty())
    {
        FatalErrorInFunction
            << "cannot find file " << io.objectPath()
            << exit(FatalError);
    }

    isPtr = NewIFstream(fName);

    if (!isPtr.valid() || !isPtr->good())
    {
        FatalIOError
        (
            "uncollatedFileOperation::readStream()",
            __FILE__,
            __LINE__,
            fName,
            0
        )   << "cannot open file"
            << exit(FatalIOError);
    }
    else if (!io.readHeader(isPtr()))
    {
        FatalIOErrorInFunction(isPtr())
            << "problem while reading header for object " << io.name()
            << exit(FatalIOError);
    }

    if (io.headerClassName() != decomposedBlockData::typeName)
    {
        return isPtr;
    }
    else
    {
        // Analyse the objectpath to find out the processor we're trying
        // to access
        fileName path;
        fileName local;
        label proci = fileOperations::masterUncollatedFileOperation::
        splitProcessorPath
        (
            io.objectPath(),
            path,
            local
        );

        if (proci == -1)
        {
            FatalIOErrorInFunction(isPtr())
                << "could not detect processor number"
                << " from objectPath:" << io.objectPath()
                << exit(FatalIOError);
        }

        // Read data and return as stream
        return decomposedBlockData::readBlock(proci, isPtr(), io);
    }
}


bool fileOperations::uncollatedFileOperation::read
(
    regIOobject& io,
    const bool masterOnly,
    const IOstream::streamFormat format,
    const word& typeName
) const
{
    bool ok = true;
    if (Pstream::master() || !masterOnly)
    {
        if (debug)
        {
            Pout<< "uncollatedFileOperation::read() : "
                << "reading object " << io.objectPath()
                << " from file " << endl;
        }

        // Set flag for e.g. codeStream
        const bool oldGlobal = io.globalObject();
        io.globalObject() = masterOnly;
        // If codeStream originates from dictionary which is
        // not IOdictionary we have a problem so use global
        const bool oldFlag = regIOobject::masterOnlyReading;
        regIOobject::masterOnlyReading = masterOnly;

        // Read file
        ok = io.readData(io.readStream(typeName));
        io.close();

        // Restore flags
        io.globalObject() = oldGlobal;
        regIOobject::masterOnlyReading = oldFlag;
    }

    if (masterOnly && Pstream::parRun())
    {
        // Master reads headerclassname from file. Make sure this gets
        // transfered as well as contents.
        Pstream::scatter(io.headerClassName());
        Pstream::scatter(io.note());

        // Get my communication order
        const List<Pstream::commsStruct>& comms =
        (
            (Pstream::nProcs() < Pstream::nProcsSimpleSum)
          ? Pstream::linearCommunication()
          : Pstream::treeCommunication()
        );
        const Pstream::commsStruct& myComm = comms[Pstream::myProcNo()];

        // Reveive from up
        if (myComm.above() != -1)
        {
            IPstream fromAbove
            (
                Pstream::commsTypes::scheduled,
                myComm.above(),
                0,
                Pstream::msgType(),
                Pstream::worldComm,
                format
            );
            ok = io.readData(fromAbove);
        }

        // Send to my downstairs neighbours
        forAll(myComm.below(), belowI)
        {
            OPstream toBelow
            (
                Pstream::commsTypes::scheduled,
                myComm.below()[belowI],
                0,
                Pstream::msgType(),
                Pstream::worldComm,
                format
            );
            bool okWrite = io.writeData(toBelow);
            ok = ok && okWrite;
        }
    }
    return ok;
}


autoPtr<ISstream>
fileOperations::uncollatedFileOperation::NewIFstream
(
    const fileName& filePath
) const
{
    return autoPtr<ISstream>(new IFstream(filePath));
}


autoPtr<Ostream>
fileOperations::uncollatedFileOperation::NewOFstream
(
    const fileName& pathName,
    IOstream::streamFormat fmt,
    IOstream::versionNumber ver,
    IOstream::compressionType cmp,
    const bool valid
) const
{
    return autoPtr<Ostream>(new OFstream(pathName, fmt, ver, cmp));
}


// ************************************************************************* //
