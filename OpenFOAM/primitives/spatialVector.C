﻿/*---------------------------------------------------------------------------*\
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

Description
    SpatialVector of scalars.

\*---------------------------------------------------------------------------*/

#include "spatialVector2.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


 namespace Foam{
template<>
const char* const spatialVector::vsType::typeName = "spatialVector";

template<>
const char* const spatialVector::vsType::componentNames[] =
{
    "wx", "wy", "wz", "lx", "ly", "lz"
};

template<>
const spatialVector spatialVector::vsType::zero_
(
    spatialVector::uniform(0)
);

template<>
const spatialVector spatialVector::vsType::one_
(
    spatialVector::uniform(1)
);

template<>
const spatialVector spatialVector::vsType::max_
(
    spatialVector::uniform(VGREAT)
);

template<>
const spatialVector spatialVector::vsType::min_
(
    spatialVector::uniform(-VGREAT)
);

template<>
const spatialVector spatialVector::vsType::rootMax_
(
    spatialVector::uniform(ROOTVGREAT)
);

template<>
const spatialVector spatialVector::vsType::rootMin_
(
    spatialVector::uniform(-ROOTVGREAT)
);


// ************************************************************************* //

 } // End namespace Foam
