﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2016 OpenFOAM Foundation
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

Description
    Vector of floats.

\*---------------------------------------------------------------------------*/

#include "floatVector.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //
namespace Foam {
    template<>
    const char* const floatVector::vsType::typeName = "floatVector";

    template<>
    const char* const floatVector::vsType::componentNames[] =
    {
        "x", "y", "z"
    };

    template<>
    const floatVector floatVector::vsType::_zero
    (
        floatVector::uniform(0)
    );

    template<>
    const floatVector floatVector::vsType::one
    (
        floatVector::uniform(1)
    );

    template<>
    const floatVector floatVector::vsType::max
    (
        floatVector::uniform(floatScalarVGREAT)
    );

    template<>
    const floatVector floatVector::vsType::min
    (
        floatVector::uniform(-floatScalarVGREAT)
    );

    template<>
    const floatVector floatVector::vsType::rootMax
    (
        floatVector::uniform(floatScalarROOTVGREAT)
    );

    template<>
    const floatVector floatVector::vsType::rootMin
    (
        floatVector::uniform(-floatScalarROOTVGREAT)
    );

}
// ************************************************************************* //
