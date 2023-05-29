/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011 OpenFOAM Foundation
    Copyright (C) 2017-2021 OpenCFD Ltd.
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

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


 namespace Foam{
inline face::face(const label sz)
:
    labelList(sz, -1)
{}


inline face::face(const labelUList& list)
:
    labelList(list)
{}


inline face::face(labelList&& list)
:
    labelList(std::move(list))
{}


inline face::face(std::initializer_list<label> list)
:
    labelList(list)
{}


template<unsigned N>
inline face::face(const FixedList<label, N>& list)
:
    labelList(list)
{}


inline face::face(const labelUList& list, const labelUList& indices)
:
    labelList(list, indices)
{}


template<unsigned N>
inline face::face
(
    const labelUList& list,
    const FixedList<label, N>& indices
)
:
    labelList(list, indices)
{}


inline face::face(Istream& is)
:
    labelList(is)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline pointField face::points(const UList<point>& pts) const
{
    // There are as many points as there are labels for them
    pointField p(size());

    auto iter = p.begin();

    for (const label pointi : *this)
    {
        *iter = pts[pointi];
        ++iter;
    }

    return p;
}


inline vector face::unitNormal(const UList<point>& p) const
{
    const vector n(areaNormal(p));
    const scalar s(::Foam::mag(n));
    return s < ROOTVSMALL ? Zero : n/s;
}


inline scalar face::mag(const UList<point>& p) const
{
    return ::Foam::mag(areaNormal(p));
}


inline label face::nEdges() const noexcept
{
    // for a closed polygon a number of edges is the same as number of points
    return size();
}


inline ::Foam::edge face::edge(const label edgei) const
{
    return ::Foam::edge(thisLabel(edgei), nextLabel(edgei));
}


inline vector face::edge
(
    const label edgei,
    const UList<point>& pts
) const
{
    return vector(pts[nextLabel(edgei)] - pts[thisLabel(edgei)]);
}


inline ::Foam::edge face::rcEdge(const label edgei) const
{
    // Edge 0 (forward and reverse) always starts at [0]
    // for consistency with face flipping
    const label pointi = edgei ? (nEdges() - edgei) : 0;
    return ::Foam::edge(thisLabel(pointi), prevLabel(pointi));
}


inline vector face::rcEdge
(
    const label edgei,
    const UList<point>& pts
) const
{
    // Edge 0 (forward and reverse) always starts at [0]
    // for consistency with face flipping
    const label pointi = edgei ? (nEdges() - edgei) : 0;
    return vector(pts[prevLabel(pointi)] - pts[thisLabel(pointi)]);
}


inline label face::which(const label pointLabel) const
{
    return labelList::find(pointLabel);
}


inline label face::thisLabel(const label i) const
{
    return labelList::operator[](i);
}


inline label face::nextLabel(const label i) const
{
    return labelList::fcValue(i);
}


inline label face::prevLabel(const label i) const
{
    return labelList::rcValue(i);
}


inline label face::nTriangles() const
{
    return size() - 2;
}


// * * * * * * * * * * * * * * * Global Operators   * * * * * * * * * * * * * //

inline bool operator==(const face& a, const face& b)
{
    return face::compare(a,b) != 0;
}

inline bool operator!=(const face& a, const face& b)
{
    return face::compare(a,b) == 0;
}


// ************************************************************************* //

 } // End namespace Foam
