﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
 2011 blueCAPE: Added hack specific for Win x64, so adding the overloaded
                method "long int elapsedClockTime()".
 2014-02-21 blueCAPE Lda: Modifications for blueCFD-Core 2.3
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

\*---------------------------------------------------------------------------*/

#include "clock.H"
#include "string.T.H"

#include <sstream>
#include <iomanip>

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //
using namespace Foam;
const char *clock::monthNames[] =
{
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec",
};


// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

time_t clock::getTime()
{
    return ::time(reinterpret_cast<time_t*>(0));
}


const struct tm clock::rawDate()
{
    time_t t = getTime();
    struct tm *timeStruct = localtime(&t);
    return *timeStruct;
}


string clock::dateTime()
{
    std::ostringstream osBuffer;

    time_t t = getTime();
    struct tm *timeStruct = localtime(&t);

    osBuffer
        << std::setfill('0')
        << std::setw(4) << timeStruct->tm_year + 1900
        << '-' << std::setw(2) << timeStruct->tm_mon + 1
        << '-' << std::setw(2) << timeStruct->tm_mday
        << 'T'
        << std::setw(2) << timeStruct->tm_hour
        << ':' << std::setw(2) << timeStruct->tm_min
        << ':' << std::setw(2) << timeStruct->tm_sec;

    return osBuffer.str();
}

string clock::date()
{
    std::ostringstream osBuffer;

    time_t t = getTime();
    struct tm *timeStruct = localtime(&t);

    osBuffer
        << monthNames[timeStruct->tm_mon]
        << ' ' << std::setw(2) << std::setfill('0') << timeStruct->tm_mday
        << ' ' << std::setw(4) << timeStruct->tm_year + 1900;

    return osBuffer.str();
}


string clock::clockTime()
{
    std::ostringstream osBuffer;

    time_t t = getTime();
    struct tm *timeStruct = localtime(&t);

    osBuffer
        << std::setfill('0')
        << std::setw(2) << timeStruct->tm_hour
        << ':' << std::setw(2) << timeStruct->tm_min
        << ':' << std::setw(2) << timeStruct->tm_sec;

    return osBuffer.str();
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

clock::clock() :    startTime_(getTime()),
    lastTime_(startTime_),
    newTime_(startTime_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

time_t clock::elapsedClockTime() const
{
    newTime_ = getTime();
    return newTime_ - startTime_;
}

time_t clock::clockTimeIncrement() const
{
    lastTime_ = newTime_;
    newTime_ = getTime();
    return newTime_ - lastTime_;
}


// ************************************************************************* //
