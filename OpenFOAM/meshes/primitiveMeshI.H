/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011 OpenFOAM Foundation
    Copyright (C) 2018-2021 OpenCFD Ltd.
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

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


 namespace Foam{
inline label primitiveMesh::nInternalPoints() const noexcept
{
    return nInternalPoints_;
}


inline label primitiveMesh::nPoints() const noexcept
{
    return nPoints_;
}


inline label primitiveMesh::nInternal0Edges() const
{
    // Force edge calculation
    (void)nEdges();
    return nInternal0Edges_;
}


inline label primitiveMesh::nInternal1Edges() const
{
    // Force edge calculation
    (void)nEdges();
    return nInternal1Edges_;
}


inline label primitiveMesh::nInternalEdges() const
{
    // Force edge calculation
    (void)nEdges();
    return nInternalEdges_;
}


inline label primitiveMesh::nEdges() const
{
    if (nEdges_ < 0)
    {
        nEdges_ = edges().size();
    }

    return nEdges_;
}


inline label primitiveMesh::nInternalFaces() const noexcept
{
    return nInternalFaces_;
}


inline label primitiveMesh::nBoundaryFaces() const noexcept
{
    return (nFaces_ - nInternalFaces_);
}


inline label primitiveMesh::nFaces() const noexcept
{
    return nFaces_;
}


inline label primitiveMesh::nCells() const noexcept
{
    return nCells_;
}


inline bool primitiveMesh::isInternalFace
(
    const label faceIndex
) const noexcept
{
    return faceIndex < nInternalFaces_;
}


inline bool primitiveMesh::hasCellShapes() const noexcept
{
    return cellShapesPtr_;
}


inline bool primitiveMesh::hasEdges() const noexcept
{
    return edgesPtr_;
}


inline bool primitiveMesh::hasCellCells() const noexcept
{
    return ccPtr_;
}


inline bool primitiveMesh::hasEdgeCells() const noexcept
{
    return ecPtr_;
}


inline bool primitiveMesh::hasPointCells() const noexcept
{
    return pcPtr_;
}


inline bool primitiveMesh::hasCells() const noexcept
{
    return cfPtr_;
}


inline bool primitiveMesh::hasEdgeFaces() const noexcept
{
    return efPtr_;
}


inline bool primitiveMesh::hasPointFaces() const noexcept
{
    return pfPtr_;
}


inline bool primitiveMesh::hasCellEdges() const noexcept
{
    return cePtr_;
}


inline bool primitiveMesh::hasFaceEdges() const noexcept
{
    return fePtr_;
}


inline bool primitiveMesh::hasPointEdges() const noexcept
{
    return pePtr_;
}


inline bool primitiveMesh::hasPointPoints() const noexcept
{
    return ppPtr_;
}


inline bool primitiveMesh::hasCellPoints() const noexcept
{
    return cpPtr_;
}


inline bool primitiveMesh::hasCellCentres() const noexcept
{
    return cellCentresPtr_;
}


inline bool primitiveMesh::hasFaceCentres() const noexcept
{
    return faceCentresPtr_;
}


inline bool primitiveMesh::hasCellVolumes() const noexcept
{
    return cellVolumesPtr_;
}


inline bool primitiveMesh::hasFaceAreas() const noexcept
{
    return faceAreasPtr_;
}


// ************************************************************************* //

 } // End namespace Foam
