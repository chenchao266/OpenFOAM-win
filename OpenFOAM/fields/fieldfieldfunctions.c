﻿/*---------------------------------------------------------------------------*\
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

\*---------------------------------------------------------------------------*/

#include "PstreamReduceOps.H"
#include "FieldFieldReuseFunctions.H"

#define TEMPLATE template<template<class> class Field, class Type>
#include "FieldFieldFunctionsM.C"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/* * * * * * * * * * * * * * * * Global functions  * * * * * * * * * * * * * */

template<template<class> class Field, class Type>
void component
(
    FieldField<Field, typename FieldField<Field, Type>::cmptType>& sf,
    const FieldField<Field, Type>& f,
    const direction d
)
{
    forAll(sf, i)
    {
        component(sf[i], f[i], d);
    }
}


template<template<class> class Field, class Type>
void T(FieldField<Field, Type>& f1, const FieldField<Field, Type>& f2)
{
    forAll(f1, i)
    {
        T(f1[i], f2[i]);
    }
}


template<template<class> class Field, class Type, direction r>
void pow
(
    FieldField<Field, typename powProduct<Type, r>::type>& f,
    const FieldField<Field, Type>& vf
)
{
    forAll(f, i)
    {
        pow(f[i], vf[i]);
    }
}

template<template<class> class Field, class Type, direction r>
tmp<FieldField<Field, typename powProduct<Type, r>::type>>
pow
(
    const FieldField<Field, Type>& f, typename powProduct<Type, r>::type
)
{
    typedef typename powProduct<Type, r>::type powProductType;

    auto tres
    (
        FieldField<Field, powProductType>::NewCalculatedType(f)
    );

    pow<Type, r>(tres.ref(), f);
    return tres;
}

template<template<class> class Field, class Type, direction r>
tmp<FieldField<Field, typename powProduct<Type, r>::type>>
pow
(
    const tmp<FieldField<Field, Type>>& tf, typename powProduct<Type, r>::type
)
{
    typedef typename powProduct<Type, r>::type powProductType;

    auto tres
    (
        reuseTmpFieldField<Field, powProductType, Type>::New(tf)
    );

    pow<Type, r>(tres.ref(), tf());
    tf.clear();
    return tres;
}


template<template<class> class Field, class Type>
void sqr
(
    FieldField<Field, typename outerProduct<Type, Type>::type>& f,
    const FieldField<Field, Type>& vf
)
{
    forAll(f, i)
    {
        sqr(f[i], vf[i]);
    }
}

template<template<class> class Field, class Type>
tmp<FieldField<Field, typename outerProduct<Type, Type>::type>>
sqr(const FieldField<Field, Type>& f)
{
    typedef typename outerProduct<Type, Type>::type outerProductType;
    tmp<FieldField<Field, outerProductType>> tres
    (
        FieldField<Field, outerProductType>::NewCalculatedType(f)
    );
    sqr(tres.ref(), f);
    return tres;
}

template<template<class> class Field, class Type>
tmp<FieldField<Field, typename outerProduct<Type, Type>::type>>
sqr(const tmp<FieldField<Field, Type>>& tf)
{
    typedef typename outerProduct<Type, Type>::type outerProductType;

    auto tres
    (
        reuseTmpFieldField<Field, outerProductType, Type>::New(tf)
    );

    sqr(tres.ref(), tf());
    tf.clear();
    return tres;
}


template<template<class> class Field, class Type>
void magSqr
(
    FieldField<Field, typename typeOfMag<Type>::type>& sf,
    const FieldField<Field, Type>& f
)
{
    forAll(sf, i)
    {
        magSqr(sf[i], f[i]);
    }
}

template<template<class> class Field, class Type>
tmp<FieldField<Field, typename typeOfMag<Type>::type>>
magSqr(const FieldField<Field, Type>& f)
{
    typedef typename typeOfMag<Type>::type magType;

    auto tres
    (
        FieldField<Field, magType>::NewCalculatedType(f)
    );

    magSqr(tres.ref(), f);
    return tres;
}

template<template<class> class Field, class Type>
tmp<FieldField<Field, typename typeOfMag<Type>::type>>
magSqr(const tmp<FieldField<Field, Type>>& tf)
{
    typedef typename typeOfMag<Type>::type magType;

    auto tres
    (
        reuseTmpFieldField<Field, magType, Type>::New(tf)
    );

    magSqr(tres.ref(), tf());
    tf.clear();
    return tres;
}


template<template<class> class Field, class Type>
void mag
(
    FieldField<Field, typename typeOfMag<Type>::type>& sf,
    const FieldField<Field, Type>& f
)
{
    forAll(sf, i)
    {
        mag(sf[i], f[i]);
    }
}

template<template<class> class Field, class Type>
tmp<FieldField<Field, typename typeOfMag<Type>::type>>
mag(const FieldField<Field, Type>& f)
{
    typedef typename typeOfMag<Type>::type magType;

    auto tres
    (
        FieldField<Field, magType>::NewCalculatedType(f)
    );

    mag(tres.ref(), f);
    return tres;
}

template<template<class> class Field, class Type>
tmp<FieldField<Field, typename typeOfMag<Type>::type>>
mag(const tmp<FieldField<Field, Type>>& tf)
{
    typedef typename typeOfMag<Type>::type magType;

    auto tres
    (
        reuseTmpFieldField<Field, magType, Type>::New(tf)
    );

    mag(tres.ref(), tf());
    tf.clear();
    return tres;
}


template<template<class> class Field, class Type>
void cmptMax
(
    FieldField<Field, typename FieldField<Field, Type>::cmptType>& cf,
    const FieldField<Field, Type>& f
)
{
    forAll(cf, i)
    {
        cmptMax(cf[i], f[i]);
    }
}

template<template<class> class Field, class Type>
tmp<FieldField<Field, typename FieldField<Field, Type>::cmptType>> cmptMax
(
    const FieldField<Field, Type>& f
)
{
    typedef typename FieldField<Field, Type>::cmptType cmptType;

    auto tres
    (
        FieldField<Field, cmptType>::NewCalculatedType(f)
    );

    cmptMax(tres.ref(), f);
    return tres;
}

template<template<class> class Field, class Type>
tmp<FieldField<Field, typename FieldField<Field, Type>::cmptType>> cmptMax
(
    const tmp<FieldField<Field, Type>>& tf
)
{
    typedef typename FieldField<Field, Type>::cmptType cmptType;

    auto tres
    (
        reuseTmpFieldField<Field, cmptType, Type>::New(tf)
    );

    cmptMax(tres.ref(), tf());
    tf.clear();
    return tres;
}


template<template<class> class Field, class Type>
void cmptMin
(
    FieldField<Field, typename FieldField<Field, Type>::cmptType>& cf,
    const FieldField<Field, Type>& f
)
{
    forAll(cf, i)
    {
        cmptMin(cf[i], f[i]);
    }
}

template<template<class> class Field, class Type>
tmp<FieldField<Field, typename FieldField<Field, Type>::cmptType>> cmptMin
(
    const FieldField<Field, Type>& f
)
{
    typedef typename FieldField<Field, Type>::cmptType cmptType;

    auto tres
    (
        FieldField<Field, cmptType>::NewCalculatedType(f)
    );

    cmptMin(tres.ref(), f);
    return tres;
}

template<template<class> class Field, class Type>
tmp<FieldField<Field, typename FieldField<Field, Type>::cmptType>> cmptMin
(
    const tmp<FieldField<Field, Type>>& tf
)
{
    typedef typename FieldField<Field, Type>::cmptType cmptType;

    auto tres
    (
        reuseTmpFieldField<Field, cmptType, Type>::New(tf)
    );

    cmptMin(tres.ref(), tf());
    tf.clear();
    return tres;
}


template<template<class> class Field, class Type>
void cmptAv
(
    FieldField<Field, typename FieldField<Field, Type>::cmptType>& cf,
    const FieldField<Field, Type>& f
)
{
    forAll(cf, i)
    {
        cmptAv(cf[i], f[i]);
    }
}

template<template<class> class Field, class Type>
tmp<FieldField<Field, typename FieldField<Field, Type>::cmptType>> cmptAv
(
    const FieldField<Field, Type>& f
)
{
    typedef typename FieldField<Field, Type>::cmptType cmptType;

    auto tres
    (
        FieldField<Field, cmptType>::NewCalculatedType(f)
    );

    cmptAv(tres.ref(), f);
    return tres;
}

template<template<class> class Field, class Type>
tmp<FieldField<Field, typename FieldField<Field, Type>::cmptType>> cmptAv
(
    const tmp<FieldField<Field, Type>>& tf
)
{
    typedef typename FieldField<Field, Type>::cmptType cmptType;

    auto tres
    (
        reuseTmpFieldField<Field, cmptType, Type>::New(tf)
    );

    cmptAv(tres.ref(), tf());
    tf.clear();
    return tres;
}


template<template<class> class Field, class Type>
void cmptMag
(
    FieldField<Field, Type>& cf,
    const FieldField<Field, Type>& f
)
{
    forAll(cf, i)
    {
        cmptMag(cf[i], f[i]);
    }
}

template<template<class> class Field, class Type>
tmp<FieldField<Field, Type>> cmptMag
(
    const FieldField<Field, Type>& f
)
{
    auto tres
    (
        FieldField<Field, Type>::NewCalculatedType(f)
    );

    cmptMag(tres.ref(), f);
    return tres;
}

template<template<class> class Field, class Type>
tmp<FieldField<Field, Type>> cmptMag
(
    const tmp<FieldField<Field, Type>>& tf
)
{
    tmp<FieldField<Field, Type>> tres(New(tf));
    cmptMag(tres.ref(), tf());
    tf.clear();
    return tres;
}


#define TMP_UNARY_FUNCTION(returnType, func)                                   \
                                                                               \
template<template<class> class Field, class Type>                              \
returnType func(const tmp<FieldField<Field, Type>>& tf1)                       \
{                                                                              \
    returnType res = func(tf1());                                              \
    tf1.clear();                                                               \
    return res;                                                                \
}

template<template<class> class Field, class Type>
Type max(const FieldField<Field, Type>& f)
{
    Type result = pTraits<Type>::min_;

    forAll(f, i)
    {
        if (f[i].size())
        {
            result = max(max(f[i]), result);
        }

    }

    return result;
}

TMP_UNARY_FUNCTION(Type, max)


template<template<class> class Field, class Type>
Type min(const FieldField<Field, Type>& f)
{
    Type result = pTraits<Type>::max_;

    forAll(f, i)
    {
        if (f[i].size())
        {
            result = min(min(f[i]), result);
        }
    }

    return result;
}

TMP_UNARY_FUNCTION(Type, min)


template<template<class> class Field, class Type>
Type sum(const FieldField<Field, Type>& f)
{
    Type Sum = Zero;

    forAll(f, i)
    {
        Sum += sum(f[i]);
    }

    return Sum;
}

TMP_UNARY_FUNCTION(Type, sum)

template<template<class> class Field, class Type>
typename typeOfMag<Type>::type sumMag(const FieldField<Field, Type>& f)
{
    typedef typename typeOfMag<Type>::type magType;

    magType result = Zero;

    forAll(f, i)
    {
        result += sumMag(f[i]);
    }

    return result;
}

TMP_UNARY_FUNCTION(typename typeOfMag<Type>::type, sumMag)

template<template<class> class Field, class Type>
Type average(const FieldField<Field, Type>& f)
{
    if (f.size())
    {
        label n = 0;

        forAll(f, i)
        {
            n += f[i].size();
        }

        if (n)
        {
            Type avrg = sum(f)/n;

            return avrg;
        }
    }

    WarningInFunction
        << "empty fieldField, returning zero" << endl;

    return Zero;
}

TMP_UNARY_FUNCTION(Type, average)


template<template<class> class Field, class Type>
MinMax<Type> minMax(const FieldField<Field, Type>& f)
{
    MinMax<Type> result;

    forAll(f, i)
    {
        result += minMax(f[i]);
    }

    return result;
}

TMP_UNARY_FUNCTION(MinMax<Type>, minMax)

template<template<class> class Field, class Type>
scalarMinMax minMaxMag(const FieldField<Field, Type>& f)
{
    scalarMinMax result;

    forAll(f, i)
    {
        result += minMaxMag(f[i]);
    }

    return result;
}

TMP_UNARY_FUNCTION(scalarMinMax, minMaxMag)


// With reduction on ReturnType
#define G_UNARY_FUNCTION(ReturnType, gFunc, func, rFunc)                       \
                                                                               \
template<template<class> class Field, class Type>                              \
ReturnType gFunc(const FieldField<Field, Type>& f)                             \
{                                                                              \
    ReturnType res = func(f);                                                  \
    reduce(res, rFunc##Op<ReturnType>());                                      \
    return res;                                                                \
}                                                                              \
TMP_UNARY_FUNCTION(ReturnType, gFunc)

G_UNARY_FUNCTION(Type, gMax, max, max)
G_UNARY_FUNCTION(Type, gMin, min, min)
G_UNARY_FUNCTION(Type, gSum, sum, sum)
G_UNARY_FUNCTION(MinMax<Type>, gMinMax, minMax, sum)
G_UNARY_FUNCTION(scalarMinMax, gMinMaxMag, minMaxMag, sum)

G_UNARY_FUNCTION(typename typeOfMag<Type>::type, gSumMag, sumMag, sum)

#undef G_UNARY_FUNCTION


template<template<class> class Field, class Type>
Type gAverage(const FieldField<Field, Type>& f)
{
    label n = 0;

    forAll(f, i)
    {
        n += f[i].size();
    }

    reduce(n, sumOp<label>());

    if (n)
    {
        Type avrg = gSum(f)/n;

        return avrg;
    }

    WarningInFunction
        << "empty fieldField, returning zero" << endl;

    return Zero;
}

TMP_UNARY_FUNCTION(Type, gAverage)

#undef TMP_UNARY_FUNCTION


BINARY_FUNCTION(Type, Type, Type, max)
BINARY_FUNCTION(Type, Type, Type, min)
BINARY_FUNCTION(Type, Type, Type, cmptMultiply)
BINARY_FUNCTION(Type, Type, Type, cmptDivide)

BINARY_TYPE_FUNCTION(Type, Type, Type, max)
BINARY_TYPE_FUNCTION(Type, Type, Type, min)
BINARY_TYPE_FUNCTION(Type, Type, Type, cmptMultiply)
BINARY_TYPE_FUNCTION(Type, Type, Type, cmptDivide)

BINARY_TYPE_FUNCTION_FS(Type, Type, MinMax<Type>, clip)


/* * * * * * * * * * * * * * * * Global operators  * * * * * * * * * * * * * */

UNARY_OPERATOR(Type, Type, -, negate)

BINARY_OPERATOR(Type, Type, scalar, *, multiply)
BINARY_OPERATOR(Type, scalar, Type, *, multiply)
BINARY_OPERATOR(Type, Type, scalar, /, divide)

BINARY_TYPE_OPERATOR_SF(Type, scalar, Type, *, multiply)
BINARY_TYPE_OPERATOR_FS(Type, Type, scalar, *, multiply)

BINARY_TYPE_OPERATOR_FS(Type, Type, scalar, /, divide)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define PRODUCT_OPERATOR(product, op, opFunc)                                  \
                                                                               \
template                                                                       \
<                                                                              \
    template<class> class Field1,                                              \
    template<class> class Field2,                                              \
    class Type1,                                                               \
    class Type2                                                                \
>                                                                              \
void opFunc                                                                    \
(                                                                              \
    FieldField<Field1, typename product<Type1, Type2>::type>& f,               \
    const FieldField<Field1, Type1>& f1,                                       \
    const FieldField<Field2, Type2>& f2                                        \
)                                                                              \
{                                                                              \
    forAll(f, i)                                                               \
    {                                                                          \
        opFunc(f[i], f1[i], f2[i]);                                            \
    }                                                                          \
}                                                                              \
                                                                               \
template                                                                       \
<                                                                              \
    template<class> class Field1,                                              \
    template<class> class Field2,                                              \
    class Type1,                                                               \
    class Type2                                                                \
>                                                                              \
tmp<FieldField<Field1, typename product<Type1, Type2>::type>>                  \
operator op                                                                    \
(                                                                              \
    const FieldField<Field1, Type1>& f1,                                       \
    const FieldField<Field2, Type2>& f2                                        \
)                                                                              \
{                                                                              \
    typedef typename product<Type1, Type2>::type productType;                  \
    tmp<FieldField<Field1, productType>> tres                                  \
    (                                                                          \
        FieldField<Field1, productType>::NewCalculatedType(f1)                 \
    );                                                                         \
    opFunc(tres.ref(), f1, f2);                                                \
    return tres;                                                               \
}                                                                              \
                                                                               \
template<template<class> class Field, class Type1, class Type2>                \
tmp<FieldField<Field, typename product<Type1, Type2>::type>>                   \
operator op                                                                    \
(                                                                              \
    const FieldField<Field, Type1>& f1,                                        \
    const tmp<FieldField<Field, Type2>>& tf2                                   \
)                                                                              \
{                                                                              \
    typedef typename product<Type1, Type2>::type productType;                  \
    tmp<FieldField<Field, productType>> tres                                   \
    (                                                                          \
        reuseTmpFieldField<Field, productType, Type2>::New(tf2)                \
    );                                                                         \
    opFunc(tres.ref(), f1, tf2());                                             \
    tf2.clear();                                                               \
    return tres;                                                               \
}                                                                              \
                                                                               \
template                                                                       \
<                                                                              \
    template<class> class Field1,                                              \
    template<class> class Field2,                                              \
    class Type1,                                                               \
    class Type2                                                                \
>                                                                              \
tmp<FieldField<Field, typename product<Type1, Type2>::type>>                   \
operator op                                                                    \
(                                                                              \
    const FieldField<Field1, Type1>& f1,                                       \
    const tmp<FieldField<Field2, Type2>>& tf2                                  \
)                                                                              \
{                                                                              \
    typedef typename product<Type1, Type2>::type productType;                  \
    tmp<FieldField<Field1, productType>> tres                                  \
    (                                                                          \
        FieldField<Field1, productType>::NewCalculatedType(f1)                 \
    );                                                                         \
    opFunc(tres.ref(), f1, tf2());                                             \
    tf2.clear();                                                               \
    return tres;                                                               \
}                                                                              \
                                                                               \
template                                                                       \
<                                                                              \
    template<class> class Field1,                                              \
    template<class> class Field2,                                              \
    class Type1,                                                               \
    class Type2                                                                \
>                                                                              \
tmp<FieldField<Field1, typename product<Type1, Type2>::type>>                  \
operator op                                                                    \
(                                                                              \
    const tmp<FieldField<Field1, Type1>>& tf1,                                 \
    const FieldField<Field2, Type2>& f2                                        \
)                                                                              \
{                                                                              \
    typedef typename product<Type1, Type2>::type productType;                  \
    tmp<FieldField<Field1, productType>> tres                                  \
    (                                                                          \
        reuseTmpFieldField<Field1, productType, Type1>::New(tf1)               \
    );                                                                         \
    opFunc(tres.ref(), tf1(), f2);                                             \
    tf1.clear();                                                               \
    return tres;                                                               \
}                                                                              \
                                                                               \
template                                                                       \
<                                                                              \
    template<class> class Field1,                                              \
    template<class> class Field2,                                              \
    class Type1,                                                               \
    class Type2                                                                \
>                                                                              \
tmp<FieldField<Field1, typename product<Type1, Type2>::type>>                  \
operator op                                                                    \
(                                                                              \
    const tmp<FieldField<Field1, Type1>>& tf1,                                 \
    const tmp<FieldField<Field2, Type2>>& tf2                                  \
)                                                                              \
{                                                                              \
    typedef typename product<Type1, Type2>::type productType;                  \
    tmp<FieldField<Field1, productType>> tres                                  \
    (                                                                          \
        reuseTmpTmpFieldField<Field1, productType, Type1, Type1, Type2>::New   \
        (tf1, tf2)                                                             \
    );                                                                         \
    opFunc(tres.ref(), tf1(), tf2());                                          \
    tf1.clear();                                                               \
    tf2.clear();                                                               \
    return tres;                                                               \
}                                                                              \
                                                                               \
template                                                                       \
<                                                                              \
    template<class> class Field,                                               \
    class Type,                                                                \
    class Form,                                                                \
    class Cmpt,                                                                \
    direction nCmpt                                                            \
>                                                                              \
void opFunc                                                                    \
(                                                                              \
    FieldField<Field, typename product<Type, Form>::type>& f,                  \
    const FieldField<Field, Type>& f1,                                         \
    const VectorSpace<Form,Cmpt,nCmpt>& vs                                     \
)                                                                              \
{                                                                              \
    forAll(f, i)                                                               \
    {                                                                          \
        opFunc(f[i], f1[i], vs);                                               \
    }                                                                          \
}                                                                              \
                                                                               \
template                                                                       \
<                                                                              \
    template<class> class Field,                                               \
    class Type,                                                                \
    class Form,                                                                \
    class Cmpt,                                                                \
    direction nCmpt                                                            \
>                                                                              \
tmp<FieldField<Field, typename product<Type, Form>::type>>                     \
operator op                                                                    \
(                                                                              \
    const FieldField<Field, Type>& f1,                                         \
    const VectorSpace<Form,Cmpt,nCmpt>& vs                                     \
)                                                                              \
{                                                                              \
    typedef typename product<Type, Form>::type productType;                    \
    tmp<FieldField<Field, productType>> tres                                   \
    (                                                                          \
        FieldField<Field, productType>::NewCalculatedType(f1)                  \
    );                                                                         \
    opFunc(tres.ref(), f1, static_cast<const Form&>(vs));                      \
    return tres;                                                               \
}                                                                              \
                                                                               \
template                                                                       \
<                                                                              \
    template<class> class Field,                                               \
    class Type,                                                                \
    class Form,                                                                \
    class Cmpt,                                                                \
    direction nCmpt                                                            \
>                                                                              \
tmp<FieldField<Field, typename product<Type, Form>::type>>                     \
operator op                                                                    \
(                                                                              \
    const tmp<FieldField<Field, Type>>& tf1,                                   \
    const VectorSpace<Form,Cmpt,nCmpt>& vs                                     \
)                                                                              \
{                                                                              \
    typedef typename product<Type, Form>::type productType;                    \
    tmp<FieldField<Field, productType>> tres                                   \
    (                                                                          \
        reuseTmpFieldField<Field, productType, Type>::New(tf1)                 \
    );                                                                         \
    opFunc(tres.ref(), tf1(), static_cast<const Form&>(vs));                   \
    tf1.clear();                                                               \
    return tres;                                                               \
}                                                                              \
                                                                               \
template                                                                       \
<                                                                              \
    template<class> class Field,                                               \
    class Type,                                                                \
    class Form,                                                                \
    class Cmpt,                                                                \
    direction nCmpt                                                            \
>                                                                              \
void opFunc                                                                    \
(                                                                              \
    FieldField<Field, typename product<Form, Type>::type>& f,                  \
    const VectorSpace<Form,Cmpt,nCmpt>& vs,                                    \
    const FieldField<Field, Type>& f1                                          \
)                                                                              \
{                                                                              \
    forAll(f, i)                                                               \
    {                                                                          \
        opFunc(f[i], vs, f1[i]);                                               \
    }                                                                          \
}                                                                              \
                                                                               \
template                                                                       \
<                                                                              \
    template<class> class Field,                                               \
    class Type,                                                                \
    class Form,                                                                \
    class Cmpt,                                                                \
    direction nCmpt                                                            \
>                                                                              \
tmp<FieldField<Field, typename product<Form, Type>::type>>                     \
operator op                                                                    \
(                                                                              \
    const VectorSpace<Form,Cmpt,nCmpt>& vs,                                    \
    const FieldField<Field, Type>& f1                                          \
)                                                                              \
{                                                                              \
    typedef typename product<Form, Type>::type productType;                    \
    tmp<FieldField<Field, productType>> tres                                   \
    (                                                                          \
        FieldField<Field, productType>::NewCalculatedType(f1)                  \
    );                                                                         \
    opFunc(tres.ref(), static_cast<const Form&>(vs), f1);                      \
    return tres;                                                               \
}                                                                              \
                                                                               \
template                                                                       \
<                                                                              \
    template<class> class Field,                                               \
    class Type,                                                                \
    class Form,                                                                \
    class Cmpt,                                                                \
    direction nCmpt                                                            \
>                                                                              \
tmp<FieldField<Field, typename product<Form, Type>::type>>                     \
operator op                                                                    \
(                                                                              \
    const VectorSpace<Form,Cmpt,nCmpt>& vs,                                    \
    const tmp<FieldField<Field, Type>>& tf1                                    \
)                                                                              \
{                                                                              \
    typedef typename product<Form, Type>::type productType;                    \
    tmp<FieldField<Field, productType>> tres                                   \
    (                                                                          \
        reuseTmpFieldField<Field, productType, Type>::New(tf1)                 \
    );                                                                         \
    opFunc(tres.ref(), static_cast<const Form&>(vs), tf1());                   \
    tf1.clear();                                                               \
    return tres;                                                               \
}

PRODUCT_OPERATOR(typeOfSum, +, add)
PRODUCT_OPERATOR(typeOfSum, -, subtract)

PRODUCT_OPERATOR(outerProduct, *, outer)
PRODUCT_OPERATOR(crossProduct, ^, cross)
PRODUCT_OPERATOR(innerProduct, &, dot)
PRODUCT_OPERATOR(scalarProduct, &&, dotdot)

#undef PRODUCT_OPERATOR


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "undefFieldFunctionsM.H"

// ************************************************************************* //
