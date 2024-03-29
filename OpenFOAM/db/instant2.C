﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
    Copyright (C) 2018 OpenCFD Ltd.
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

#include "instant2.H"
#include "Time1.h"
#include <cstdlib>

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


 namespace Foam{
const char* const instant::typeName = "instant";


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

instant::instant(scalar timeValue)
:
    Instant<word>(timeValue, Time::timeName(timeValue))
{}


instant::instant(const word& timeName)
:
    Instant<word>(std::atof(timeName.c_str()), timeName)
{}


instant::instant(word&& timeName)
:
    Instant<word>(std::atof(timeName.c_str()), std::move(timeName))
{}


// ************************************************************************* //

 } // End namespace Foam
