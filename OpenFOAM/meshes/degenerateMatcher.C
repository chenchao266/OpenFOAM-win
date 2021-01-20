/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2016 OpenFOAM Foundation
     \\/     M anipulation  |
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
#include "ListOps.T.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //
using namespace Foam;
hexMatcher degenerateMatcher::hex;
wedgeMatcher degenerateMatcher::wedge;
prismMatcher degenerateMatcher::prism;
tetWedgeMatcher degenerateMatcher::tetWedge;
pyrMatcher degenerateMatcher::pyr;
tetMatcher degenerateMatcher::tet;


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
    else if (tet.matchShape(false, faces, owner, celli, cellFaces))
    {
        return cellShape(tet.model(), tet.vertLabels());
    }
    else if (prism.matchShape(false, faces, owner, celli, cellFaces))
    {
        return cellShape(prism.model(), prism.vertLabels());
    }
    else if (pyr.matchShape(false, faces, owner, celli, cellFaces))
    {
        return cellShape(pyr.model(), pyr.vertLabels());
    }
    else if (wedge.matchShape(false, faces, owner, celli, cellFaces))
    {
        return cellShape(wedge.model(), wedge.vertLabels());
    }
    else if (tetWedge.matchShape(false, faces, owner, celli, cellFaces))
    {
        return cellShape(tetWedge.model(), tetWedge.vertLabels());
    }
    else
    {
        return cellShape(*(cellModeller::lookup(0)), labelList(0));
    }
}


cellShape degenerateMatcher::match(const faceList& faces)
{
    // Do as if single cell mesh; all faces are referenced by a single cell

    return match
    (
        faces,
        labelList(faces.size(), 0),    // cell 0 is owner of all faces
        0,                             // cell 0
        identity(faces.size())         // cell 0 consists of all faces
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
