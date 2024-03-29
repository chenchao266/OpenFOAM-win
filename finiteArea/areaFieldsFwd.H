/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016-2017 Wikki Ltd
    Copyright (C) 2018 OpenCFD Ltd.
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
    Forwards and collection of common area field types

SourceFiles
    areaFields.C

\*---------------------------------------------------------------------------*/

#ifndef areaFieldsFwd_H
#define areaFieldsFwd_H

#include "fieldTypes.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Forward Declarations
class areaMesh;

template<class Type> class faPatchField;

template<class Type, template<class> class PatchField, class GeoMesh>
class GeometricField;

// Typedefs

typedef GeometricField<scalar, faPatchField, areaMesh> areaScalarField;
typedef GeometricField<vector, faPatchField, areaMesh> areaVectorField;
typedef GeometricField<sphericalTensor, faPatchField, areaMesh>
    areaSphericalTensorField;
typedef GeometricField<symmTensor, faPatchField, areaMesh> areaSymmTensorField;
typedef GeometricField<tensor, faPatchField, areaMesh> areaTensorField;


/*---------------------------------------------------------------------------*\
                       Namespace fieldTypes Declaration
\*---------------------------------------------------------------------------*/

namespace fieldTypes
{
    //- Standard area field types (scalar, vector, tensor, etc)
    extern const wordList area;

} // End namespace fieldTypes


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
