﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2015 OpenFOAM Foundation
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
    Specialisation of FieldField\<T\> for symmTensor.

\*---------------------------------------------------------------------------*/

//#include "symmTensorFieldField.H"

#define TEMPLATE template<template<class> class Field>
#include "FieldFieldFunctionsM.C"

// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //


 namespace Foam{
template<template<class> class Field, class Cmpt>
void zip
(
    FieldField<Field, SymmTensor<Cmpt>>& result,
    const FieldField<Field, Cmpt>& xx,
    const FieldField<Field, Cmpt>& xy,
    const FieldField<Field, Cmpt>& xz,
    const FieldField<Field, Cmpt>& yy,
    const FieldField<Field, Cmpt>& yz,
    const FieldField<Field, Cmpt>& zz
)
{
    forAll(result, i)
    {
        zip
        (
            result[i],
            xx[i], xy[i], xz[i],
            /*yx*/ yy[i], yz[i],
            /*zx   zy */  zz[i]
        );
    }
}


template<template<class> class Field, class Cmpt>
void unzip
(
    const FieldField<Field, SymmTensor<Cmpt>>& input,
    FieldField<Field, Cmpt>& xx,
    FieldField<Field, Cmpt>& xy,
    FieldField<Field, Cmpt>& xz,
    FieldField<Field, Cmpt>& yy,
    FieldField<Field, Cmpt>& yz,
    FieldField<Field, Cmpt>& zz
)
{
    forAll(input, i)
    {
        unzip
        (
            input[i],
            xx[i], xy[i], xz[i],
            /*yx*/ yy[i], yz[i],
            /*zx   zy */  zz[i]
        );
    }
}


template<template<class> class Field, class Cmpt>
void unzipDiag
(
    const FieldField<Field, SymmTensor<Cmpt>>& input,
    FieldField<Field, Vector<Cmpt>>& result
)
{
    forAll(input, i)
    {
        unzipDiag(input[i], result[i]);
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


 } // End namespace Foam
namespace Foam
{

// * * * * * * * * * * * * * * * global operators  * * * * * * * * * * * * * //

UNARY_FUNCTION(symmTensor, vector, sqr)
UNARY_FUNCTION(symmTensor, symmTensor, innerSqr)

UNARY_FUNCTION(scalar, symmTensor, tr)
UNARY_FUNCTION(sphericalTensor, symmTensor, sph)
UNARY_FUNCTION(symmTensor, symmTensor, symm)
UNARY_FUNCTION(symmTensor, symmTensor, twoSymm)
UNARY_FUNCTION(symmTensor, symmTensor, dev)
UNARY_FUNCTION(symmTensor, symmTensor, dev2)
UNARY_FUNCTION(scalar, symmTensor, det)
UNARY_FUNCTION(symmTensor, symmTensor, cof)
UNARY_FUNCTION(symmTensor, symmTensor, inv)


// * * * * * * * * * * * * * * * global operators  * * * * * * * * * * * * * //

UNARY_OPERATOR(vector, symmTensor, *, hdual)

BINARY_OPERATOR(tensor, symmTensor, symmTensor, &, dot)
BINARY_TYPE_OPERATOR(tensor, symmTensor, symmTensor, &, dot)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "undefFieldFunctionsM.H"

// ************************************************************************* //
