﻿/*---------------------------------------------------------------------------*\
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

#include "Time1.H"
#include "PstreamReduceOps.H"
#include "argList.H"
#include "HashSet.H"
#include "profiling.H"
#include "IOdictionary.H"
#include "registerSwitch.H"
#include <sstream>

// * * * * * * * * * * * * * Static Member Data  * * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(Time, 0);
 

const Enum
<
    Time::stopAtControls
>
Time::stopAtControlNames
({
    { stopAtControls::saEndTime, "endTime" },
    { stopAtControls::saNoWriteNow, "noWriteNow" },
    { stopAtControls::saWriteNow, "writeNow" },
    { stopAtControls::saNextWrite, "nextWrite" },
    // Leave saUnknown untabulated - fallback to flag unknown settings
});


const Enum
<
    Time::writeControls
>
Time::writeControlNames
({
    { writeControls::wcNone, "none" },
    { writeControls::wcTimeStep, "timeStep" },
    { writeControls::wcRunTime, "runTime" },
    { writeControls::wcAdjustableRunTime, "adjustable" },
    { writeControls::wcAdjustableRunTime, "adjustableRunTime" },
    { writeControls::wcClockTime, "clockTime" },
    { writeControls::wcCpuTime, "cpuTime" },
    // Leave wcUnknown untabulated - fallback to flag unknown settings
});


Time::fmtflags Time::format_(Time::general);

int Time::precision_(6);

const int Time::maxPrecision_(3 - log10(SMALL));

word Time::controlDictName("controlDict");

int Time::printExecutionFormat_
(
    debug::infoSwitch("printExecutionFormat", 0)
);

registerInfoSwitch
(
    "printExecutionFormat",
    int,
    Time::printExecutionFormat_
);

// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

void Time::adjustDeltaT()
{
    bool adjustTime = false;
    scalar timeToNextWrite = VGREAT;

    if (writeControl_ == wcAdjustableRunTime)
    {
        adjustTime = true;
        timeToNextWrite = max
        (
            0.0,
            (writeTimeIndex_ + 1)*writeInterval_ - (value() - startTime_)
        );
    }

    if (adjustTime)
    {
        scalar nSteps = timeToNextWrite/deltaT_;

        // For tiny deltaT the label can overflow!
        if (nSteps < scalar(labelMax))
        {
            // nSteps can be < 1 so make sure at least 1
            label nStepsToNextWrite = max(1, round(nSteps));

            scalar newDeltaT = timeToNextWrite/nStepsToNextWrite;

            // Control the increase of the time step to within a factor of 2
            // and the decrease within a factor of 5.
            if (newDeltaT >= deltaT_)
            {
                deltaT_ = min(newDeltaT, 2.0*deltaT_);
            }
            else
            {
                deltaT_ = max(newDeltaT, 0.2*deltaT_);
            }
        }
    }

    functionObjects_.adjustTimeStep();
}


void Time::setControls()
{
    // default is to resume calculation from "latestTime"
    const word startFrom = controlDict_.getOrDefault<word>
    (
        "startFrom",
        "latestTime"
    );

    if (startFrom == "startTime")
    {
        controlDict_.readEntry("startTime", startTime_);
    }
    else
    {
        // Search directory for valid time directories
        instantList timeDirs = findTimes(path(), constant());

        const label nTimes = timeDirs.size();

        if (startFrom == "firstTime")
        {
            if (nTimes > 1 && timeDirs.first().name() == constant())
            {
                startTime_ = timeDirs[1].value();
            }
            else if (nTimes)
            {
                startTime_ = timeDirs.first().value();
            }
        }
        else if (startFrom == "latestTime")
        {
            if (nTimes)
            {
                startTime_ = timeDirs.last().value();
            }
        }
        else
        {
            FatalIOErrorInFunction(controlDict_)
                << "expected startTime, firstTime or latestTime"
                << " found '" << startFrom << "'"
                << exit(FatalIOError);
        }
    }

    setTime(startTime_, 0);

    readDict();
    deltaTSave_ = deltaT_;
    deltaT0_ = deltaT_;

    // Check if time directory exists
    // If not increase time precision to see if it is formatted differently.
    // Note: do not use raw fileHandler().exists(...) since does not check
    //       alternative processorsDDD directories naming
    if (fileHandler().filePath(timePath()).empty())
    {
        int oldPrecision = precision_;
        int requiredPrecision = -1;
        bool found = false;
        word oldTime(timeName());
        for
        (
            precision_ = maxPrecision_;
            precision_ > oldPrecision;
            --precision_
        )
        {
            // Update the time formatting
            setTime(startTime_, 0);

            word newTime(timeName());
            if (newTime == oldTime)
            {
                break;
            }
            oldTime = newTime;

            // Check the existence of the time directory with the new format
            //found = fileHandler().exists(timePath(), false);
            const fileName dirName(fileHandler().filePath(timePath()));
            found = !dirName.empty();

            if (found)
            {
                requiredPrecision = precision_;
            }
        }

        if (requiredPrecision > 0)
        {
            // Update the time precision
            precision_ = requiredPrecision;

            // Update the time formatting
            setTime(startTime_, 0);

            WarningInFunction
                << "Increasing the timePrecision from " << oldPrecision
                << " to " << precision_
                << " to support the formatting of the current time directory "
                << timeName() << nl << endl;
        }
        else
        {
            // Could not find time directory so assume it is not present
            precision_ = oldPrecision;

            // Revert the time formatting
            setTime(startTime_, 0);
        }
    }

    if (Pstream::parRun())
    {
        scalar sumStartTime = startTime_;
        reduce(sumStartTime, sumOp<scalar>());
        if
        (
            mag(Pstream::nProcs()*startTime_ - sumStartTime)
          > Pstream::nProcs()*deltaT_/10.0
        )
        {
            FatalIOErrorInFunction(controlDict_)
                << "Start time is not the same for all processors" << nl
                << "processor " << Pstream::myProcNo() << " has startTime "
                << startTime_ << exit(FatalIOError);
        }
    }

    IOdictionary timeDict
    (
        IOobject
        (
            "time",
            timeName(),
            "uniform",
            *this,
            IOobject::READ_IF_PRESENT,
            IOobject::NO_WRITE,
            false
        )
    );

    // Read and set the deltaT only if time-step adjustment is active
    // otherwise use the deltaT from the controlDict
    if (controlDict_.getOrDefault("adjustTimeStep", false))
    {
        if (timeDict.readIfPresent("deltaT", deltaT_))
        {
            deltaTSave_ = deltaT_;
            deltaT0_ = deltaT_;
        }
    }

    timeDict.readIfPresent("deltaT0", deltaT0_);

    if (timeDict.readIfPresent("index", startTimeIndex_))
    {
        timeIndex_ = startTimeIndex_;
    }


    // Check if values stored in time dictionary are consistent

    // 1. Based on time name
    bool checkValue = true;

    string storedTimeName;
    if (timeDict.readIfPresent("name", storedTimeName))
    {
        if (storedTimeName == timeName())
        {
            // Same time. No need to check stored value
            checkValue = false;
        }
    }

    // 2. Based on time value
    //    (consistent up to the current time writing precision so it won't
    //     trigger if we just change the write precision)
    if (checkValue)
    {
        scalar storedTimeValue;
        if (timeDict.readIfPresent("value", storedTimeValue))
        {
            word storedTimeName(timeName(storedTimeValue));

            if (storedTimeName != timeName())
            {
                IOWarningInFunction(timeDict)
                    << "Time read from time dictionary " << storedTimeName
                    << " differs from actual time " << timeName() << '.' << nl
                    << "    This may cause unexpected database behaviour."
                    << " If you are not interested" << nl
                    << "    in preserving time state delete"
                    << " the time dictionary."
                    << endl;
            }
        }
    }
}


void Time::setMonitoring(const bool forceProfiling)
{
    const dictionary* profilingDict = controlDict_.findDict("profiling");
    if (!profilingDict)
    {
        // ... or from etc/controlDict
        profilingDict = debug::controlDict().findDict("profiling");
    }

    // initialize profiling on request
    // otherwise rely on profiling entry within controlDict
    // and skip if 'active' keyword is explicitly set to false
    if (forceProfiling)
    {
        profiling::initialize
        (
            IOobject
            (
                "profiling",
                timeName(),
                "uniform",
                *this,
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
            ),
            *this
        );
    }
    else if
    (
        profilingDict
     && profilingDict->getOrDefault("active", true)
    )
    {
        profiling::initialize
        (
            *profilingDict,
            IOobject
            (
                "profiling",
                timeName(),
                "uniform",
                *this,
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
            ),
            *this
        );
    }

    // Time objects not registered so do like objectRegistry::checkIn ourselves.
    if (runTimeModifiable_)
    {
        // Monitor all files that controlDict depends on
        fileHandler().addWatches(controlDict_, controlDict_.files());
    }

    // Clear dependent files - not needed now
    controlDict_.files().clear();
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Time::Time
(
    const word& ctrlDictName,
    const fileName& rootPath,
    const fileName& caseName,
    const word& systemName,
    const word& constantName,
    const bool enableFunctionObjects,
    const bool enableLibs
)
:
    TimePaths
    (
        rootPath,
        caseName,
        systemName,
        constantName
    ),

    objectRegistry(*this),

    loopProfiling_(nullptr),
    libs_(),

    controlDict_
    (
        IOobject
        (
            ctrlDictName,
            system(),
            *this,
            IOobject::MUST_READ_IF_MODIFIED,
            IOobject::NO_WRITE,
            false
        )
    ),

    startTimeIndex_(0),
    startTime_(0),
    endTime_(0),

    stopAt_(saEndTime),
    writeControl_(wcTimeStep),
    writeInterval_(GREAT),
    purgeWrite_(0),
    subCycling_(0),
    writeOnce_(false),
    sigWriteNow_(*this, true),
    sigStopAtWriteNow_(*this, true),
    writeStreamOption_(IOstream::ASCII),
    graphFormat_("raw"),
    runTimeModifiable_(false),
    functionObjects_(*this, false)
{
    if (enableFunctionObjects)
    {
        functionObjects_.on();
    }

    if (enableLibs)
    {
        libs_.open("libs", controlDict_);
    }

    // Explicitly set read flags on objectRegistry so anything constructed
    // from it reads as well (e.g. fvSolution).
    readOpt(IOobject::MUST_READ_IF_MODIFIED);

    setControls();
    setMonitoring();
}


Time::Time
(
    const word& ctrlDictName,
    const argList& args,
    const word& systemName,
    const word& constantName,
    const bool enableFunctionObjects,
    const bool enableLibs
)
:
    TimePaths(args, systemName, constantName),

    objectRegistry(*this),

    loopProfiling_(nullptr),
    libs_(),

    controlDict_
    (
        IOobject
        (
            ctrlDictName,
            system(),
            *this,
            IOobject::MUST_READ_IF_MODIFIED,
            IOobject::NO_WRITE,
            false
        )
    ),

    startTimeIndex_(0),
    startTime_(0),
    endTime_(0),

    stopAt_(saEndTime),
    writeControl_(wcTimeStep),
    writeInterval_(GREAT),
    purgeWrite_(0),
    subCycling_(0),
    writeOnce_(false),
    sigWriteNow_(*this, true),
    sigStopAtWriteNow_(*this, true),
    writeStreamOption_(IOstream::ASCII),
    graphFormat_("raw"),
    runTimeModifiable_(false),
    functionObjects_(*this, false)
{
    // Functions
    //
    // * '-withFunctionObjects' exists and used = enable
    // * '-noFunctionObjects' exists and used = disable
    // * default: no functions if there is no way to enable/disable them
    if
    (
        argList::validOptions.found("withFunctionObjects")
      ? args.found("withFunctionObjects")
      : argList::validOptions.found("noFunctionObjects")
      ? !args.found("noFunctionObjects")
      : false
    )
    {
        if (enableFunctionObjects)
        {
            functionObjects_.on();
        }
    }

    // Libraries
    //
    // * enable by default unless '-no-libs' option was used
    if (enableLibs && !args.found("no-libs"))
    {
        libs_.open("libs", controlDict_);
    }

    // Explicitly set read flags on objectRegistry so anything constructed
    // from it reads as well (e.g. fvSolution).
    readOpt(IOobject::MUST_READ_IF_MODIFIED);

    setControls();

    // '-profiling' = force profiling, ignore controlDict entry
    setMonitoring(args.found("profiling"));
}


Time::Time
(
    const dictionary& dict,
    const fileName& rootPath,
    const fileName& caseName,
    const word& systemName,
    const word& constantName,
    const bool enableFunctionObjects,
    const bool enableLibs
)
:
    TimePaths
    (
        rootPath,
        caseName,
        systemName,
        constantName
    ),

    objectRegistry(*this),

    loopProfiling_(nullptr),
    libs_(),

    controlDict_
    (
        IOobject
        (
            controlDictName,
            system(),
            *this,
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false
        ),
        dict
    ),

    startTimeIndex_(0),
    startTime_(0),
    endTime_(0),

    stopAt_(saEndTime),
    writeControl_(wcTimeStep),
    writeInterval_(GREAT),
    purgeWrite_(0),
    subCycling_(0),
    writeOnce_(false),
    sigWriteNow_(*this, true),
    sigStopAtWriteNow_(*this, true),
    writeStreamOption_(IOstream::ASCII),
    graphFormat_("raw"),
    runTimeModifiable_(false),
    functionObjects_(*this, false)
{
    if (enableFunctionObjects)
    {
        functionObjects_.on();
    }

    if (enableLibs)
    {
        libs_.open("libs", controlDict_);
    }


    // Explicitly set read flags on objectRegistry so anything constructed
    // from it reads as well (e.g. fvSolution).
    readOpt(IOobject::MUST_READ_IF_MODIFIED);

    // Since could not construct regIOobject with setting:
    controlDict_.readOpt(IOobject::MUST_READ_IF_MODIFIED);

    setControls();
    setMonitoring();
}


Time::Time
(
    const fileName& rootPath,
    const fileName& caseName,
    const word& systemName,
    const word& constantName,
    const bool enableFunctionObjects,
    const bool enableLibs
)
:
    TimePaths
    (
        rootPath,
        caseName,
        systemName,
        constantName
    ),

    objectRegistry(*this),

    loopProfiling_(nullptr),
    libs_(),

    controlDict_
    (
        IOobject
        (
            controlDictName,
            system(),
            *this,
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false
        )
    ),

    startTimeIndex_(0),
    startTime_(0),
    endTime_(0),

    stopAt_(saEndTime),
    writeControl_(wcTimeStep),
    writeInterval_(GREAT),
    purgeWrite_(0),
    subCycling_(0),
    writeOnce_(false),
    writeStreamOption_(IOstream::ASCII),
    graphFormat_("raw"),
    runTimeModifiable_(false),
    functionObjects_(*this, false)
{
    if (enableFunctionObjects)
    {
        functionObjects_.on();
    }

    if (enableLibs)
    {
        libs_.open("libs", controlDict_);
    }

    setMonitoring(); // for profiling etc
}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

autoPtr<Time> Time::New()
{
    return
        autoPtr<Time>::New
        (
            fileName("."),  // root-path
            fileName("."),  // case-name
            false,          // No enableFunctionObjects
            false           // No enableLibs
        );
}


autoPtr<Time> Time::New(const fileName& caseDir)
{
    return
        autoPtr<Time>::New
        (
            caseDir.path(), // root-path
            caseDir.name(), // case-name
            false,          // No enableFunctionObjects
            false           // No enableLibs
        );
}


autoPtr<Time> Time::New(const argList& args)
{
    return
        autoPtr<Time>::New
        (
            Time::controlDictName,
            args,
            false,          // No enableFunctionObjects
            false           // No enableLibs
        );
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Time::~Time()
{
    loopProfiling_.reset(nullptr);

    forAllReverse(controlDict_.watchIndices(), i)
    {
        fileHandler().removeWatch(controlDict_.watchIndices()[i]);
    }

    // Destroy function objects first
    functionObjects_.clear();

    // Clean up profiling
    profiling::stop(*this);

    // Ensure all owned objects are also cleaned up now
    objectRegistry::clear();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

word Time::timeName(const scalar t, const int precision)
{
    std::ostringstream buf;
    buf.setf(ios_base::fmtflags(format_), ios_base::floatfield);
    buf.precision(precision);
    buf << t;
    return buf.str();
}


word Time::timeName() const
{
    return dimensionedScalar::name();
}


word Time::findInstance
(
    const fileName& dir,
    const word& name,
    const IOobject::readOption rOpt,
    const word& stopInstance
) const
{
    IOobject startIO
    (
        name,           // name might be empty!
        timeName(),
        dir,
        *this,
        rOpt
    );

    IOobject io
    (
        fileHandler().findInstance
        (
            startIO,
            timeOutputValue(),
            stopInstance
        )
    );
    return io.instance();
}


word Time::findInstancePath
(
    const fileName& directory,
    const instant& t
) const
{
    // Simplified version: use findTimes (readDir + sort). The expensive
    // bit is the readDir, not the sorting. Tbd: avoid calling findInstancePath
    // from filePath.

    instantList timeDirs = findTimes(path(), constant());
    // Note:
    // - times will include constant (with value 0) as first element.
    //   For backwards compatibility make sure to find 0 in preference
    //   to constant.
    // - list is sorted so could use binary search

    forAllReverse(timeDirs, i)
    {
        if (t.equal(timeDirs[i].value()))
        {
            return timeDirs[i].name();
        }
    }

    return word::null;
}


word Time::findInstancePath(const instant& t) const
{
    return findInstancePath(path(), t);
}


label Time::startTimeIndex() const
{
    return startTimeIndex_;
}


dimensionedScalar Time::startTime() const
{
    return dimensionedScalar("startTime", dimTime, startTime_);
}


dimensionedScalar Time::endTime() const
{
    return dimensionedScalar("endTime", dimTime, endTime_);
}


Time::stopAtControls Time::stopAt() const
{
    return stopAt_;
}


bool Time::run() const
{
    loopProfiling_.reset(nullptr);

    bool isRunning = value() < (endTime_ - 0.5*deltaT_);

    if (!subCycling_)
    {
        // Only execute when the condition is no longer true
        // ie, when exiting the control loop
        if (!isRunning && timeIndex_ != startTimeIndex_)
        {
            // Ensure functionObjects execute on last time step
            // (and hence write uptodate functionObjectProperties)
            {
                addProfiling(fo, "functionObjects.execute()");
                functionObjects_.execute();
            }
            {
                addProfiling(fo, "functionObjects.end()");
                functionObjects_.end();
            }
        }
    }

    if (isRunning)
    {
        if (!subCycling_)
        {
            const_cast<Time&>(*this).readModifiedObjects();

            if (timeIndex_ == startTimeIndex_)
            {
                addProfiling(functionObjects, "functionObjects.start()");
                functionObjects_.start();
            }
            else
            {
                addProfiling(functionObjects, "functionObjects.execute()");
                functionObjects_.execute();
            }

            // Check if the execution of functionObjects require re-reading
            // any files. This moves effect of e.g. 'timeActivatedFileUpdate'
            // one time step forward. Note that we cannot call
            // readModifiedObjects from within timeActivatedFileUpdate since
            // it might re-read the functionObjects themselves (and delete
            // the timeActivatedFileUpdate one)
            if (functionObjects_.filesModified())
            {
                const_cast<Time&>(*this).readModifiedObjects();
            }
        }

        // Update the "is-running" status following the
        // possible side-effects from functionObjects
        isRunning = value() < (endTime_ - 0.5*deltaT_);

        // (re)trigger profiling
        if (profiling::active())
        {
            loopProfiling_.reset
            (
                new profilingTrigger("time.run() " + objectRegistry::name())
            );
        }
    }

    return isRunning;
}


bool Time::loop()
{
    const bool isRunning = run();

    if (isRunning)
    {
        operator++();
    }

    return isRunning;
}


bool Time::end() const
{
    return value() > (endTime_ + 0.5*deltaT_);
}


bool Time::stopAt(const stopAtControls stopCtrl) const
{
    if (stopCtrl == stopAtControls::saUnknown)
    {
        return false;
    }

    const bool changed = (stopAt_ != stopCtrl);
    stopAt_ = stopCtrl;
    endTime_ = GREAT;

    // Adjust endTime
    if (stopCtrl == stopAtControls::saEndTime)
    {
        controlDict_.readEntry("endTime", endTime_);
    }

    return changed;
}


bool Time::isAdjustTimeStep() const
{
    return controlDict_.getOrDefault("adjustTimeStep", false);
}


void Time::setTime(const Time& t)
{
    value() = t.value();
    dimensionedScalar::name() = t.dimensionedScalar::name();
    timeIndex_ = t.timeIndex_;
    fileHandler().setTime(*this);
}


void Time::setTime(const instant& inst, const label newIndex)
{
    value() = inst.value();
    dimensionedScalar::name() = inst.name();
    timeIndex_ = newIndex;

    IOdictionary timeDict
    (
        IOobject
        (
            "time",
            timeName(),
            "uniform",
            *this,
            IOobject::READ_IF_PRESENT,
            IOobject::NO_WRITE,
            false
        )
    );

    timeDict.readIfPresent("deltaT", deltaT_);
    timeDict.readIfPresent("deltaT0", deltaT0_);
    timeDict.readIfPresent("index", timeIndex_);
    fileHandler().setTime(*this);
}


void Time::setTime(const dimensionedScalar& newTime, const label newIndex)
{
    setTime(newTime.value(), newIndex);
}


void Time::setTime(const scalar newTime, const label newIndex)
{
    value() = newTime;
    dimensionedScalar::name() = timeName(timeToUserTime(newTime));
    timeIndex_ = newIndex;
    fileHandler().setTime(*this);
}


void Time::setEndTime(const dimensionedScalar& endTime)
{
    setEndTime(endTime.value());
}


void Time::setEndTime(const scalar endTime)
{
    endTime_ = endTime;
}


void Time::setDeltaT
(
    const dimensionedScalar& deltaT,
    const bool adjust
)
{
    setDeltaT(deltaT.value(), adjust);
}


void Time::setDeltaT(const scalar deltaT, const bool adjust)
{
    deltaT_ = deltaT;
    deltaTchanged_ = true;

    if (adjust)
    {
        adjustDeltaT();
    }
}


TimeState Time::subCycle(const label nSubCycles)
{
    #ifdef FULLDEBUG
    if (prevTimeState_)
    {
        FatalErrorInFunction
            << "previous time state already set" << nl
            << exit(FatalError);
    }
    #endif

    prevTimeState_.reset(new TimeState(*this));

    setTime(*this - deltaT(), (timeIndex() - 1)*nSubCycles);
    deltaT_ /= nSubCycles;
    deltaT0_ /= nSubCycles;
    deltaTSave_ = deltaT0_;

    subCycling_ = nSubCycles;

    return prevTimeState();
}


void Time::subCycleIndex(const label index)
{
    // Only permit adjustment if sub-cycling was already active
    // and if the index is valid (positive, non-zero).
    // This avoids potential mixups for deleting.

    if (subCycling_ && index > 0)
    {
        subCycling_ = index;
    }
}


void Time::endSubCycle()
{
    if (subCycling_)
    {
        TimeState::operator=(prevTimeState());
        prevTimeState_.reset(nullptr);
    }

    subCycling_ = 0;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

Time& Time::operator+=(const dimensionedScalar& deltaT)
{
    return operator+=(deltaT.value());
}


Time& Time::operator+=(const scalar deltaT)
{
    setDeltaT(deltaT);
    return operator++();
}


Time& Time::operator++()
{
    deltaT0_ = deltaTSave_;
    deltaTSave_ = deltaT_;

    // Save old time value and name
    const scalar oldTimeValue = timeToUserTime(value());
    const word oldTimeName = dimensionedScalar::name();

    // Increment time
    setTime(value() + deltaT_, timeIndex_ + 1);

    if (!subCycling_)
    {
        // If the time is very close to zero reset to zero
        if (mag(value()) < 10*SMALL*deltaT_)
        {
            setTime(0.0, timeIndex_);
        }

        if (sigStopAtWriteNow_.active() || sigWriteNow_.active())
        {
            // A signal might have been sent on one processor only
            // Reduce so all decide the same.

            label flag = 0;
            if (sigStopAtWriteNow_.active() && stopAt_ == saWriteNow)
            {
                flag += 1;
            }
            if (sigWriteNow_.active() && writeOnce_)
            {
                flag += 2;
            }
            reduce(flag, maxOp<label>());

            if (flag & 1)
            {
                stopAt_ = saWriteNow;
            }
            if (flag & 2)
            {
                writeOnce_ = true;
            }
        }

        writeTime_ = false;

        switch (writeControl_)
        {
            case wcNone:
            case wcUnknown:
            break;

            case wcTimeStep:
                writeTime_ = !(timeIndex_ % label(writeInterval_));
            break;

            case wcRunTime:
            case wcAdjustableRunTime:
            {
                const label writeIndex = label
                (
                    ((value() - startTime_) + 0.5*deltaT_)
                  / writeInterval_
                );

                if (writeIndex > writeTimeIndex_)
                {
                    writeTime_ = true;
                    writeTimeIndex_ = writeIndex;
                }
            }
            break;

            case wcCpuTime:
            {
                const label writeIndex = label
                (
                    returnReduce(elapsedCpuTime(), maxOp<double>())
                  / writeInterval_
                );
                if (writeIndex > writeTimeIndex_)
                {
                    writeTime_ = true;
                    writeTimeIndex_ = writeIndex;
                }
            }
            break;

            case wcClockTime:
            {
                const label writeIndex = label
                (
                    returnReduce(elapsedClockTime(), maxOp<double>())
                  / writeInterval_
                );
                if (writeIndex > writeTimeIndex_)
                {
                    writeTime_ = true;
                    writeTimeIndex_ = writeIndex;
                }
            }
            break;
        }


        // Check if endTime needs adjustment to stop at the next run()/end()
        if (!end())
        {
            if (stopAt_ == saNoWriteNow)
            {
                endTime_ = value();
            }
            else if (stopAt_ == saWriteNow)
            {
                endTime_ = value();
                writeTime_ = true;
            }
            else if (stopAt_ == saNextWrite && writeTime_ == true)
            {
                endTime_ = value();
            }
        }

        // Override writeTime if one-shot writing
        if (writeOnce_)
        {
            writeTime_ = true;
            writeOnce_ = false;
        }

        // Adjust the precision of the time directory name if necessary
        if (writeTime_)
        {
            // Tolerance used when testing time equivalence
            const scalar timeTol =
                max(min(pow(10.0, -precision_), 0.1*deltaT_), SMALL);

            // User-time equivalent of deltaT
            const scalar userDeltaT = timeToUserTime(deltaT_);

            // Time value obtained by reading timeName
            scalar timeNameValue = -VGREAT;

            // Check that new time representation differs from old one
            // reinterpretation of the word
            if
            (
                readScalar(dimensionedScalar::name(), timeNameValue)
             && (mag(timeNameValue - oldTimeValue - userDeltaT) > timeTol)
            )
            {
                int oldPrecision = precision_;
                while
                (
                    precision_ < maxPrecision_
                 && readScalar(dimensionedScalar::name(), timeNameValue)
                 && (mag(timeNameValue - oldTimeValue - userDeltaT) > timeTol)
                )
                {
                    precision_++;
                    setTime(value(), timeIndex());
                }

                if (precision_ != oldPrecision)
                {
                    WarningInFunction
                        << "Increased the timePrecision from " << oldPrecision
                        << " to " << precision_
                        << " to distinguish between timeNames at time "
                        << dimensionedScalar::name()
                        << endl;

                    if (precision_ == maxPrecision_)
                    {
                        // Reached maxPrecision limit
                        WarningInFunction
                            << "Current time name " << dimensionedScalar::name()
                            << nl
                            << "    The maximum time precision has been reached"
                               " which might result in overwriting previous"
                               " results."
                            << endl;
                    }

                    // Check if round-off error caused time-reversal
                    scalar oldTimeNameValue = -VGREAT;
                    if
                    (
                        readScalar(oldTimeName, oldTimeNameValue)
                     && (
                            sign(timeNameValue - oldTimeNameValue)
                         != sign(deltaT_)
                        )
                    )
                    {
                        WarningInFunction
                            << "Current time name " << dimensionedScalar::name()
                            << " is set to an instance prior to the "
                               "previous one "
                            << oldTimeName << nl
                            << "    This might result in temporal "
                               "discontinuities."
                            << endl;
                    }
                }
            }
        }
    }

    return *this;
}


Time& Time::operator++(int)
{
    return operator++();
}

}
// ************************************************************************* //
