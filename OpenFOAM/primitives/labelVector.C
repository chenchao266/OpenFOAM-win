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

#include "labelVector.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


 namespace Foam{
template<>
const char* const labelVector::vsType::typeName = "labelVector";

template<>
const char* const labelVector::vsType::componentNames[] =
{
    "x", "y", "z"
};

template<>
const labelVector labelVector::vsType::zero_
(
    labelVector::uniform(0)
);

template<>
const labelVector labelVector::vsType::one_
(
    labelVector::uniform(1)
);

template<>
const labelVector labelVector::vsType::max_
(
    labelVector::uniform(labelMax)
);

template<>
const labelVector labelVector::vsType::min_
(
    labelVector::uniform(-labelMax)
);

template<>
const labelVector labelVector::vsType::rootMax_
(
    labelVector::uniform(sqrt(scalar(labelMax)))
);

template<>
const labelVector labelVector::vsType::rootMin_
(
    labelVector::uniform(-sqrt(scalar(labelMax)))
);


// ************************************************************************* //

 } // End namespace Foam
