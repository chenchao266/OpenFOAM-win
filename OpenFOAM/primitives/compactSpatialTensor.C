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

#include "compactSpatialTensor2.H"
#include "CompactSpatialTensorT.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


 namespace Foam{
template<>
const char* const compactSpatialTensor::vsType::typeName =
    "compactSpatialTensor";

template<>
const char* const compactSpatialTensor::vsType::componentNames[] =
{
    "Exx",  "Exy",  "Exz",
    "Eyx",  "Eyy",  "Eyz",
    "Ezx",  "Ezy",  "Ezz",

    "Erxx", "Erxy", "Erxz",
    "Eryx", "Eryy", "Eryz",
    "Erzx", "Erzy", "Erzz",
};

template<>
const compactSpatialTensor compactSpatialTensor::vsType::zero_
(
    compactSpatialTensor::uniform(0)
);

template<>
const compactSpatialTensor compactSpatialTensor::vsType::one_
(
    compactSpatialTensor::uniform(1)
);

template<>
const compactSpatialTensor compactSpatialTensor::vsType::max_
(
    compactSpatialTensor::uniform(VGREAT)
);

template<>
const compactSpatialTensor compactSpatialTensor::vsType::min_
(
    compactSpatialTensor::uniform(-VGREAT)
);

template<>
const compactSpatialTensor compactSpatialTensor::vsType::rootMax_
(
    compactSpatialTensor::uniform(ROOTVGREAT)
);

template<>
const compactSpatialTensor compactSpatialTensor::vsType::rootMin_
(
    compactSpatialTensor::uniform(-ROOTVGREAT)
);


// ************************************************************************* //

 } // End namespace Foam
