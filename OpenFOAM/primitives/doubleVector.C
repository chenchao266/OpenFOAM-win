/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2018 OpenCFD Ltd.
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

#include "doubleVector.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


 namespace Foam{
template<>
const char* const doubleVector::vsType::typeName = "doubleVector";

template<>
const char* const doubleVector::vsType::componentNames[] =
{
    "x", "y", "z"
};

template<>
const doubleVector doubleVector::vsType::zero_
(
    doubleVector::uniform(0)
);

template<>
const doubleVector doubleVector::vsType::one_
(
    doubleVector::uniform(1)
);

template<>
const doubleVector doubleVector::vsType::max_
(
    doubleVector::uniform(doubleScalarVGREAT)
);

template<>
const doubleVector doubleVector::vsType::min_
(
    doubleVector::uniform(-doubleScalarVGREAT)
);

template<>
const doubleVector doubleVector::vsType::rootMax_
(
    doubleVector::uniform(doubleScalarROOTVGREAT)
);

template<>
const doubleVector doubleVector::vsType::rootMin_
(
    doubleVector::uniform(-doubleScalarROOTVGREAT)
);


// ************************************************************************* //

 } // End namespace Foam
