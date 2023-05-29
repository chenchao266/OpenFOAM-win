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

Description
    Vector of scalars.

\*---------------------------------------------------------------------------*/

#include "vector2.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


 namespace Foam{
template<>
const char* const vector::vsType::typeName = "vector";

template<>
const char* const vector::vsType::componentNames[] = {"x", "y", "z"};

template<>
const vector vector::vsType::zero_(vector::uniform(0));

template<>
const vector vector::vsType::one_(vector::uniform(1));

template<>
const vector vector::vsType::max_(vector::uniform(VGREAT));

template<>
const vector vector::vsType::min_(vector::uniform(-VGREAT));

template<>
const vector vector::vsType::rootMax_(vector::uniform(ROOTVGREAT));

template<>
const vector vector::vsType::rootMin_(vector::uniform(-ROOTVGREAT));


// ************************************************************************* //

 } // End namespace Foam
