/*---------------------------------------------------------------------------*\
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

Class
    Foam::clock

Description
    Read access to the system clock with formatting.

SourceFiles
    clock.C

\*---------------------------------------------------------------------------*/

#ifndef clock_H
#define clock_H

#include <ctime>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class string;

/*---------------------------------------------------------------------------*\
                           Class clock Declaration
\*---------------------------------------------------------------------------*/

class clock
{
    // Private data

        //- Start time in seconds
        time_t startTime_;

        //- Time when clockTimeIncrement() was last called
        mutable time_t lastTime_;

        //- Latest time from either elapsedClockTime() or clockTimeIncrement()
        mutable time_t newTime_;

        //- Names of the months
        static const char *monthNames[];


public:

    // Constructors

        //- Null constructor which stores the start time
        clock();


    // Member Functions

        //- Get the current clock time in seconds
        static time_t getTime();

        //- Return the current wall-clock date as a raw struct
        static const struct tm rawDate();

        //- Return the current wall-clock date/time as a string
        //  format according to ISO-8601 (yyyy-mm-ddThh:mm:ss)
        static string dateTime();

        //- Return the current wall-clock date as a string
        static string date();

        //- Return the current wall-clock time as a string
        static string clockTime();

        //- Returns wall-clock time from clock instantiation
        time_t elapsedClockTime() const;

        //- Returns wall-clock time from last call of clockTimeIncrement()
        time_t clockTimeIncrement() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
