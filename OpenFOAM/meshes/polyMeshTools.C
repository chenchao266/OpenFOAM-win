﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2012-2016 OpenFOAM Foundation
    Copyright (C) 2021 OpenCFD Ltd.
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

#include "polyMeshTools.H"
#include "syncTools.H"
#include "pyramidPointFaceRef.H"
#include "primitiveMeshTools.H"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


 namespace Foam{
tmp<scalarField> polyMeshTools::faceOrthogonality
(
    const polyMesh& mesh,
    const vectorField& areas,
    const vectorField& cc
)
{
    const labelList& own = mesh.faceOwner();
    const labelList& nei = mesh.faceNeighbour();
    const polyBoundaryMesh& pbm = mesh.boundaryMesh();

    tmp<scalarField> tortho(new scalarField(mesh.nFaces(), 1.0));
    scalarField& ortho = tortho.ref();

    // Internal faces
    forAll(nei, facei)
    {
        ortho[facei] = primitiveMeshTools::faceOrthogonality
        (
            cc[own[facei]],
            cc[nei[facei]],
            areas[facei]
        );
    }


    // Coupled faces

    pointField neighbourCc;
    syncTools::swapBoundaryCellPositions(mesh, cc, neighbourCc);

    forAll(pbm, patchi)
    {
        const polyPatch& pp = pbm[patchi];
        if (pp.coupled())
        {
            forAll(pp, i)
            {
                label facei = pp.start() + i;
                label bFacei = facei - mesh.nInternalFaces();

                ortho[facei] = primitiveMeshTools::faceOrthogonality
                (
                    cc[own[facei]],
                    neighbourCc[bFacei],
                    areas[facei]
                );
            }
        }
    }

    return tortho;
}


tmp<scalarField> polyMeshTools::faceSkewness
(
    const polyMesh& mesh,
    const pointField& p,
    const vectorField& fCtrs,
    const vectorField& fAreas,
    const vectorField& cellCtrs
)
{
    const labelList& own = mesh.faceOwner();
    const labelList& nei = mesh.faceNeighbour();
    const polyBoundaryMesh& pbm = mesh.boundaryMesh();

    tmp<scalarField> tskew(new scalarField(mesh.nFaces()));
    scalarField& skew = tskew.ref();

    forAll(nei, facei)
    {
        skew[facei] = primitiveMeshTools::faceSkewness
        (
            mesh,
            p,
            fCtrs,
            fAreas,

            facei,
            cellCtrs[own[facei]],
            cellCtrs[nei[facei]]
        );
    }


    // Boundary faces: consider them to have only skewness error.
    // (i.e. treat as if mirror cell on other side)

    pointField neighbourCc;
    syncTools::swapBoundaryCellPositions(mesh, cellCtrs, neighbourCc);

    forAll(pbm, patchi)
    {
        const polyPatch& pp = pbm[patchi];
        if (pp.coupled())
        {
            forAll(pp, i)
            {
                label facei = pp.start() + i;
                label bFacei = facei - mesh.nInternalFaces();

                skew[facei] = primitiveMeshTools::faceSkewness
                (
                    mesh,
                    p,
                    fCtrs,
                    fAreas,

                    facei,
                    cellCtrs[own[facei]],
                    neighbourCc[bFacei]
                );
            }
        }
        else
        {
            forAll(pp, i)
            {
                label facei = pp.start() + i;

                skew[facei] = primitiveMeshTools::boundaryFaceSkewness
                (
                    mesh,
                    p,
                    fCtrs,
                    fAreas,

                    facei,
                    cellCtrs[own[facei]]
                );
            }
        }
    }

    return tskew;
}


tmp<scalarField> polyMeshTools::faceWeights
(
    const polyMesh& mesh,
    const vectorField& fCtrs,
    const vectorField& fAreas,
    const vectorField& cellCtrs
)
{
    const labelList& own = mesh.faceOwner();
    const labelList& nei = mesh.faceNeighbour();
    const polyBoundaryMesh& pbm = mesh.boundaryMesh();

    tmp<scalarField> tweight(new scalarField(mesh.nFaces(), 1.0));
    scalarField& weight = tweight.ref();

    // Internal faces
    forAll(nei, facei)
    {
        const point& fc = fCtrs[facei];
        const vector& fa = fAreas[facei];

        scalar dOwn = mag(fa & (fc-cellCtrs[own[facei]]));
        scalar dNei = mag(fa & (cellCtrs[nei[facei]]-fc));

        weight[facei] = min(dNei,dOwn)/(dNei+dOwn+VSMALL);
    }


    // Coupled faces

    pointField neiCc;
    syncTools::swapBoundaryCellPositions(mesh, cellCtrs, neiCc);

    forAll(pbm, patchi)
    {
        const polyPatch& pp = pbm[patchi];
        if (pp.coupled())
        {
            forAll(pp, i)
            {
                label facei = pp.start() + i;
                label bFacei = facei - mesh.nInternalFaces();

                const point& fc = fCtrs[facei];
                const vector& fa = fAreas[facei];

                scalar dOwn = mag(fa & (fc-cellCtrs[own[facei]]));
                scalar dNei = mag(fa & (neiCc[bFacei]-fc));

                weight[facei] = min(dNei,dOwn)/(dNei+dOwn+VSMALL);
            }
        }
    }

    return tweight;
}


tmp<scalarField> polyMeshTools::volRatio
(
    const polyMesh& mesh,
    const scalarField& vol
)
{
    const labelList& own = mesh.faceOwner();
    const labelList& nei = mesh.faceNeighbour();
    const polyBoundaryMesh& pbm = mesh.boundaryMesh();

    tmp<scalarField> tratio(new scalarField(mesh.nFaces(), 1.0));
    scalarField& ratio = tratio.ref();

    // Internal faces
    forAll(nei, facei)
    {
        scalar volOwn = vol[own[facei]];
        scalar volNei = vol[nei[facei]];

        ratio[facei] = min(volOwn,volNei)/(max(volOwn, volNei)+VSMALL);
    }


    // Coupled faces

    scalarField neiVol;
    syncTools::swapBoundaryCellList(mesh, vol, neiVol);

    forAll(pbm, patchi)
    {
        const polyPatch& pp = pbm[patchi];
        if (pp.coupled())
        {
            forAll(pp, i)
            {
                label facei = pp.start() + i;
                label bFacei = facei - mesh.nInternalFaces();

                scalar volOwn = vol[own[facei]];
                scalar volNei = neiVol[bFacei];

                ratio[facei] = min(volOwn,volNei)/(max(volOwn, volNei)+VSMALL);
            }
        }
    }

    return tratio;
}


polyMesh::readUpdateState polyMeshTools::combine
(
    const polyMesh::readUpdateState& state0,
    const polyMesh::readUpdateState& state1
)
{
    if
    (
        (
            state0 == polyMesh::UNCHANGED
         && state1 != polyMesh::UNCHANGED
        )
     || (
            state0 == polyMesh::POINTS_MOVED
         && (state1 != polyMesh::UNCHANGED && state1 != polyMesh::POINTS_MOVED)
        )
     || (
            state0 == polyMesh::TOPO_CHANGE
         && state1 == polyMesh::TOPO_PATCH_CHANGE
        )
    )
    {
        return state1;
    }
    else
    {
        return state0;
    }
}


// ************************************************************************* //

 } // End namespace Foam
