/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2017-2021 OpenCFD Ltd.
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

#include "JobInfo.H"
#include "clock.H"
#include "OFstream.H"
#include "OSspecific.H"
#include "Pstream.H"
#include "foamVersion.H"

// Fallback for job-control directory is in the user-directory
// ~/.OpenFOAM/jobControl

#ifndef FOAM_RESOURCE_USER_CONFIG_DIRNAME
#define FOAM_RESOURCE_USER_CONFIG_DIRNAME ".OpenFOAM"
#ifdef FULLDEBUG
    #warning FOAM_RESOURCE_USER_CONFIG_DIRNAME was undefined (now ".OpenFOAM")
#endif
#endif

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


 namespace Foam{
bool JobInfo::writeJobInfo(debug::infoSwitch("writeJobInfo", 0));
JobInfo jobInfo;

// Foam::JobInfo::constructed  defined in globals.C


// * * * * * * * * * * * * * * * Local Functions * * * * * * * * * * * * * * //


 } // End namespace Foam
namespace Foam
{

// Ensure given directory exists (called on master only)
static inline bool ensureJobDirExists(const fileName& dir)
{
    if (!isDir(dir) && !mkDir(dir))
    {
        std::cerr
            << "WARNING: no JobInfo directory: " << dir << nl
            << "    disabling JobInfo" << nl;

        return false;
    }

    return true;
}


// Write dictionary entries (called on master only)
static inline bool writeJobDict(Ostream& os, const dictionary& dict)
{
    if (os.good())
    {
        dict.writeEntries(os, true);  // With extraNewLine=true
        return true;
    }

    return false;
}

} // End namespace Foam


// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //


 namespace Foam{
void JobInfo::disable() noexcept
{
    writeJobInfo = false;
}


void JobInfo::shutdown()
{
    jobInfo.jobEnding();
}


void JobInfo::shutdown(bool isAbort)
{
    if (isAbort)
    {
        jobInfo.jobEnding("abort");
    }
    else
    {
        jobInfo.jobEnding("exit");
    }
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void JobInfo::jobEnding()
{
    if (!running_.empty())
    {
        if (!mv(running_, finished_))
        {
            rm(running_);
        }
    }

    running_.clear();
    finished_.clear();
    constructed = false;
}


void JobInfo::jobEnding(const word& terminationType)
{
    if (writeJobInfo && !finished_.empty())
    {
        add("cpuTime", cpuTime_.elapsedCpuTime());
        add("endDate", clock::date());
        add("endTime", clock::clockTime());

        if (!terminationType.empty() && !found("termination"))
        {
            add("termination", terminationType);
        }

        rm(running_);
        OFstream os(finished_);
        if (!writeJobDict(os, *this))
        {
            std::cerr
                << "WARNING: could not write JobInfo file: "
                << finished_ << nl;
        }
    }

    running_.clear();
    finished_.clear();
    constructed = false;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

JobInfo::JobInfo()
{
    if (constructed)
    {
        std::cerr
            << "WARNING: JobInfo was already constructed. "
               "Should be a singleton!!" << nl;
    }

    // Only populate on master process, and when enabled
    if (writeJobInfo && Pstream::master())
    {
        string jobDir = getEnv("FOAM_JOB_DIR");
        if (jobDir.empty())
        {
            jobDir = home()/FOAM_RESOURCE_USER_CONFIG_DIRNAME/"jobControl";
        }
        string jobFile = hostName() + '.' + ::Foam::name(pid());
        running_  = jobDir/"runningJobs"/jobFile;
        finished_ = jobDir/"finishedJobs"/jobFile;

        if
        (
            !ensureJobDirExists(jobDir)
         || !ensureJobDirExists(running_.path())
         || !ensureJobDirExists(finished_.path())
        )
        {
            running_.clear();
            finished_.clear();
        }
    }

    dictionary::name() = "JobInfo";
    constructed = true;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

JobInfo::~JobInfo()
{
    jobEnding();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void JobInfo::write() const
{
    if (writeJobInfo && !running_.empty())
    {
        OFstream os(running_);
        if (!writeJobDict(os, *this))
        {
            std::cerr
                << "WARNING: could not write JobInfo file: "
                << running_ << nl;

            // Normally does not happen
            const_cast<fileName&>(running_).clear();
        }
    }
}


void JobInfo::stop() { jobEnding("normal"); }

void JobInfo::exit() { jobEnding("exit"); }

void JobInfo::abort() { jobEnding("abort"); }

void JobInfo::signalEnd() { jobEnding(); }


// ************************************************************************* //

 } // End namespace Foam
