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

#include "DimensionedFieldReuseFunctions.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define UNARY_FUNCTION(ReturnType, Type1, Func, Dfunc)                         \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> Func                                \
(                                                                              \
    const DimensionedField<Type1, GeoMesh>& df1                                \
)                                                                              \
{                                                                              \
    auto tres =                                                                \
        tmp<DimensionedField<ReturnType, GeoMesh>>::New                        \
        (                                                                      \
            IOobject                                                           \
            (                                                                  \
                #Func "(" + df1.name() + ')',                                  \
                df1.instance(),                                                \
                df1.db()                                                       \
            ),                                                                 \
            df1.mesh(),                                                        \
            Dfunc(df1.dimensions())                                            \
        );                                                                     \
                                                                               \
    Func(tres.ref().field(), df1.field());                                     \
    tres.ref().oriented() = Dfunc(df1.oriented());                             \
                                                                               \
    return tres;                                                               \
}                                                                              \
                                                                               \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> Func                                \
(                                                                              \
    const tmp<DimensionedField<Type1, GeoMesh>>& tdf1                          \
)                                                                              \
{                                                                              \
    const DimensionedField<Type1, GeoMesh>& df1 = tdf1();                      \
                                                                               \
    auto tres =                                                                \
        reuseTmpDimensionedField<ReturnType, Type1, GeoMesh>::New              \
        (                                                                      \
            tdf1,                                                              \
            #Func "(" + df1.name() + ')',                                      \
            Dfunc(df1.dimensions())                                            \
        );                                                                     \
                                                                               \
    Func(tres.ref().field(), df1.field());                                     \
    tres.ref().oriented() = Dfunc(df1.oriented());                             \
                                                                               \
    tdf1.clear();                                                              \
    return tres;                                                               \
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define UNARY_OPERATOR(ReturnType, Type1, Op, OpFunc, Dfunc)                   \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> operator Op                         \
(                                                                              \
    const DimensionedField<Type1, GeoMesh>& df1                                \
)                                                                              \
{                                                                              \
    auto tres =                                                                \
        tmp<DimensionedField<ReturnType, GeoMesh>>::New                        \
        (                                                                      \
            IOobject                                                           \
            (                                                                  \
                #Op + df1.name(),                                              \
                df1.instance(),                                                \
                df1.db()                                                       \
            ),                                                                 \
            df1.mesh(),                                                        \
            Dfunc(df1.dimensions())                                            \
        );                                                                     \
                                                                               \
    OpFunc(tres.ref().field(), df1.field());                             \
    tres.ref().oriented() = Dfunc(df1.oriented());                             \
                                                                               \
    return tres;                                                               \
}                                                                              \
                                                                               \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> operator Op                         \
(                                                                              \
    const tmp<DimensionedField<Type1, GeoMesh>>& tdf1                          \
)                                                                              \
{                                                                              \
    const DimensionedField<Type1, GeoMesh>& df1 = tdf1();                      \
                                                                               \
    auto tres =                                                                \
        reuseTmpDimensionedField<ReturnType, Type1, GeoMesh>::New              \
        (                                                                      \
            tdf1,                                                              \
            #Op + df1.name(),                                                  \
            Dfunc(df1.dimensions())                                            \
        );                                                                     \
                                                                               \
    OpFunc(tres.ref().field(), df1.field());                             \
    tres.ref().oriented() = Dfunc(df1.oriented());                             \
                                                                               \
    tdf1.clear();                                                              \
    return tres;                                                               \
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define BINARY_FUNCTION(ReturnType, Type1, Type2, Func)                        \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> Func                                \
(                                                                              \
    const DimensionedField<Type1, GeoMesh>& df1,                               \
    const DimensionedField<Type2, GeoMesh>& df2                                \
)                                                                              \
{                                                                              \
    auto tres =                                                                \
        tmp<DimensionedField<ReturnType, GeoMesh>>::New                        \
        (                                                                      \
            IOobject                                                           \
            (                                                                  \
                #Func "(" + df1.name() + ',' + df2.name() + ')',               \
                df1.instance(),                                                \
                df1.db()                                                       \
            ),                                                                 \
            df1.mesh(),                                                        \
            Func(df1.dimensions(), df2.dimensions())                           \
        );                                                                     \
                                                                               \
    Func(tres.ref().field(), df1.field(), df2.field());                        \
    tres.ref().oriented() = Func(df1.oriented(), df2.oriented());              \
                                                                               \
    return tres;                                                               \
}                                                                              \
                                                                               \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> Func                                \
(                                                                              \
    const DimensionedField<Type1, GeoMesh>& df1,                               \
    const tmp<DimensionedField<Type2, GeoMesh>>& tdf2                          \
)                                                                              \
{                                                                              \
    const DimensionedField<Type2, GeoMesh>& df2 = tdf2();                      \
                                                                               \
    auto tres =                                                                \
        reuseTmpDimensionedField<ReturnType, Type2, GeoMesh>::New              \
        (                                                                      \
            tdf2,                                                              \
            #Func "(" + df1.name() + ',' + df2.name() + ')',                   \
            Func(df1.dimensions(), df2.dimensions())                           \
        );                                                                     \
                                                                               \
    Func(tres.ref().field(), df1.field(), df2.field());                        \
    tres.ref().oriented() = Func(df1.oriented(), df2.oriented());              \
                                                                               \
    tdf2.clear();                                                              \
    return tres;                                                               \
}                                                                              \
                                                                               \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> Func                                \
(                                                                              \
    const tmp<DimensionedField<Type1, GeoMesh>>& tdf1,                         \
    const DimensionedField<Type2, GeoMesh>& df2                                \
)                                                                              \
{                                                                              \
    const DimensionedField<Type1, GeoMesh>& df1 = tdf1();                      \
                                                                               \
    tmp<DimensionedField<ReturnType, GeoMesh>> tres                            \
    (                                                                          \
        reuseTmpDimensionedField<ReturnType, Type1, GeoMesh>::New              \
        (                                                                      \
            tdf1,                                                              \
            #Func "(" + df1.name() + ',' + df2.name() + ')',                   \
            Func(df1.dimensions(), df2.dimensions())                           \
        )                                                                      \
    );                                                                         \
                                                                               \
    Func(tres.ref().field(), df1.field(), df2.field());                        \
    tres.ref().oriented() = Func(df1.oriented(), df2.oriented());              \
                                                                               \
    tdf1.clear();                                                              \
    return tres;                                                               \
}                                                                              \
                                                                               \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> Func                                \
(                                                                              \
    const tmp<DimensionedField<Type1, GeoMesh>>& tdf1,                         \
    const tmp<DimensionedField<Type2, GeoMesh>>& tdf2                          \
)                                                                              \
{                                                                              \
    const DimensionedField<Type1, GeoMesh>& df1 = tdf1();                      \
    const DimensionedField<Type2, GeoMesh>& df2 = tdf2();                      \
                                                                               \
    auto tres =                                                                \
        reuseTmpTmpDimensionedField                                            \
        <ReturnType, Type1, Type1, Type2, GeoMesh>::New                        \
        (                                                                      \
            tdf1,                                                              \
            tdf2,                                                              \
            #Func "(" + df1.name() + ',' + df2.name() + ')',                   \
            Func(df1.dimensions(), df2.dimensions())                           \
        );                                                                     \
                                                                               \
    Func(tres.ref().field(), df1.field(), df2.field());                        \
    tres.ref().oriented() = Func(df1.oriented(), df2.oriented());              \
                                                                               \
    tdf1.clear();                                                              \
    tdf2.clear();                                                              \
    return tres;                                                               \
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define BINARY_TYPE_FUNCTION_SF(ReturnType, Type1, Type2, Func)                \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> Func                                \
(                                                                              \
    const dimensioned<Type1>& dt1,                                             \
    const DimensionedField<Type2, GeoMesh>& df2                                \
)                                                                              \
{                                                                              \
    auto tres =                                                                \
        tmp<DimensionedField<ReturnType, GeoMesh>>::New                        \
        (                                                                      \
            IOobject                                                           \
            (                                                                  \
                #Func "(" + dt1.name() + ',' + df2.name() + ')',               \
                df2.instance(),                                                \
                df2.db()                                                       \
            ),                                                                 \
            df2.mesh(),                                                        \
            Func(dt1.dimensions(), df2.dimensions())                           \
        );                                                                     \
                                                                               \
    Func(tres.ref().field(), dt1.value(), df2.field());                        \
    tres.ref().oriented() = df2.oriented();                                    \
                                                                               \
    return tres;                                                               \
}                                                                              \
                                                                               \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> Func                                \
(                                                                              \
    const Type1& t1,                                                           \
    const DimensionedField<Type2, GeoMesh>& df2                                \
)                                                                              \
{                                                                              \
    return Func(dimensioned<Type1>(t1), df2);                                  \
}                                                                              \
                                                                               \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> Func                                \
(                                                                              \
    const dimensioned<Type1>& dt1,                                             \
    const tmp<DimensionedField<Type2, GeoMesh>>& tdf2                          \
)                                                                              \
{                                                                              \
    const DimensionedField<Type2, GeoMesh>& df2 = tdf2();                      \
                                                                               \
    auto tres =                                                                \
        reuseTmpDimensionedField<ReturnType, Type2, GeoMesh>::New              \
        (                                                                      \
            tdf2,                                                              \
            #Func "(" + dt1.name() + ',' + df2.name() + ')',                   \
            Func(dt1.dimensions(), df2.dimensions())                           \
        );                                                                     \
                                                                               \
    Func(tres.ref().field(), dt1.value(), df2.field());                        \
    tres.ref().oriented() = df2.oriented();                                    \
                                                                               \
    tdf2.clear();                                                              \
    return tres;                                                               \
}                                                                              \
                                                                               \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> Func                                \
(                                                                              \
    const Type1& t1,                                                           \
    const tmp<DimensionedField<Type2, GeoMesh>>& tdf2                          \
)                                                                              \
{                                                                              \
    return Func(dimensioned<Type2>(t1), tdf2);                                 \
}


#define BINARY_TYPE_FUNCTION_FS(ReturnType, Type1, Type2, Func)                \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> Func                                \
(                                                                              \
    const DimensionedField<Type1, GeoMesh>& df1,                               \
    const dimensioned<Type2>& dt2                                              \
)                                                                              \
{                                                                              \
    auto tres =                                                                \
        tmp<DimensionedField<ReturnType, GeoMesh>>::New                        \
        (                                                                      \
            IOobject                                                           \
            (                                                                  \
                #Func "(" + df1.name() + ',' + dt2.name() + ')',               \
                df1.instance(),                                                \
                df1.db()                                                       \
            ),                                                                 \
            df1.mesh(),                                                        \
            Func(df1.dimensions(), dt2.dimensions())                           \
        );                                                                     \
                                                                               \
    Func(tres.ref().field(), df1.field(), dt2.value());                        \
    tres.ref().oriented() = df1.oriented();                                    \
                                                                               \
    return tres;                                                               \
}                                                                              \
                                                                               \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> Func                                \
(                                                                              \
    const DimensionedField<Type1, GeoMesh>& df1,                               \
    const Type2& t2                                                            \
)                                                                              \
{                                                                              \
    return Func(df1, dimensioned<Type2>(t2));                                  \
}                                                                              \
                                                                               \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> Func                                \
(                                                                              \
    const tmp<DimensionedField<Type1, GeoMesh>>& tdf1,                         \
    const dimensioned<Type2>& dt2                                              \
)                                                                              \
{                                                                              \
    const DimensionedField<Type1, GeoMesh>& df1 = tdf1();                      \
                                                                               \
    auto tres =                                                                \
        reuseTmpDimensionedField<ReturnType, Type1, GeoMesh>::New              \
        (                                                                      \
            tdf1,                                                              \
            #Func "(" + df1.name() + ',' + dt2.name() + ')',                   \
            Func(df1.dimensions(), dt2.dimensions())                           \
        );                                                                     \
                                                                               \
    Func(tres.ref().field(), df1.field(), dt2.value());                        \
    tres.ref().oriented() = df1.oriented();                                    \
                                                                               \
    tdf1.clear();                                                              \
    return tres;                                                               \
}                                                                              \
                                                                               \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> Func                                \
(                                                                              \
    const tmp<DimensionedField<Type1, GeoMesh>>& tdf1,                         \
    const Type2& t2                                                            \
)                                                                              \
{                                                                              \
    return Func(tdf1, dimensioned<Type2>(t2));                                 \
}


#define BINARY_TYPE_FUNCTION(ReturnType, Type1, Type2, Func)                   \
    BINARY_TYPE_FUNCTION_SF(ReturnType, Type1, Type2, Func)                    \
    BINARY_TYPE_FUNCTION_FS(ReturnType, Type1, Type2, Func)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define BINARY_OPERATOR(ReturnType, Type1, Type2, Op, OpName, OpFunc)          \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> operator Op                         \
(                                                                              \
    const DimensionedField<Type1, GeoMesh>& df1,                               \
    const DimensionedField<Type2, GeoMesh>& df2                                \
)                                                                              \
{                                                                              \
    auto tres =                                                                \
        tmp<DimensionedField<ReturnType, GeoMesh>>::New                        \
        (                                                                      \
            IOobject                                                           \
            (                                                                  \
                '(' + df1.name() + OpName + df2.name() + ')',                  \
                df1.instance(),                                                \
                df1.db()                                                       \
            ),                                                                 \
            df1.mesh(),                                                        \
            df1.dimensions() Op df2.dimensions()                               \
        );                                                                     \
                                                                               \
    OpFunc(tres.ref().field(), df1.field(), df2.field());                \
    tres.ref().oriented() = df1.oriented() Op df2.oriented();                  \
                                                                               \
    return tres;                                                               \
}                                                                              \
                                                                               \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> operator Op                         \
(                                                                              \
    const DimensionedField<Type1, GeoMesh>& df1,                               \
    const tmp<DimensionedField<Type2, GeoMesh>>& tdf2                          \
)                                                                              \
{                                                                              \
    const DimensionedField<Type2, GeoMesh>& df2 = tdf2();                      \
                                                                               \
    auto tres =                                                                \
        reuseTmpDimensionedField<ReturnType, Type2, GeoMesh>::New              \
        (                                                                      \
            tdf2,                                                              \
            '(' + df1.name() + OpName + df2.name() + ')',                      \
            df1.dimensions() Op df2.dimensions()                               \
        );                                                                     \
                                                                               \
    OpFunc(tres.ref().field(), df1.field(), df2.field());                \
    tres.ref().oriented() = df1.oriented() Op df2.oriented();                  \
                                                                               \
    tdf2.clear();                                                              \
    return tres;                                                               \
}                                                                              \
                                                                               \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> operator Op                         \
(                                                                              \
    const tmp<DimensionedField<Type1, GeoMesh>>& tdf1,                         \
    const DimensionedField<Type2, GeoMesh>& df2                                \
)                                                                              \
{                                                                              \
    const DimensionedField<Type1, GeoMesh>& df1 = tdf1();                      \
                                                                               \
    auto tres =                                                                \
        reuseTmpDimensionedField<ReturnType, Type1, GeoMesh>::New              \
        (                                                                      \
            tdf1,                                                              \
            '(' + df1.name() + OpName + df2.name() + ')',                      \
            df1.dimensions() Op df2.dimensions()                               \
        );                                                                     \
                                                                               \
    OpFunc(tres.ref().field(), df1.field(), df2.field());                \
    tres.ref().oriented() = df1.oriented() Op df2.oriented();                  \
                                                                               \
    tdf1.clear();                                                              \
    return tres;                                                               \
}                                                                              \
                                                                               \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> operator Op                         \
(                                                                              \
    const tmp<DimensionedField<Type1, GeoMesh>>& tdf1,                         \
    const tmp<DimensionedField<Type2, GeoMesh>>& tdf2                          \
)                                                                              \
{                                                                              \
    const DimensionedField<Type1, GeoMesh>& df1 = tdf1();                      \
    const DimensionedField<Type2, GeoMesh>& df2 = tdf2();                      \
                                                                               \
    auto tres =                                                                \
        reuseTmpTmpDimensionedField                                            \
        <ReturnType, Type1, Type1, Type2, GeoMesh>::New                        \
        (                                                                      \
            tdf1,                                                              \
            tdf2,                                                              \
            '(' + df1.name() + OpName + df2.name() + ')',                      \
            df1.dimensions() Op df2.dimensions()                               \
        );                                                                     \
                                                                               \
    OpFunc(tres.ref().field(), df1.field(), df2.field());                \
    tres.ref().oriented() = df1.oriented() Op df2.oriented();                  \
                                                                               \
    tdf1.clear();                                                              \
    tdf2.clear();                                                              \
    return tres;                                                               \
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define BINARY_TYPE_OPERATOR_SF(ReturnType, Type1, Type2, Op, OpName, OpFunc)  \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> operator Op                         \
(                                                                              \
    const dimensioned<Type1>& dt1,                                             \
    const DimensionedField<Type2, GeoMesh>& df2                                \
)                                                                              \
{                                                                              \
    auto tres =                                                                \
        tmp<DimensionedField<ReturnType, GeoMesh>>::New                        \
        (                                                                      \
            IOobject                                                           \
            (                                                                  \
                '(' + dt1.name() + OpName + df2.name() + ')',                  \
                df2.instance(),                                                \
                df2.db()                                                       \
            ),                                                                 \
            df2.mesh(),                                                        \
            dt1.dimensions() Op df2.dimensions()                               \
        );                                                                     \
                                                                               \
    tres.ref().oriented() = df2.oriented();                                    \
                                                                               \
    OpFunc(tres.ref().field(), dt1.value(), df2.field());                \
                                                                               \
    return tres;                                                               \
}                                                                              \
                                                                               \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> operator Op                         \
(                                                                              \
    const Type1& t1,                                                           \
    const DimensionedField<Type2, GeoMesh>& df2                                \
)                                                                              \
{                                                                              \
    return dimensioned<Type1>(t1) Op df2;                                      \
}                                                                              \
                                                                               \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> operator Op                         \
(                                                                              \
    const dimensioned<Type1>& dt1,                                             \
    const tmp<DimensionedField<Type2, GeoMesh>>& tdf2                          \
)                                                                              \
{                                                                              \
    const DimensionedField<Type2, GeoMesh>& df2 = tdf2();                      \
                                                                               \
    auto tres =                                                                \
        reuseTmpDimensionedField<ReturnType, Type2, GeoMesh>::New              \
        (                                                                      \
            tdf2,                                                              \
            '(' + dt1.name() + OpName + df2.name() + ')',                      \
            dt1.dimensions() Op df2.dimensions()                               \
        );                                                                     \
                                                                               \
    OpFunc(tres.ref().field(), dt1.value(), tdf2().field());             \
    tres.ref().oriented() = df2.oriented();                                    \
                                                                               \
    tdf2.clear();                                                              \
    return tres;                                                               \
}                                                                              \
                                                                               \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> operator Op                         \
(                                                                              \
    const Type1& t1,                                                           \
    const tmp<DimensionedField<Type2, GeoMesh>>& tdf2                          \
)                                                                              \
{                                                                              \
    return dimensioned<Type1>(t1) Op tdf2;                                     \
}


#define BINARY_TYPE_OPERATOR_FS(ReturnType, Type1, Type2, Op, OpName, OpFunc)  \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> operator Op                         \
(                                                                              \
    const DimensionedField<Type1, GeoMesh>& df1,                               \
    const dimensioned<Type2>& dt2                                              \
)                                                                              \
{                                                                              \
    auto tres =                                                                \
        tmp<DimensionedField<ReturnType, GeoMesh>>::New                        \
        (                                                                      \
            IOobject                                                           \
            (                                                                  \
                '(' + df1.name() + OpName + dt2.name() + ')',                  \
                df1.instance(),                                                \
                df1.db()                                                       \
            ),                                                                 \
            df1.mesh(),                                                        \
            df1.dimensions() Op dt2.dimensions()                               \
        );                                                                     \
                                                                               \
    OpFunc(tres.ref().field(), df1.field(), dt2.value());                \
    tres.ref().oriented() = df1.oriented();                                    \
                                                                               \
    return tres;                                                               \
}                                                                              \
                                                                               \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> operator Op                         \
(                                                                              \
    const DimensionedField<Type1, GeoMesh>& df1,                               \
    const Type2& t2                                                            \
)                                                                              \
{                                                                              \
    return df1 Op dimensioned<Type2>(t2);                                      \
}                                                                              \
                                                                               \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> operator Op                         \
(                                                                              \
    const tmp<DimensionedField<Type1, GeoMesh>>& tdf1,                         \
    const dimensioned<Type2>& dt2                                              \
)                                                                              \
{                                                                              \
    const DimensionedField<Type1, GeoMesh>& df1 = tdf1();                      \
                                                                               \
    auto tres =                                                                \
        reuseTmpDimensionedField<ReturnType, Type1, GeoMesh>::New              \
        (                                                                      \
            tdf1,                                                              \
            '(' + df1.name() + OpName + dt2.name() + ')',                      \
            df1.dimensions() Op dt2.dimensions()                               \
        );                                                                     \
                                                                               \
    OpFunc(tres.ref().field(), tdf1().field(), dt2.value());             \
    tres.ref().oriented() = df1.oriented();                                    \
                                                                               \
    tdf1.clear();                                                              \
    return tres;                                                               \
}                                                                              \
                                                                               \
                                                                               \
TEMPLATE                                                                       \
tmp<DimensionedField<ReturnType, GeoMesh>> operator Op                         \
(                                                                              \
    const tmp<DimensionedField<Type1, GeoMesh>>& tdf1,                         \
    const Type2& t2                                                            \
)                                                                              \
{                                                                              \
    return tdf1 Op dimensioned<Type2>(t2);                                     \
}

#define BINARY_TYPE_OPERATOR(ReturnType, Type1, Type2, Op, OpName, OpFunc)     \
    BINARY_TYPE_OPERATOR_SF(ReturnType, Type1, Type2, Op, OpName, OpFunc)      \
    BINARY_TYPE_OPERATOR_FS(ReturnType, Type1, Type2, Op, OpName, OpFunc)


// ************************************************************************* //

 