/*---------------------------------------------------------------------------*\
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

#include "labelTensor.H"


// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


 namespace Foam{
template<>
const char* const labelTensor::vsType::typeName = "labelTensor";

template<>
const char* const labelTensor::vsType::componentNames[] =
{
    "xx", "xy", "xz",
    "yx", "yy", "yz",
    "zx", "zy", "zz"
};

template<>
const labelTensor labelTensor::vsType::zero_
(
    labelTensor::uniform(0)
);

template<>
const labelTensor labelTensor::vsType::one_
(
    labelTensor::uniform(1)
);

template<>
const labelTensor labelTensor::vsType::max_
(
    labelTensor::uniform(labelMax)
);

template<>
const labelTensor labelTensor::vsType::min_
(
    labelTensor::uniform(-labelMax)
);

template<>
const labelTensor labelTensor::vsType::rootMax_
(
    labelTensor::uniform(sqrt(scalar(labelMax)))
);

template<>
const labelTensor labelTensor::vsType::rootMin_
(
    labelTensor::uniform(-sqrt(scalar(labelMax)))
);


// ************************************************************************* //

 } // End namespace Foam
