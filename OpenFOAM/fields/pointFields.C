﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
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

#include "polyMesh.H"
#include "pointFields.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTemplate2TypeNameAndDebug(pointScalarField::Internal, 0);
    defineTemplate2TypeNameAndDebug(pointVectorField::Internal, 0);
    defineTemplate2TypeNameAndDebug
    (
        pointSphericalTensorField::Internal,
        0
    );
    defineTemplate2TypeNameAndDebug
    (
        pointSymmTensorField::Internal,
        0
    );
    defineTemplate2TypeNameAndDebug(pointTensorField::Internal, 0);


    defineTemplateTypeNameAndDebug(pointScalarField, 0);
    defineTemplateTypeNameAndDebug(pointVectorField, 0);
    defineTemplateTypeNameAndDebug(pointSphericalTensorField, 0);
    defineTemplateTypeNameAndDebug(pointSymmTensorField, 0);
    defineTemplateTypeNameAndDebug(pointTensorField, 0);

} // End namespace Foam


// * * * * * * * * * * * * * * * * Global Data * * * * * * * * * * * * * * * //

// Note hard-coded values are more reliable than other alternatives


 namespace Foam{
const wordList fieldTypes::point
({
    "pointScalarField",
    "pointVectorField",
    "pointSphericalTensorField",
    "pointSymmTensorField",
    "pointTensorField"
});


// ************************************************************************* //

 } // End namespace Foam
