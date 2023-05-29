/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2013 OpenFOAM Foundation
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

#include "fundamentalConstants.H"
#include "mathematicalConstants.H"
#include "universalConstants.H"
#include "electromagneticConstants.H"
#include "atomicConstants.H"
#include "dimensionedConstants.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace constant
{

defineDimensionedConstantWithDefault
(
    electromagnetic::group,
    electromagnetic::mu0,
    dimensionedScalar
    (
        word("mu0"),
        dimensionSet(1, 1, -2, 0, 0, -2, 0),
        4.0*mathematical::pi*1e-07
    ),
    constantelectromagneticmu0,
    "mu0"
);


// Note: cannot use dimless etc. since not guaranteed to be constructed
defineDimensionedConstantWithDefault
(
    electromagnetic::group,
    electromagnetic::epsilon0,

    dimensionedScalar
    (
        word("epsilon0"),
        dimensionedScalar
        (
            word("C"),
            dimensionSet(0, 0, 0, 0, 0),
            1.0
        )
       /(electromagnetic::mu0*sqr(universal::c))
    ),
    constantelectromagneticepsilon0,
    "epsilon0"
);


defineDimensionedConstantWithDefault
(
    electromagnetic::group,
    electromagnetic::Z0,
    dimensionedScalar
    (
        word("Z0"),
        electromagnetic::mu0*universal::c
    ),
    constantelectromagneticZ0,
    "Z0"
);


defineDimensionedConstantWithDefault
(
    electromagnetic::group,
    electromagnetic::kappa,

    dimensionedScalar
    (
        word("kappa"),
        dimensionedScalar
        (
            word("C"),
            dimensionSet(0, 0, 0, 0, 0),
            1.0/(4.0*mathematical::pi)
        )
       /electromagnetic::epsilon0
    ),

    constantelectromagnetickappa,
    "kappa"
);


defineDimensionedConstantWithDefault
(
    electromagnetic::group,
    electromagnetic::G0,
    dimensionedScalar
    (
        word("G0"),
        dimensionedScalar(word("C"), dimensionSet(0, 0, 0, 0, 0), 2)
       *sqr(electromagnetic::e)
       /universal::h
    ),
    constantelectromagneticG0,
    "G0"
);


defineDimensionedConstantWithDefault
(
    electromagnetic::group,
    electromagnetic::KJ,
    dimensionedScalar
    (
        word("KJ"),
        dimensionedScalar(word("C"), dimensionSet(0, 0, 0, 0, 0), 2)
       *electromagnetic::e
       /universal::h
    ),
    constantelectromagneticKJ,
    "KJ"
);


defineDimensionedConstantWithDefault
(
    electromagnetic::group,
    electromagnetic::phi0,
    dimensionedScalar
    (
        word("phi0"),
        universal::h
       /(
            dimensionedScalar(word("C"), dimensionSet(0, 0, 0, 0, 0), 2)
           *electromagnetic::e
        )
    ),
    constantelectromagneticphi0,
    "phi0"
);


defineDimensionedConstantWithDefault
(
    electromagnetic::group,
    electromagnetic::RK,
    dimensionedScalar
    (
        word("RK"),
        universal::h/sqr(electromagnetic::e)
    ),
    constantelectromagneticRK,
    "RK"
);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace constant
} // End namespace Foam

// ************************************************************************* //
