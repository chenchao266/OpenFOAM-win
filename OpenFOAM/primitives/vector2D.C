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
    Vector2D of scalars.

\*---------------------------------------------------------------------------*/

#include "vector2D2.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


 namespace Foam{
template<>
const char* const vector2D::vsType::typeName = "vector2D";

template<>
const char* const vector2D::vsType::componentNames[] = {"x", "y"};

template<>
const vector2D vector2D::vsType::vsType::zero_
(
    vector2D::uniform(0)
);

template<>
const vector2D vector2D::vsType::one_
(
    vector2D::uniform(1)
);

template<>
const vector2D vector2D::vsType::max_
(
    vector2D::uniform(VGREAT)
);

template<>
const vector2D vector2D::vsType::min_
(
    vector2D::uniform(-VGREAT)
);

template<>
const vector2D vector2D::vsType::rootMax_
(
    vector2D::uniform(ROOTVGREAT)
);

template<>
const vector2D vector2D::vsType::rootMin_
(
    vector2D::uniform(-ROOTVGREAT)
);


// ************************************************************************* //

 } // End namespace Foam
