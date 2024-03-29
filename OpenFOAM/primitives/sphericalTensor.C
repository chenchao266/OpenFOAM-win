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

#include "sphericalTensor2.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


 namespace Foam{
template<>
const char* const sphericalTensor::vsType::typeName = "sphericalTensor";

template<>
const char* const sphericalTensor::vsType::componentNames[] = {"ii"};

template<>
const sphericalTensor sphericalTensor::vsType::zero_
(
    sphericalTensor::uniform(0)
);

template<>
const sphericalTensor sphericalTensor::vsType::one_
(
    sphericalTensor::uniform(1)
);

template<>
const sphericalTensor sphericalTensor::vsType::max_
(
    sphericalTensor::uniform(VGREAT)
);

template<>
const sphericalTensor sphericalTensor::vsType::min_
(
    sphericalTensor::uniform(-VGREAT)
);

template<>
const sphericalTensor sphericalTensor::vsType::rootMax_
(
    sphericalTensor::uniform(ROOTVGREAT)
);

template<>
const sphericalTensor sphericalTensor::vsType::rootMin_
(
    sphericalTensor::uniform(-ROOTVGREAT)
);

template<>
const sphericalTensor sphericalTensor::I(1);

template<>
const sphericalTensor sphericalTensor::oneThirdI(1.0/3.0);

template<>
const sphericalTensor sphericalTensor::twoThirdsI(2.0/3.0);

// ************************************************************************* //

 } // End namespace Foam
