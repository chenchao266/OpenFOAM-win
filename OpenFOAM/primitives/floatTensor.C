/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016 OpenFOAM Foundation
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

#include "floatTensor.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


 namespace Foam{
template<>
const char* const floatTensor::vsType::typeName = "floatTensor";

template<>
const char* const floatTensor::vsType::componentNames[] =
{
    "xx", "xy", "xz",
    "yx", "yy", "yz",
    "zx", "zy", "zz"
};

template<>
const floatTensor floatTensor::vsType::zero_
(
    floatTensor::uniform(0)
);

template<>
const floatTensor floatTensor::vsType::one_
(
    floatTensor::uniform(1)
);

template<>
const floatTensor floatTensor::vsType::max_
(
    floatTensor::uniform(floatScalarVGREAT)
);

template<>
const floatTensor floatTensor::vsType::min_
(
    floatTensor::uniform(-floatScalarVGREAT)
);

template<>
const floatTensor floatTensor::vsType::rootMax_
(
    floatTensor::uniform(floatScalarROOTVGREAT)
);

template<>
const floatTensor floatTensor::vsType::rootMin_
(
    floatTensor::uniform(-floatScalarROOTVGREAT)
);

template<>
const floatTensor floatTensor::I
(
    1, 0, 0,
    0, 1, 0,
    0, 0, 1
);


// ************************************************************************* //

 } // End namespace Foam
