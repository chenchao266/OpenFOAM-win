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

#include "labelSphericalTensor.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


 namespace Foam{
template<>
const char* const labelSphericalTensor::vsType::typeName
(
    "labelSphericalTensor"
);

template<>
const char* const labelSphericalTensor::vsType::componentNames[] =
{
    "ii"
};

template<>
const labelSphericalTensor
labelSphericalTensor::vsType::vsType::zero_
(
    labelSphericalTensor::uniform(0)
);

template<>
const labelSphericalTensor labelSphericalTensor::vsType::one_
(
    labelSphericalTensor::uniform(1)
);

template<>
const labelSphericalTensor labelSphericalTensor::vsType::max_
(
    labelSphericalTensor::uniform(labelMax)
);

template<>
const labelSphericalTensor labelSphericalTensor::vsType::min_
(
    labelSphericalTensor::uniform(-labelMax)
);

template<>
const labelSphericalTensor labelSphericalTensor::vsType::rootMax_
(
    labelSphericalTensor::uniform(sqrt(scalar(labelMax)))
);

template<>
const labelSphericalTensor labelSphericalTensor::vsType::rootMin_
(
    labelSphericalTensor::uniform(-sqrt(scalar(labelMax)))
);

template<>
const labelSphericalTensor labelSphericalTensor::I(1);


// ************************************************************************* //

 } // End namespace Foam
