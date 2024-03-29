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

Description
    Calculate the face centres and areas.

    Calculate the centre by breaking the face into triangles using the face
    centre and area-weighted averaging their centres.  This method copes with
    small face-concavity.

\*---------------------------------------------------------------------------*/

#include "primitiveMesh.H"
#include "primitiveMeshTools.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


 namespace Foam{
void primitiveMesh::calcFaceCentresAndAreas() const
{
    if (debug)
    {
        Pout<< "primitiveMesh::calcFaceCentresAndAreas() : "
            << "Calculating face centres and face areas"
            << endl;
    }

    // It is an error to attempt to recalculate faceCentres
    // if the pointer is already set
    if (faceCentresPtr_ || faceAreasPtr_)
    {
        FatalErrorInFunction
            << "Face centres or face areas already calculated"
            << abort(FatalError);
    }

    faceCentresPtr_ = new vectorField(nFaces());
    vectorField& fCtrs = *faceCentresPtr_;

    faceAreasPtr_ = new vectorField(nFaces());
    vectorField& fAreas = *faceAreasPtr_;

    primitiveMeshTools::makeFaceCentresAndAreas(*this, points(), fCtrs, fAreas);

    if (debug)
    {
        Pout<< "primitiveMesh::calcFaceCentresAndAreas() : "
            << "Finished calculating face centres and face areas"
            << endl;
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const vectorField& primitiveMesh::faceCentres() const
{
    if (!faceCentresPtr_)
    {
        //calcFaceCentresAndAreas();
        const_cast<primitiveMesh&>(*this).updateGeom();
    }

    return *faceCentresPtr_;
}


const vectorField& primitiveMesh::faceAreas() const
{
    if (!faceAreasPtr_)
    {
        //calcFaceCentresAndAreas();
        const_cast<primitiveMesh&>(*this).updateGeom();
    }

    return *faceAreasPtr_;
}


// ************************************************************************* //

 } // End namespace Foam
