/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2013-2015 OpenFOAM Foundation
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

Class
    ensightPTraits

Description
    Ensight names and component order for base types.

    For the purpose of traits, integers (label) are treated like
    floating point (scalar). Spherical tensors are mapped as a scalar.

\*---------------------------------------------------------------------------*/

#ifndef ensightPTraits_H
#define ensightPTraits_H

#include "fieldTypes.H"
#include "direction.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                         Class ensightPTraits Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
struct ensightPTraits
{
    //- The type name used in ensight case files
    static const char* const typeName;

    //- Ordering table: return OpenFOAM component given Ensight component
    //  This is used for the symmTensor ordering: Ensight requires
    //  xx yy zz xy xz yz
    static const direction componentOrder[];
};


// Specializations

template<>
const char* const
ensightPTraits<label>::typeName = "scalar";

template<>
const direction
ensightPTraits<label>::componentOrder[] = { 0 };


template<>
const char* const
ensightPTraits<scalar>::typeName = "scalar";

template<>
const direction
ensightPTraits<scalar>::componentOrder[] = { 0 };


template<>
const char* const
ensightPTraits<vector>::typeName = "vector";

template<>
const direction
ensightPTraits<vector>::componentOrder[] = { 0, 1, 2 };


// Use mag(sphericalTensor) instead
template<>
const char* const
ensightPTraits<sphericalTensor>::typeName = "scalar";

template<>
const direction
ensightPTraits<sphericalTensor>::componentOrder[] = { 0 };


template<>
const char* const
ensightPTraits<symmTensor>::typeName = "tensor symm";

template<>
const direction
ensightPTraits<symmTensor>::componentOrder[] = { 0, 3, 5, 1, 2, 4 };


template<>
const char* const
ensightPTraits<tensor>::typeName = "tensor asym";

template<>
const direction
ensightPTraits<tensor>::componentOrder[] =
{ 0, 1, 2, 3, 4, 5, 6, 7, 8 };

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
