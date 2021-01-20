/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2012 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of blueCAPE's unofficial mingw patches for OpenFOAM.
    For more information about these patches, visit:
         http://bluecfd.com/Core

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
    This file is based on the original version for POSIX:
        OpenFOAM/src/OSspecific/POSIX/

    This copy of this file has been created by blueCAPE's unofficial mingw
    patches for OpenFOAM.
    For more information about these patches, visit:
        http://bluecfd.com/Core

    Modifications made:
      - Used NULL instead of 0.

\*---------------------------------------------------------------------------*/

#include "clockTime.H"
#include "unistd.h"
#include <time.h>
#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define DELTA_EPOCH_IN_MICROSECS 11644473600000000Ui64
#else
#define DELTA_EPOCH_IN_MICROSECS 11644473600000000ULL
#endif
// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //
  int gettimeofday(struct timeval *tv, struct timezone *tz) 
  {
        FILETIME ft;
        uint64_t tmpres = 0;
        static int tzflag = 0;


        if (tv)
        {
#ifdef _WIN32_WCE
            SYSTEMTIME st;
            GetSystemTime(&st);
            SystemTimeToFileTime(&st, &ft);
#else
            GetSystemTimeAsFileTime(&ft);
#endif


            tmpres |= ft.dwHighDateTime;
            tmpres <<= 32;
            tmpres |= ft.dwLowDateTime;


            /*converting file time to unix epoch*/
            tmpres /= 10;  /*convert into microseconds*/
            tmpres -= DELTA_EPOCH_IN_MICROSECS;
            tv->tv_sec = (long)(tmpres / 1000000UL);
            tv->tv_usec = (long)(tmpres % 1000000UL);
        }


        if (tz) {
            if (!tzflag) {
#if !TSK_UNDER_WINDOWS_RT
                _tzset();
#endif
                tzflag++;
            }
            tz->tz_minuteswest = _timezone / 60;
            tz->tz_dsttime = _daylight;
        }


        return 0;
}


void Foam::clockTime::getTime(timeType& t)
{
    gettimeofday(&t, NULL);
}


double Foam::clockTime::timeDifference(const timeType& beg, const timeType& end)
{
    return end.tv_sec - beg.tv_sec + 1e-6*(end.tv_usec - beg.tv_usec);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::clockTime::clockTime()
{
    getTime(startTime_);
    lastTime_ = startTime_;
    newTime_ = startTime_;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

double Foam::clockTime::elapsedTime() const
{
    getTime(newTime_);
    return timeDifference(startTime_, newTime_);
}


double Foam::clockTime::timeIncrement() const
{
    lastTime_ = newTime_;
    getTime(newTime_);
    return timeDifference(lastTime_, newTime_);
}


// ************************************************************************* //
