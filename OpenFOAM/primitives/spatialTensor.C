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

#include "spatialTensor2.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


 namespace Foam{
template<>
const char* const spatialTensor::vsType::typeName = "spatialTensor";

template<>
const char* const spatialTensor::vsType::componentNames[] =
{
    "Exx",  "Exy",  "Exz",    "Erxx", "Erxy", "Erxz",
    "Eyx",  "Eyy",  "Eyz",    "Eryx", "Eryy", "Eryz",
    "Ezx",  "Ezy",  "Ezz",    "Erzx", "Erzy", "Erzz",

    "Erxx", "Erxy", "Erxz",   "Exx",  "Exy",  "Exz",
    "Eryx", "Eryy", "Eryz",   "Eyx",  "Eyy",  "Eyz",
    "Erzx", "Erzy", "Erzz",   "Ezx",  "Ezy",  "Ezz"
};

template<>
const spatialTensor spatialTensor::vsType::zero_
(
    spatialTensor::uniform(0)
);

template<>
const spatialTensor spatialTensor::vsType::one_
(
    spatialTensor::uniform(1)
);

template<>
const spatialTensor spatialTensor::vsType::max_
(
    spatialTensor::uniform(VGREAT)
);

template<>
const spatialTensor spatialTensor::vsType::min_
(
    spatialTensor::uniform(-VGREAT)
);

template<>
const spatialTensor spatialTensor::vsType::rootMax_
(
    spatialTensor::uniform(ROOTVGREAT)
);

template<>
const spatialTensor spatialTensor::vsType::rootMin_
(
    spatialTensor::uniform(-ROOTVGREAT)
);

template<>
const spatialTensor spatialTensor::I
(
    spatialTensor::identity()
);


// ************************************************************************* //

 } // End namespace Foam
