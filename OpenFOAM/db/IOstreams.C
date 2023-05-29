/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011 OpenFOAM Foundation
    Copyright (C) 2018-2020 OpenCFD Ltd.
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

Note
    File included by global/global.Cver

\*---------------------------------------------------------------------------*/

#include "IOstreamOption.H"
#include "IOstreams.H"
#include "OFstream.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

// Default output precision

 namespace Foam{
unsigned int IOstream::precision_
(
    debug::infoSwitch("writePrecision", 6)
);


// * * * * * * * * * * * * * * Global Variables  * * * * * * * * * * * * * * //

// Global IO streams

ISstream Sin(std::cin, "Sin");
OSstream Sout(std::cout, "Sout");
OSstream Serr(std::cerr, "Serr");
OFstream Snull(nullptr);  // A "/dev/null" equivalent

prefixOSstream Pout(std::cout, "Pout");
prefixOSstream Perr(std::cerr, "Perr");


// ************************************************************************* //

 } // End namespace Foam
