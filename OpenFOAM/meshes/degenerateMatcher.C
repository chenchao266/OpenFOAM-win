/*---------------------------------------------------------------------------*\
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

#include "degenerateMatcher.H"
#include "primitiveMesh.H"
#include "ListOps.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


 namespace Foam{
hexMatcher degenerateMatcher::hex;
wedgeMatcher degenerateMatcher::wedge;
prismMatcher degenerateMatcher::prism;
tetWedgeMatcher degenerateMatcher::tetWedge;
pyrMatcher degenerateMatcher::pyr;
tetMatcher degenerateMatcher::tet;


// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

cellShape degenerateMatcher::match
(
    const faceList& faces,
    const labelList& owner,
    const label celli,
    const labelList& cellFaces
)
{
    // Recognize in order of assumed occurrence.

    if (hex.matchShape(false, faces, owner, celli, cellFaces))
    {
        return cellShape(hex.model(), hex.vertLabels());
    }
    if (tet.matchShape(false, faces, owner, celli, cellFaces))
    {
        return cellShape(tet.model(), tet.vertLabels());
    }
    if (prism.matchShape(false, faces, owner, celli, cellFaces))
    {
        return cellShape(prism.model(), prism.vertLabels());
    }
    if (pyr.matchShape(false, faces, owner, celli, cellFaces))
    {
        return cellShape(pyr.model(), pyr.vertLabels());
    }
    if (wedge.matchShape(false, faces, owner, celli, cellFaces))
    {
        return cellShape(wedge.model(), wedge.vertLabels());
    }
    if (tetWedge.matchShape(false, faces, owner, celli, cellFaces))
    {
        return cellShape(tetWedge.model(), tetWedge.vertLabels());
    }

    return cellShape(cellModel::ref(cellModel::UNKNOWN), labelList());
}


cellShape degenerateMatcher::match(const faceList& faces)
{
    // Do as if single cell mesh; all faces are referenced by a single cell

    return match
    (
        faces,
        labelList(faces.size(), Zero),  // cell 0 is owner of all faces
        0,                              // cell 0
        identity(faces.size())          // cell 0 consists of all faces
    );
}


cellShape degenerateMatcher::match(const cellShape& shape)
{
    return match(shape.collapsedFaces());
}


cellShape degenerateMatcher::match
(
    const primitiveMesh& mesh,
    const label celli
)
{
    return match
    (
        mesh.faces(),
        mesh.faceOwner(),
        celli,
        mesh.cells()[celli]
    );
}


// ************************************************************************* //

 } // End namespace Foam
