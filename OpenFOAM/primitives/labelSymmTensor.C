﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
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

#include "labelSymmTensor.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


 namespace Foam{
template<>
const char* const labelSymmTensor::vsType::typeName = "labelSymmTensor";

template<>
const char* const labelSymmTensor::vsType::componentNames[] =
{
    "xx", "xy", "xz",
          "yy", "yz",
                "zz"
};

template<>
const labelSymmTensor labelSymmTensor::vsType::vsType::zero_
(
    labelSymmTensor::uniform(0)
);

template<>
const labelSymmTensor labelSymmTensor::vsType::one_
(
    labelSymmTensor::uniform(1)
);

template<>
const labelSymmTensor labelSymmTensor::vsType::max_
(
    labelSymmTensor::uniform(labelMax)
);

template<>
const labelSymmTensor labelSymmTensor::vsType::min_
(
    labelSymmTensor::uniform(-labelMax)
);

template<>
const labelSymmTensor labelSymmTensor::vsType::rootMax_
(
    labelSymmTensor::uniform(sqrt(scalar(labelMax)))
);

template<>
const labelSymmTensor labelSymmTensor::vsType::rootMin_
(
    labelSymmTensor::uniform(-sqrt(scalar(labelMax)))
);

template<>
const labelSymmTensor labelSymmTensor::I
(
    1, 0, 0,
       1, 0,
          1
);


// ************************************************************************* //

 } // End namespace Foam
