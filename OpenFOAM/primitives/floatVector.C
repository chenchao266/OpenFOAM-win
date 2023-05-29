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

#include "floatVector.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


 namespace Foam{
template<>
const char* const floatVector::vsType::typeName = "floatVector";

template<>
const char* const floatVector::vsType::componentNames[] =
{
    "x", "y", "z"
};

template<>
const floatVector floatVector::vsType::zero_
(
    floatVector::uniform(0)
);

template<>
const floatVector floatVector::vsType::one_
(
    floatVector::uniform(1)
);

template<>
const floatVector floatVector::vsType::max_
(
    floatVector::uniform(floatScalarVGREAT)
);

template<>
const floatVector floatVector::vsType::min_
(
    floatVector::uniform(-floatScalarVGREAT)
);

template<>
const floatVector floatVector::vsType::rootMax_
(
    floatVector::uniform(floatScalarROOTVGREAT)
);

template<>
const floatVector floatVector::vsType::rootMin_
(
    floatVector::uniform(-floatScalarROOTVGREAT)
);


// ************************************************************************* //

 } // End namespace Foam
