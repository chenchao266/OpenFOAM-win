/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2019 OpenCFD Ltd.
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
    Vector of complex numbers.

\*---------------------------------------------------------------------------*/

#include "complexVector.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


 namespace Foam{
template<>
const char* const complexVector::vsType::typeName = "complexVector";

template<>
const char* const complexVector::vsType::componentNames[] =
{
    "x", "y", "z"
};

template<>
const complexVector complexVector::vsType::zero_
(
    complexVector::uniform(pTraits<complex>::zero_)
);

template<>
const complexVector complexVector::vsType::one_
(
    complexVector::uniform(pTraits<complex>::one_)
);

template<>
const complexVector complexVector::vsType::max_
(
    complexVector::uniform(pTraits<complex>::max_)
);

template<>
const complexVector complexVector::vsType::min_
(
    complexVector::uniform(pTraits<complex>::min_)
);

template<>
const complexVector complexVector::vsType::rootMax_
(
    complexVector::uniform(pTraits<complex>::rootMax_)
);

template<>
const complexVector complexVector::vsType::rootMin_
(
    complexVector::uniform(pTraits<complex>::rootMin_)
);


// ************************************************************************* //

 } // End namespace Foam
