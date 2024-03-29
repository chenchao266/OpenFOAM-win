﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2017-2018 OpenFOAM Foundation
    Copyright (C) 2019-2021 OpenCFD Ltd.
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

#include "fileOperation.H"
#include "uncollatedFileOperation.H"
#include "regIOobject.H"
#include "argList.H"
#include "HashSet.H"
#include "objectRegistry.H"
#include "decomposedBlockData.H"
#include "polyMesh.H"
#include "registerSwitch.H"
#include "Time1.h"
#include "ITstream.H"
#include <cerrno>
#include <cinttypes>
#include <cctype>
 

/* * * * * * * * * * * * * * * Static Member Data  * * * * * * * * * * * * * */

namespace Foam
{
    defineTypeNameAndDebug(fileOperation, 0);
    defineRunTimeSelectionTable(fileOperation, word);

    word fileOperation::defaultFileHandler
    (
        debug::optimisationSwitches().getOrAdd<word>
        (
            "fileHandler",
            //Foam::fileOperations::uncollatedFileOperation::typeName,
            "uncollated",
            keyType::LITERAL
        )
    );
 

const Enum<fileOperation::pathType>
fileOperation::pathTypeNames_
({
    { fileOperation::NOTFOUND, "notFound" },
    { fileOperation::ABSOLUTE, "absolute" },
    { fileOperation::OBJECT, "objectPath" },
    { fileOperation::WRITEOBJECT, "writeObject" },
    { fileOperation::PROCUNCOLLATED, "uncollatedProc" },
    { fileOperation::PROCBASEOBJECT, "globalProc" },
    { fileOperation::PROCOBJECT, "localProc" },
    { fileOperation::PARENTOBJECT, "parentObjectPath" },
    { fileOperation::FINDINSTANCE, "findInstance" },
    { fileOperation::PROCUNCOLLATEDINSTANCE, "uncollatedProcInstance" },
    { fileOperation::PROCBASEINSTANCE, "globalProcInstance" },
    { fileOperation::PROCINSTANCE, "localProcInstance" }
});


word fileOperation::processorsBaseDir = "processors";

autoPtr<fileOperation> fileOperation::fileHandlerPtr_;


// * * * * * * * * * * * * * * * Local Functions * * * * * * * * * * * * * * //

namespace
{

// Need to parse the numbers
// from "processors(\d+)" and
// from "processors(\d+)_(\d+)-(\d+)"
//
// Receive the string matching "^(\d+)(?:_(\d+)-(\d+))?/?$"
//
//    \1 = numProcs
//    \2 = firstProc
//    \3 = lastProc
//
// Return true on success and set parameters numProcs and group (size,start)
//
// Use low-level C-string to integer parsing to drive the sequence.
//
// For simplicity, also skip INT_MAX checks everywhere but check for
// - (errno) for success
// - (nptr == endptr) for leading junk
// - (*endptr != endChar) for trailing junk
// - skip INT_MAX checks as being too pessimistic

static bool parseProcsNumRange
(
    const std::string str,
    int& numProcs,
    fileOperation::procRangeType& group
)
{
    const char * nptr = str.c_str();
    char *endptr = nullptr;

    // 1. numProcs
    errno = 0;
    intmax_t parsed = std::strtoimax(nptr, &endptr, 10);
    if (errno || nptr == endptr) return false;  // bad parse

    const int nProcs = int(parsed);

    // End of string? Then no range and we are done.
    if (*endptr == '\0')
    {
        numProcs = nProcs;
        return true;
    }

    // Parse point at start of range ('_' character)?
    if (*endptr != '_') return false;
    nptr = ++endptr;


    // 2. firstProc
    errno = 0;
    parsed = std::strtoimax(nptr, &endptr, 10);
    if (errno || nptr == endptr) return false;  // bad parse

    const int firstProc = int(parsed);

    // Parse point at range separator ('-' character)?
    if (*endptr != '-') return false;
    nptr = ++endptr;


    // 3. lastProc
    errno = 0;
    parsed = std::strtoimax(nptr, &endptr, 10);
    if (errno || nptr == endptr) return false;  // bad parse

    const int lastProc = int(parsed);


    if
    (
        // Parse point at end of string
        (*endptr == '\0')

        // Input plausibility
        // Accept nProcs == 0 in case that becomes useful in the future
     && (nProcs >= 0 && firstProc >= 0 && firstProc <= lastProc)
    )
    {
        numProcs = nProcs;

        // Convert first/last to start/size
        group.reset(firstProc, lastProc-firstProc+1);

        return true;
    }

    return false;
}

} // End anonymous namespace


#if 0

// Sorting of processor directories
#include "stringOpsSort.H"
namespace
{

// Sort processor directory names (natural order)
// - not strictly necessary
void sortProcessorDirs(UList<fileOperation::dirIndex>& dirs)
{
    if (dirs.size() > 1)
    {
        std::stable_sort
        (
            dirs.begin(),
            dirs.end(),
            []
            (
                const fileOperation::dirIndex& a,
                const fileOperation::dirIndex& b
            ) -> bool
            {
                return
                    stringOps::natural_sort::compare
                    (
                        a.first(),
                        b.first()
                    ) < 0;
            }
        );
    }
}

} // End anonymous namespace
#endif

// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

labelList fileOperation::ioRanks()
{
    labelList ranks;

    ITstream is(getEnv("FOAM_IORANKS"));
    if (!is.empty())
    {
        is >> ranks;
    }

    return ranks;
}


instantList
fileOperation::sortTimes
(
    const fileNameList& dirEntries,
    const word& constantName
)
{
    // Check for "constant"
    bool haveConstant = false;

    if (!constantName.empty())
    {
        for (const fileName& dirName : dirEntries)
        {
            if (dirName == constantName)
            {
                haveConstant = true;
                break;
            }
        }
    }

    instantList times(dirEntries.size() + 1);
    label nTimes = 0;

    if (haveConstant)
    {
        times[nTimes].value() = 0;
        times[nTimes].name() = constantName;
        ++nTimes;
    }

    // Parse directory entries for scalar values
    for (const fileName& dirName : dirEntries)
    {
        if (readScalar(dirName, times[nTimes].value()))
        {
            times[nTimes].name() = dirName;
            ++nTimes;
        }
    }

    times.resize(nTimes);

    if (haveConstant)
    {
        if (nTimes > 2)
        {
            std::sort(&times[1], times.end(), instant::less());
        }
    }
    else if (nTimes > 1)
    {
        std::sort(times.begin(), times.end(), instant::less());
    }

    return times;
}


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

fileMonitor& fileOperation::monitor() const
{
    if (!monitorPtr_)
    {
        monitorPtr_.reset
        (
            new fileMonitor
            (
                IOobject::fileModificationChecking == IOobject::inotify
             || IOobject::fileModificationChecking == IOobject::inotifyMaster
            )
        );
    }
    return *monitorPtr_;
}


void fileOperation::mergeTimes
(
    const instantList& extraTimes,
    const word& constantName,
    instantList& times
)
{
    if (extraTimes.size())
    {
        const bool haveConstant =
        (
            times.size()
         && times[0].name() == constantName
        );

        const bool haveExtraConstant =
        (
            extraTimes.size()
         && extraTimes[0].name() == constantName
        );

        // Combine times
        instantList combinedTimes(times.size()+extraTimes.size());
        label sz = 0;
        label extrai = 0;
        if (haveExtraConstant)
        {
            extrai = 1;
            if (!haveConstant)
            {
                combinedTimes[sz++] = extraTimes[0];    // constant
            }
        }
        forAll(times, i)
        {
            combinedTimes[sz++] = times[i];
        }
        for (; extrai < extraTimes.size(); extrai++)
        {
            combinedTimes[sz++] = extraTimes[extrai];
        }
        combinedTimes.setSize(sz);
        times.transfer(combinedTimes);

        // Sort
        if (times.size() > 1)
        {
            label starti = 0;
            if (times[0].name() == constantName)
            {
                starti = 1;
            }
            std::sort(&times[starti], times.end(), instant::less());

            // Filter out duplicates
            label newi = starti+1;
            for (label i = newi; i < times.size(); i++)
            {
                if (times[i].value() != times[i-1].value())
                {
                    if (newi != i)
                    {
                        times[newi] = times[i];
                    }
                    newi++;
                }
            }

            times.setSize(newi);
        }
    }
}


bool fileOperation::isFileOrDir(const bool isFile, const fileName& f)
{
    return (isFile ? ::Foam::isFile(f) : ::Foam::isDir(f));
}


refPtr<fileOperation::dirIndexList>
fileOperation::lookupAndCacheProcessorsPath
(
    const fileName& fName,
    const bool syncPar
) const
{
    // If path is local to a processor (e.g. contains 'processor2')
    // find the corresponding actual processor directory (e.g. 'processors4')
    // and index (2)

    fileName path, pDir, local;
    procRangeType group;
    label numProcs;
    const label proci =
        splitProcessorPath(fName, path, pDir, local, group, numProcs);

    if (proci != -1)
    {
        const fileName procPath(path/pDir);

        const auto iter = procsDirs_.cfind(procPath);

        if (iter.found())
        {
            return iter.val();
        }

        DynamicList<dirIndex> procDirs;
        fileNameList dirEntries;

        // Read all directories to see any beginning with processor
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        // Note: use parallel synchronised reading so cache will be same
        //       order on all processors

        const bool readDirMasterOnly
        (
            Pstream::parRun() && !distributed()
         &&
            (
                IOobject::fileModificationChecking == IOobject::timeStampMaster
             || IOobject::fileModificationChecking == IOobject::inotifyMaster
            )
        );

        // The above selection excludes masterUncollated, which uses inotify or
        // timeStamp but provides its own internals for readDir() anyhow.

        if (readDirMasterOnly)
        {
            // Parallel and non-distributed
            // Read on master only and send to subProcs

            if (Pstream::master(comm_))
            {
                dirEntries = readDir(path, fileName::Type::DIRECTORY);

                DebugInfo
                    << "readDir on master: send " << dirEntries.size()
                    << " names to sub-processes" << endl;
            }

            Pstream::scatter(dirEntries, Pstream::msgType(), comm_);
        }
        else
        {
            // Serial or distributed roots.
            // Handle readDir() with virtual method

            if (debug)
            {
                Pout<< "readDir without special master/send treatment"
                    << endl;
            }

            dirEntries = readDir(path, fileName::Type::DIRECTORY);
        }

        // Extract info from processorN or processorsNN
        // - highest processor number
        // - directory+offset containing data for proci

        label nProcs = 0;
        for (const fileName& dirN : dirEntries)
        {
            // Analyse directory name
            fileName rp, rd, rl;
            label rNum;
            const label readProci =
                splitProcessorPath(dirN, rp, rd, rl, group, rNum);

            nProcs = max(nProcs, readProci+1);

            Tuple2<pathType, int> pathTypeIdx(pathType::NOTFOUND, 0);

            if (proci == readProci)
            {
                // Found "processorN"
                pathTypeIdx.first() = pathType::PROCUNCOLLATED;
            }
            else if (rNum != -1)
            {
                // "processorsNN" or "processorsNN_start-end"
                nProcs = max(nProcs, rNum);

                if (group.empty())
                {
                    // "processorsNN"

                    if (proci < rNum)
                    {
                        // And it is also in range.
                        // Eg for "processors4": 3 is ok, 10 is not

                        pathTypeIdx.first() = pathType::PROCBASEOBJECT;
                        pathTypeIdx.second() = proci;
                    }
                }
                else if (group.found(proci))
                {
                    // "processorsNN_start-end"
                    // - save the local proc offset

                    pathTypeIdx.first() = pathType::PROCOBJECT;
                    pathTypeIdx.second() = (proci - group.start());
                }
            }

            if (pathTypeIdx.first() != pathType::NOTFOUND)
            {
                procDirs.append(dirIndex(dirN, pathTypeIdx));
            }
        }

        // Global check of empty/exists.
        // 1 : empty directory
        // 2 : non-empty directory
        // 3 : mixed empty/non-empty directory (after reduce)
        // Combines andOp<bool>() and orOp<bool>() in single operation

        unsigned procDirsStatus = (procDirs.empty() ? 1u : 2u);

        if (debug)
        {
            Pout<< "fileOperation::lookupProcessorsPath " << procPath
                << " detected:" << procDirs << endl;
        }

        if (Pstream::parRun() && (!distributed() || syncPar))
        {
            reduce(procDirsStatus, bitOrOp<unsigned>());  // worldComm

            if (procDirsStatus == 3u)
            {
                // Mixed empty/exists for procDirs.
                // Synthesize missing directory name (consistency in cache
                // existence).
                // Cannot reliably synthesize RANK-COLLATED, only COLLATED or
                // UNCOLLATED.
                //
                // RANK-COLLATED should have been read from its corresponding
                // master anyhow

                int flavour(pathType::PROCUNCOLLATED);
                for (const dirIndex& pDir : procDirs)
                {
                    flavour = max(flavour, int(pDir.second().first()));
                }

                reduce(nProcs, maxOp<label>());  // worldComm
                reduce(flavour, maxOp<int>());   // worldComm

                if (procDirs.empty())
                {
                    Tuple2<pathType, int> pathTypeIdx(pathType(flavour), 0);

                    if
                    (
                        pathTypeIdx.first() == pathType::PROCBASEOBJECT
                     && proci < nProcs
                    )
                    {
                        pathTypeIdx.second() = proci;

                        procDirs.append
                        (
                            dirIndex
                            (
                                processorsBaseDir + name(nProcs),
                                pathTypeIdx
                            )
                        );
                    }
                    else
                    {
                        // - pathType::PROCUNCOLLATED
                        // - poor fallback for pathType::PROCOBJECT
                        // - out-of-range pathType::PROCBASEOBJECT

                        procDirs.append
                        (
                            dirIndex
                            (
                                "processor" + name(proci),
                                pathTypeIdx
                            )
                        );
                    }

                    if (debug)
                    {
                        Pout<< "fileOperation::lookupProcessorsPath "
                            << procPath
                            << " synthetic:" << procDirs << endl;
                    }
                }
            }
        }
        else if (!Pstream::parRun())
        {
            // Serial: use the number of decompositions (if found)
            if (nProcs)
            {
                const_cast<fileOperation&>(*this).setNProcs(nProcs);
            }
        }

        // Sort processor directory names (natural order)
        /// sortProcessorDirs(procDirs);

        if (procDirsStatus & 2u)
        {
            procsDirs_.insert(procPath, procDirs);

            // Make sure to return a reference
            return procsDirs_[procPath];
        }
    }

    return refPtr<dirIndexList>::New();
}


refPtr<fileOperation::dirIndexList>
fileOperation::lookupProcessorsPath(const fileName& fName) const
{
    // Use parallel synchronisation
    return lookupAndCacheProcessorsPath(fName, true);
}


bool fileOperation::exists(IOobject& io) const
{
    // Generate output filename for object
    fileName objPath(objectPath(io, word::null));

    // Test for either directory or a (valid) file & IOobject
    bool ok;
    if (io.name().empty())
    {
        ok = isDir(objPath);
    }
    else
    {
        ok =
            isFile(objPath)
         && io.typeHeaderOk<IOList<label>>(false);// object with local scope
    }

    if (!ok)
    {
        // Re-test with searched for objectPath. This is for backwards
        // compatibility
        fileName originalPath(filePath(io.objectPath()));
        if (originalPath != objPath)
        {
            // Test for either directory or a (valid) file & IOobject
            if (io.name().empty())
            {
                ok = isDir(originalPath);
            }
            else
            {
                ok =
                    isFile(originalPath)
                 && io.typeHeaderOk<IOList<label>>(false);
            }
        }
    }

    return ok;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

fileOperation::fileOperation
(
    const label comm,
    const bool distributedRoots
)
:
    comm_(comm),
    distributed_(distributedRoots)
{}


autoPtr<fileOperation>
fileOperation::New
(
    const word& handlerType,
    bool verbose
)
{
    DebugInFunction
        << "Constructing fileHandler" << endl;

    auto* ctorPtr = wordConstructorTable(handlerType);

    if (!ctorPtr)
    {
        FatalErrorInLookup
        (
            "fileHandler",
            handlerType,
            *wordConstructorTablePtr_
        ) << abort(FatalError);
    }

    return autoPtr<fileOperation>(ctorPtr(verbose));
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool fileOperation::distributed(bool on) const noexcept
{
    bool old(distributed_);
    distributed_ = on;
    return old;
}


fileName fileOperation::objectPath
(
    const IOobject& io,
    const word& typeName
) const
{
    return io.objectPath();
}


bool fileOperation::writeObject
(
    const regIOobject& io,
    IOstreamOption streamOpt,
    const bool valid
) const
{
    if (valid)
    {
        const fileName pathName(io.objectPath());

        mkDir(pathName.path());

        autoPtr<OSstream> osPtr(NewOFstream(pathName, streamOpt));

        if (!osPtr)
        {
            return false;
        }

        OSstream& os = *osPtr;

        // Update meta-data for current state
        const_cast<regIOobject&>(io).updateMetaData();

        // If any of these fail, return (leave error handling to Ostream class)

        const bool ok =
        (
            os.good()
         && io.writeHeader(os)
         && io.writeData(os)
        );

        if (ok)
        {
            IOobject::writeEndDivider(os);
        }

        return ok;
    }
    return true;
}


fileName fileOperation::filePath(const fileName& fName) const
{
    if (debug)
    {
        Pout<< "fileOperation::filePath :" << " fName:" << fName << endl;
    }

    fileName path, pDir, local;
    procRangeType group;
    label numProcs;
    label proci =
        splitProcessorPath(fName, path, pDir, local, group, numProcs);

    if (numProcs != -1)
    {
        WarningInFunction << "Filename is already adapted:" << fName << endl;
    }

    // Give preference to processors variant
    if (proci != -1)
    {
        // Get all processor directories
        refPtr<dirIndexList> procDirs(lookupProcessorsPath(fName));
        for (const dirIndex& dirIdx : procDirs())
        {
            const fileName& procDir = dirIdx.first();

            fileName collatedName(path/procDir/local);
            if (exists(collatedName))
            {
                if (debug)
                {
                    Pout<< "fileOperation::filePath : " << collatedName << endl;
                }
                return collatedName;
            }
        }
    }

    if (exists(fName))
    {
        if (debug)
        {
            Pout<< "fileOperation::filePath : " << fName << endl;
        }
        return fName;
    }

    if (debug)
    {
        Pout<< "fileOperation::filePath : Not found" << endl;
    }
    return fileName::null;
}


label fileOperation::addWatch(const fileName& fName) const
{
    return monitor().addWatch(fName);
}


bool fileOperation::removeWatch(const label watchIndex) const
{
    return monitor().removeWatch(watchIndex);
}


label fileOperation::findWatch
(
    const labelList& watchIndices,
    const fileName& fName
) const
{
    forAll(watchIndices, i)
    {
        if (getFile(watchIndices[i]) == fName)
        {
            return i;
        }
    }
    return -1;
}


void fileOperation::addWatches
(
    regIOobject& rio,
    const fileNameList& files
) const
{
    const labelList& watchIndices = rio.watchIndices();

    DynamicList<label> newWatchIndices;
    labelHashSet removedWatches(watchIndices);

    for (const fileName& f : files)
    {
        const label index = findWatch(watchIndices, f);

        if (index == -1)
        {
            newWatchIndices.append(addWatch(f));
        }
        else
        {
            // Existing watch
            newWatchIndices.append(watchIndices[index]);
            removedWatches.erase(index);
        }
    }

    // Remove any unused watches
    for (const label index : removedWatches)
    {
        removeWatch(watchIndices[index]);
    }

    rio.watchIndices() = newWatchIndices;
}


fileName fileOperation::getFile(const label watchIndex) const
{
    return monitor().getFile(watchIndex);
}


void fileOperation::updateStates
(
    const bool masterOnly,
    const bool syncPar
) const
{
    monitor().updateStates(masterOnly, Pstream::parRun());
}


fileMonitor::fileState fileOperation::getState
(
    const label watchFd
) const
{
    return monitor().getState(watchFd);
}


void fileOperation::setUnmodified(const label watchFd) const
{
    monitor().setUnmodified(watchFd);
}


instantList fileOperation::findTimes
(
    const fileName& directory,
    const word& constantName
) const
{
    if (debug)
    {
        Pout<< "fileOperation::findTimes : Finding times in directory "
            << directory << endl;
    }

    // Note: do NOT use master-only reading here (as per lookupProcessorsPath)
    // since this routine is called on an individual processorN directory

    // Read directory entries into a list
    fileNameList dirEntries(readDir(directory, fileName::DIRECTORY));
    instantList times = sortTimes(dirEntries, constantName);


    // Get all processor directories
    refPtr<dirIndexList> procDirs(lookupProcessorsPath(directory));
    for (const dirIndex& dirIdx : procDirs())
    {
        const fileName& procDir = dirIdx.first();
        fileName collDir(processorsPath(directory, procDir));
        if (!collDir.empty() && collDir != directory)
        {
            fileNameList extraEntries
            (
                readDir
                (
                    collDir,
                    fileName::DIRECTORY
                )
            );
            mergeTimes
            (
                sortTimes(extraEntries, constantName),
                constantName,
                times
            );
        }
    }

    if (debug)
    {
        Pout<< "fileOperation::findTimes : Found times:" << times << endl;
    }
    return times;
}


IOobject fileOperation::findInstance
(
    const IOobject& startIO,
    const scalar startValue,
    const word& stopInstance
) const
{
    const Time& time = startIO.time();

    IOobject io(startIO);

    // Note: - if name is empty, just check the directory itself
    //       - check both for isFile and headerOk since the latter does a
    //         filePath so searches for the file.
    //       - check for an object with local file scope (so no looking up in
    //         parent directory in case of parallel)

    if (exists(io))
    {
        DebugInFunction
            << "Found exact match for \"" << io.name()
            << "\" in " << io.instance()/io.local()
            << endl;

        return io;
    }

    // Search back through the time directories to find the first time
    // that is less than or equal to the current time

    instantList ts = time.times();
    label instanceI = ts.size()-1;

    for (; instanceI >= 0; --instanceI)
    {
        if (ts[instanceI].value() <= startValue)
        {
            break;
        }
    }

    // Found the time, continue from here
    for (; instanceI >= 0; --instanceI)
    {
        io.instance() = ts[instanceI].name();

        // Shortcut: if actual directory is the timeName we've already tested it
        if
        (
            io.instance() == startIO.instance()
         && io.instance() != stopInstance
        )
        {
            continue;
        }

        if (exists(io))
        {
            DebugInFunction
                << "Found exact match for \"" << io.name()
                << "\" in " << io.instance()/io.local()
                << endl;

            return io;
        }

        // Check if hit minimum instance
        if (io.instance() == stopInstance)
        {
            DebugInFunction
                << "Hit stopInstance " << stopInstance << endl;

            if
            (
                startIO.readOpt() == IOobject::MUST_READ
             || startIO.readOpt() == IOobject::MUST_READ_IF_MODIFIED
            )
            {
                if (io.name().empty())
                {
                    FatalErrorInFunction
                        << "Cannot find directory "
                        << io.local() << " in times " << startIO.instance()
                        << " down to " << stopInstance
                        << exit(FatalError);
                }
                else
                {
                    FatalErrorInFunction
                        << "Cannot find file \"" << io.name()
                        << "\" in directory " << io.local()
                        << " in times " << startIO.instance()
                        << " down to " << stopInstance
                        << exit(FatalError);
                }
            }

            return io;
        }
    }

    // Times usually already includes 'constant' so would have been checked
    // above.
    // However, re-test under these conditions:
    // - Times is empty.
    //   Sometimes this can happen (eg, decomposePar with collated)
    // - Times[0] is not constant
    // - The startValue is negative (eg, kivaTest).
    //   This plays havoc with the reverse search, causing it to miss 'constant'

    if
    (
        ts.empty()
     || ts.first().name() != time.constant()
     || startValue < 0
    )
    {
        io.instance() = time.constant();
        if (exists(io))
        {
            DebugInFunction
                << "Found constant match for \"" << io.name()
                << "\" in " << io.instance()/io.local()
                << endl;

            return io;
        }
    }


    if
    (
        startIO.readOpt() == IOobject::MUST_READ
     || startIO.readOpt() == IOobject::MUST_READ_IF_MODIFIED
    )
    {
        FatalErrorInFunction
            << "Cannot find file \"" << io.name() << "\" in directory "
            << io.local() << " in times " << startIO.instance()
            << " down to " << time.constant()
            << exit(FatalError);
    }

    return io;
}


fileNameList fileOperation::readObjects
(
    const objectRegistry& db,
    const fileName& instance,
    const fileName& local,
    word& newInstance
) const
{
    if (debug)
    {
        Pout<< "fileOperation::readObjects :"
            << " db:" << db.objectPath()
            << " instance:" << instance << endl;
    }

    fileName path(db.path(instance, db.dbDir()/local));

    newInstance = word::null;
    fileNameList objectNames;

    if (isDir(path))
    {
        newInstance = instance;
        objectNames = readDir(path, fileName::FILE);
    }
    else
    {
        // Get processors equivalent of path
        fileName procsPath(filePath(path));

        if (!procsPath.empty())
        {
            newInstance = instance;
            objectNames = readDir(procsPath, fileName::FILE);
        }
    }
    return objectNames;
}


void fileOperation::setNProcs(const label nProcs)
{}


label fileOperation::nProcs
(
    const fileName& dir,
    const fileName& local
) const
{
    label nProcs = 0;
    if (Pstream::master(comm_))
    {
        fileNameList dirNames(readDir(dir, fileName::Type::DIRECTORY));

        // Detect any processorsDDD or processorDDD
        label maxProc = -1;
        for (const fileName& dirN : dirNames)
        {
            fileName rp, rd, rl;
            procRangeType group;
            label rNum;

            const label readProci =
                splitProcessorPath(dirN, rp, rd, rl, group, rNum);

            maxProc = max(maxProc, readProci);
            if (rNum != -1)
            {
                // Direct detection of processorsDDD
                maxProc = rNum-1;
                break;
            }
        }
        nProcs = maxProc+1;

        if (nProcs == 0 && isDir(dir/processorsBaseDir))
        {
            WarningInFunction
                << "Defunct collated naming: " << processorsBaseDir << nl
                << "Manually rename with the decomposition number. Eg," << nl << nl
                << "    mv processors processors16" << nl << nl
                << "...returning 1" << endl;

            nProcs = 1;
        }
    }
    Pstream::scatter(nProcs, Pstream::msgType(), comm_);
    return nProcs;
}


void fileOperation::flush() const
{
    if (debug)
    {
        Pout<< "fileOperation::flush : clearing processor directories cache"
            << endl;
    }
    procsDirs_.clear();
}


fileName fileOperation::processorsCasePath
(
    const IOobject& io,
    const word& procsDir
) const
{
    return io.rootPath()/io.time().globalCaseName()/procsDir;
}


fileName fileOperation::processorsPath
(
    const IOobject& io,
    const word& instance,
    const word& procsDir
) const
{
    return
        processorsCasePath(io, procsDir)
       /instance
       /io.db().dbDir()
       /io.local();
}


fileName fileOperation::processorsPath
(
    const fileName& dir,
    const word& procsDir
) const
{
    // Check if directory is processorDDD

    const word caseName(dir.name());
    if (caseName.starts_with("processor"))
    {
        // Reject both '^processor$' and '^processors.*$'

        if (!std::isdigit(caseName[9]))
        {
            WarningInFunction << "Directory " << dir
                << " does not end in old-style processorDDD" << endl;
        }

        return dir.path()/procsDir;
    }

    return fileName::null;
}


label fileOperation::splitProcessorPath
(
    const fileName& objPath,
    fileName& path,
    fileName& procDir,
    fileName& local,

    procRangeType& group,
    label& nProcs
)
{
    // Return value
    label returnProci = -1;

    // Clear out the return parameters

    path.clear();
    procDir.clear();
    local.clear();
    group.clear();

    // Invalidate detected number of processors
    nProcs = -1;

    // The local processor group is read as first/last, but stored as
    // start/size.  Empty with start=0, size=0 if no range is detected


    // Start of 'processor..' directory name (the procDir)
    size_t pos = 0;

    // The slash starting the trailing (local) directory
    size_t slashLocal = string::npos;


    // Search for processor at start of string or after /processor
    //
    // 'processor(\d+)'
    // 'processors(\d+)'
    // 'processors(\d+)_(\d+)-(\d+)'

    for
    (
        /*nil*/;
        (pos = objPath.find("processor", pos)) != string::npos;
        pos += 9
    )
    {
        if (pos > 0 && objPath[pos-1] != '/')
        {
            // Not start of string or after /processor
            continue;
        }

        // The parse point. One past 'processor'
        size_t firstp = pos + 9;

        // normal: 'processor(\d+)'
        // plural: 'processors(\d+)'

        const bool plural = (objPath[firstp] == 's');

        if (plural)
        {
            ++firstp;  // Skip over the 's'
        }
        else if (!std::isdigit(objPath[firstp]))
        {
            // Non-plural version (uncollated) requires digits only
            continue;
        }

        // The next slash indicates there is a local directory
        slashLocal = objPath.find('/', firstp);

        // The last parse point is the slash, or end of string
        const size_t lastp =
            (slashLocal == string::npos ? objPath.length() : slashLocal);

        if (!std::isdigit(objPath[lastp-1]))
        {
            // Must end in a digit!
            // This traps entries that are too short or look quite wrong
            // and avoid a string to int conversion that will fail anyhow
            continue;
        }


        // Match: '^processors(\d+)$'  -> nProcs

        // Match: '^processors(\d+)_(\d+)-(\d+)$'
        // \1 = nProcs
        // \2 = beg processor group
        // \3 = end processor group (inclusive)

        if (plural)
        {
            int nProcsRead = 0;

            if
            (
                parseProcsNumRange
                (
                    objPath.substr(firstp, lastp-firstp),
                    nProcsRead,
                    group
                )
            )
            {
                // Total number of processors
                nProcs = nProcsRead;

                // We are done!
                break;
            }
        }

        // Single
        // Match: '^processor(\d+)$'   -> proci

        label proci = 0;
        if
        (
            ::Foam::read(objPath.substr(firstp, lastp-firstp), proci)
         && (proci >= 0)
        )
        {
            // Capture value of an individual processor
            returnProci = proci;

            // We are done!
            break;
        }
    }

    if (pos != string::npos)
    {
        // The split succeeded, extract the components.

        // The leading directory
        if (pos > 0)
        {
            path = objPath.substr(0, pos-1);
        }

        // The slash starting the trailing (local) directory
        if (slashLocal != string::npos)
        {
            procDir = objPath.substr(pos, slashLocal-pos);
            local = objPath.substr(slashLocal+1);
        }
        else
        {
            procDir = objPath.substr(pos);
        }
    }

    return returnProci;
}


label fileOperation::detectProcessorPath(const fileName& fName)
{
    fileName path, pDir, local;
    procRangeType group;
    label nProcs;
    return splitProcessorPath(fName, path, pDir, local, group, nProcs);
}


// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

autoPtr<fileOperation> fileOperation::NewUncollated()
{
    return autoPtr<fileOperation>
    (
        new fileOperations::uncollatedFileOperation(false)
    );
}


// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

const fileOperation& fileHandler()
{
    if (!fileOperation::fileHandlerPtr_)
    {
        word handler(getEnv("FOAM_FILEHANDLER"));

        if (handler.empty())
        {
            handler = fileOperation::defaultFileHandler;
        }

        fileOperation::fileHandlerPtr_ = fileOperation::New(handler, true);
    }

    return *fileOperation::fileHandlerPtr_;
}


autoPtr<fileOperation>
fileHandler(autoPtr<fileOperation>&& newHandler)
{
    if
    (
        newHandler
     && fileOperation::fileHandlerPtr_
     && newHandler->type() == fileOperation::fileHandlerPtr_->type()
    )
    {
        return nullptr;  // No change
    }

    autoPtr<fileOperation> old(std::move(fileOperation::fileHandlerPtr_));

    fileOperation::fileHandlerPtr_ = std::move(newHandler);

    return old;
}

}
// ************************************************************************* //
