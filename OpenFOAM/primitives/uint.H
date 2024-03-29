﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2020 OpenCFD Ltd.
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

Primitive
    uint

Description
    System unsigned integer

SourceFiles
    uintIO.C

\*---------------------------------------------------------------------------*/

#ifndef uint_H
#define uint_H

#include "uint8.H"
#include "uint16.H"
#include "uint32.H"
#include "uint64.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

#define MAXMIN(retType, type1, type2)                                          \
                                                                               \
/*! \brief Floating/integral min. Use stdmin() to preserve references */ \
inline retType min(const type1 s1, const type2 s2)                             \
{                                                                              \
    return (s1 < s2) ? s1 : s2;                                                \
}                                                                              \
                                                                               \
/*! \brief Floating/integral max. Use stdmax() to preserve references */ \
inline retType max(const type1 s1, const type2 s2)                             \
{                                                                              \
    return (s2 < s1) ? s1 : s2;                                                \
}

MAXMIN(uint8_t, uint8_t, uint8_t)
MAXMIN(uint16_t, uint16_t, uint16_t)

MAXMIN(uint32_t, uint32_t, uint32_t)
MAXMIN(uint64_t, uint64_t, uint32_t)
MAXMIN(uint64_t, uint32_t, uint64_t)
MAXMIN(uint64_t, uint64_t, uint64_t)


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

//- Read unsigned int from stream
unsigned int readUint(Istream& is);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
