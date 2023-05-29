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
#include "physicoChemicalConstants.H"

#include "dimensionedConstants.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace constant
{

defineDimensionedConstantWithDefault
(
    physicoChemical::group,
    physicoChemical::R,
    dimensionedScalar
    (
        word("R"),
        physicoChemical::NA*physicoChemical::k
    ),
    constantphysicoChemicalR,
    "R"
);


defineDimensionedConstantWithDefault
(
    physicoChemical::group,
    physicoChemical::F,
    dimensionedScalar
    (
        word("F"),
        physicoChemical::NA*electromagnetic::e
    ),
    constantphysicoChemicalF,
    "F"
);


// Note: cannot use dimless etc. since not guaranteed to be constructed
defineDimensionedConstantWithDefault
(
    physicoChemical::group,
    physicoChemical::sigma,
    dimensionedScalar
    (
        word("sigma"),
        dimensionedScalar
        (
            word("C"),
            dimensionSet(0, 0, 0, 0, 0),    //dimless,
            sqr(mathematical::pi)/60.0
        )
       *pow4(physicoChemical::k)
       /(pow3(universal::hr)*sqr(universal::c))
    ),
    constantphysicoChemicalsigma,
    "sigma"
);


defineDimensionedConstantWithDefault
(
    physicoChemical::group,
    physicoChemical::b,
    dimensionedScalar
    (
        word("b"),
        (universal::h*universal::c/physicoChemical::k)
       /dimensionedScalar
        (
            word("C"),
            dimensionSet(0, 0, 0, 0, 0),    //dimless
            4.965114231
        )
    ),
    constantphysicoChemicalb,
    "b"
);


defineDimensionedConstantWithDefault
(
    physicoChemical::group,
    physicoChemical::c1,
    dimensionedScalar
    (
        word("c1"),
        dimensionedScalar
        (
            word("C"),
            dimensionSet(0, 0, 0, 0, 0),    //dimless,
            mathematical::twoPi
        )
       *universal::h*sqr(universal::c)
    ),
    constantphysicoChemicalc1,
    "c1"
);


defineDimensionedConstantWithDefault
(
    physicoChemical::group,
    physicoChemical::c2,
    dimensionedScalar
    (
        "c2",
        universal::h*universal::c/physicoChemical::k
    ),
    constantphysicoChemicalc2,
    "c2"
);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace constant
} // End namespace Foam

// ************************************************************************* //
