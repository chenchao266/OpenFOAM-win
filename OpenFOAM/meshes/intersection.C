﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011 OpenFOAM Foundation
     \\/     M anipulation  |
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

#include "intersection.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //
using namespace Foam;
scalar intersection::planarTol_ = 0.2;

namespace Foam
{
    template<>
    const char* NamedEnum
    <
        intersection::direction,
        2
    >::names[] =
    {
        "vector",
        "contactSphere"
    };

    template<>
    const char* NamedEnum
    <
        intersection::algorithm,
        3
    >::names[] =
    {
        "fullRay",
        "halfRay",
        "visible"
    };
}

const NamedEnum<intersection::direction, 2>
intersection::directionNames_;

const NamedEnum<intersection::algorithm, 3>
intersection::algorithmNames_;


// ************************************************************************* //
