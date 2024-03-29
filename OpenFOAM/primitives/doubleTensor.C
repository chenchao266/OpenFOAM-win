﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
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

#include "doubleTensor.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


 namespace Foam{
template<>
const char* const doubleTensor::vsType::typeName = "doubleTensor";

template<>
const char* const doubleTensor::vsType::componentNames[] =
{
    "xx", "xy", "xz",
    "yx", "yy", "yz",
    "zx", "zy", "zz"
};

template<>
const doubleTensor doubleTensor::vsType::zero_
(
    doubleTensor::uniform(0)
);

template<>
const doubleTensor doubleTensor::vsType::one_
(
    doubleTensor::uniform(1)
);

template<>
const doubleTensor doubleTensor::vsType::max_
(
    doubleTensor::uniform(doubleScalarVGREAT)
);

template<>
const doubleTensor doubleTensor::vsType::min_
(
    doubleTensor::uniform(-doubleScalarVGREAT)
);

template<>
const doubleTensor doubleTensor::vsType::rootMax_
(
    doubleTensor::uniform(doubleScalarROOTVGREAT)
);

template<>
const doubleTensor doubleTensor::vsType::rootMin_
(
    doubleTensor::uniform(-doubleScalarROOTVGREAT)
);

template<>
const doubleTensor doubleTensor::I
(
    1, 0, 0,
    0, 1, 0,
    0, 0, 1
);


// ************************************************************************* //

 } // End namespace Foam
