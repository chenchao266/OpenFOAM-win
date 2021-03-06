﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2017 OpenFOAM Foundation
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

    This file has been created by blueCAPE's unofficial mingw patches for
    OpenFOAM.
    For more information about these patches, visit:
        http://bluecfd.com/Core

    Modifications made:
      - Derived from the patches for blueCFD 2.1 and 2.2.

Class
    Foam::fileStat

Description
    Wrapper for stat() system call.

Warning
    on Linux (an maybe on others) a stat() of an nfs mounted (remote)
    file does never timeout and cannot be interrupted!
    So e.g. Foam::ping first and hope nfs is running.

SourceFiles
    fileStat.C

\*---------------------------------------------------------------------------*/

#ifndef fileStat_H
#define fileStat_H


#include <sys/types.h>

#include <sys/stat.h>
#include <time.h>

#include "label.H"
#include "fileName.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of friend functions and operators

class fileStat;

Istream& operator>>(Istream&, fileStat&);
Ostream& operator<<(Ostream&, const fileStat&);
 
struct stat_extended : stat
{
    _timespec64 st_atim;
};

/*---------------------------------------------------------------------------*\
                           Class fileStat Declaration
\*---------------------------------------------------------------------------*/

class fileStat
{
    // Private data

        struct stat_extended status_;

        bool isValid_;


public:

    // Constructors

        //- Empty constructor
        fileStat();

        //- Construct from components.
        //  followLink : in case of link get status of pointed-to file
        //  maxTime    : time out
        fileStat
        (
            const fileName& fName,
            const bool followLink = true,
            const unsigned int maxTime = 0
        );

        //- Construct from Istream
        fileStat(Istream&);


    // Member Functions

        // Access

            //- Raw status
            const struct stat_extended& status() const
            {
                return status_;
            }

            //- Did constructor fail
            bool isValid() const
            {
                return isValid_;
            }


        // Check

            //- Compare two fileStats for same device
            bool sameDevice(const fileStat& stat2) const;

            //- Compare two fileStats for same Inode
            bool sameINode(const fileStat& stat2) const;

            //- Compare state against inode
            bool sameINode(const label iNode) const;


    // IOstream Operators

        friend Istream& operator>>(Istream&, fileStat&);
        friend Ostream& operator<<(Ostream&, const fileStat&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
