/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011 OpenFOAM Foundation
    Copyright (C) 2019-2021 OpenCFD Ltd.
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
    Specialisation of FieldField\<T\> for tensor.

\*---------------------------------------------------------------------------*/

#include "tensorFieldField.H"

#define TEMPLATE template<template<class> class Field>
#include "FieldFieldFunctionsM.C"

// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //


 namespace Foam{
template<template<class> class Field, class Cmpt>
void zip
(
    FieldField<Field, Tensor<Cmpt>>& result,
    const FieldField<Field, Cmpt>& xx,
    const FieldField<Field, Cmpt>& xy,
    const FieldField<Field, Cmpt>& xz,
    const FieldField<Field, Cmpt>& yx,
    const FieldField<Field, Cmpt>& yy,
    const FieldField<Field, Cmpt>& yz,
    const FieldField<Field, Cmpt>& zx,
    const FieldField<Field, Cmpt>& zy,
    const FieldField<Field, Cmpt>& zz
)
{
    forAll(result, i)
    {
        zip
        (
            result[i],
            xx[i], xy[i], xz[i],
            yx[i], yy[i], yz[i],
            zx[i], zy[i], zz[i]
        );
    }
}


template<template<class> class Field, class Cmpt>
void unzip
(
    const FieldField<Field, Tensor<Cmpt>>& input,
    FieldField<Field, Cmpt>& xx,
    FieldField<Field, Cmpt>& xy,
    FieldField<Field, Cmpt>& xz,
    FieldField<Field, Cmpt>& yx,
    FieldField<Field, Cmpt>& yy,
    FieldField<Field, Cmpt>& yz,
    FieldField<Field, Cmpt>& zx,
    FieldField<Field, Cmpt>& zy,
    FieldField<Field, Cmpt>& zz
)
{
    forAll(input, i)
    {
        unzip
        (
            input[i],
            xx[i], xy[i], xz[i],
            yx[i], yy[i], yz[i],
            zx[i], zy[i], zz[i]
        );
    }
}


template<template<class> class Field, class Cmpt>
void zipRows
(
    FieldField<Field, Tensor<Cmpt>>& result,
    const FieldField<Field, Vector<Cmpt>>& x,
    const FieldField<Field, Vector<Cmpt>>& y,
    const FieldField<Field, Vector<Cmpt>>& z
)
{
    forAll(result, i)
    {
        zipRows(result[i], x[i], y[i], z[i]);
    }
}


template<template<class> class Field, class Cmpt>
void zipCols
(
    FieldField<Field, Tensor<Cmpt>>& result,
    const FieldField<Field, Vector<Cmpt>>& x,
    const FieldField<Field, Vector<Cmpt>>& y,
    const FieldField<Field, Vector<Cmpt>>& z
)
{
    forAll(result, i)
    {
        zipCols(result[i], x[i], y[i], z[i]);
    }
}


template<template<class> class Field, class Cmpt>
void unzipRows
(
    const FieldField<Field, Tensor<Cmpt>>& input,
    FieldField<Field, Vector<Cmpt>>& x,
    FieldField<Field, Vector<Cmpt>>& y,
    FieldField<Field, Vector<Cmpt>>& z
)
{
    forAll(input, i)
    {
        unzipRows(input[i], x[i], y[i], z[i]);
    }
}


template<template<class> class Field, class Cmpt>
void unzipCols
(
    const FieldField<Field, Tensor<Cmpt>>& input,
    FieldField<Field, Vector<Cmpt>>& x,
    FieldField<Field, Vector<Cmpt>>& y,
    FieldField<Field, Vector<Cmpt>>& z
)
{
    forAll(input, i)
    {
        unzipCols(input[i], x[i], y[i], z[i]);
    }
}


template<template<class> class Field, class Cmpt>
void unzipRow
(
    const FieldField<Field, Tensor<Cmpt>>& input,
    const vector::components cmpt,
    FieldField<Field, Vector<Cmpt>>& result
)
{
    forAll(input, i)
    {
        unzipRow(input[i], cmpt, result[i]);
    }
}


template<template<class> class Field, class Cmpt>
void unzipCol
(
    const FieldField<Field, Tensor<Cmpt>>& input,
    const vector::components cmpt,
    FieldField<Field, Vector<Cmpt>>& result
)
{
    forAll(input, i)
    {
        unzipCol(input[i], cmpt, result[i]);
    }
}


template<template<class> class Field, class Cmpt>
void unzipDiag
(
    const FieldField<Field, Tensor<Cmpt>>& input,
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

// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

UNARY_FUNCTION(scalar, tensor, tr)
UNARY_FUNCTION(sphericalTensor, tensor, sph)
UNARY_FUNCTION(symmTensor, tensor, symm)
UNARY_FUNCTION(symmTensor, tensor, twoSymm)
UNARY_FUNCTION(tensor, tensor, skew)
UNARY_FUNCTION(tensor, tensor, dev)
UNARY_FUNCTION(tensor, tensor, dev2)
UNARY_FUNCTION(scalar, tensor, det)
UNARY_FUNCTION(tensor, tensor, cof)
UNARY_FUNCTION(tensor, tensor, inv)

UNARY_FUNCTION(vector, symmTensor, eigenValues)
UNARY_FUNCTION(tensor, symmTensor, eigenVectors)


// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

UNARY_OPERATOR(vector, tensor, *, hdual)
UNARY_OPERATOR(tensor, vector, *, hdual)

BINARY_OPERATOR(vector, vector, tensor, /, divide)
BINARY_TYPE_OPERATOR(vector, vector, tensor, /, divide)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "undefFieldFunctionsM.H"

// ************************************************************************* //
