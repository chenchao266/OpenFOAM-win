/*---------------------------------------------------------------------------*\
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

\*---------------------------------------------------------------------------*/

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


 namespace Foam{
inline const word& cellModel::name() const noexcept
{
    return name_;
}


inline label cellModel::index() const noexcept
{
    return index_;
}


inline label cellModel::nPoints() const noexcept
{
    return nPoints_;
}


inline label cellModel::nEdges() const noexcept
{
    return edges_.size();
}


inline label cellModel::nFaces() const noexcept
{
    return faces_.size();
}


inline const edgeList& cellModel::modelEdges() const noexcept
{
    return edges_;
}


inline const faceList& cellModel::modelFaces() const noexcept
{
    return faces_;
}


inline edgeList cellModel::edges
(
    const labelUList& pointLabels
) const
{
    edgeList theEdges(edges_.size());

    forAll(edges_, edgei)
    {
        // From model labels to global labels
        theEdges[edgei] = ::Foam::edge(pointLabels, edges_[edgei]);
    }

    return theEdges;
}


inline ::Foam::edge cellModel::edge
(
    const label modelEdgei,
    const labelUList& pointLabels
) const
{
    // From model labels to global labels
    return ::Foam::edge(pointLabels, edges_[modelEdgei]);
}


inline faceList cellModel::faces
(
    const labelUList& pointLabels
) const
{
    faceList theFaces(faces_.size());

    forAll(faces_, facei)
    {
        // From model labels to global labels
        theFaces[facei] = ::Foam::face(pointLabels, faces_[facei]);
    }

    return theFaces;
}


inline ::Foam::face cellModel::face
(
    const label modelFacei,
    const labelUList& pointLabels
) const
{
    // From model labels to global labels
    return ::Foam::face(pointLabels, faces_[modelFacei]);
}


// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

inline bool operator==
(
    const cellModel& lhs,
    const cellModel& rhs
) noexcept
{
    return (&lhs == &rhs);
}


inline bool operator!=
(
    const cellModel& lhs,
    const cellModel& rhs
) noexcept
{
    return (&lhs != &rhs);
}


// ************************************************************************* //

 } // End namespace Foam
