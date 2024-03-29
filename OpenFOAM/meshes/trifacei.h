﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2013 OpenFOAM Foundation
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

#include "IOstreams.H"
#include "face.H"
#include "triPointRef.H"
#include <algorithm>  // For std::swap

// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //


 namespace Foam{
inline int triFace::compare(const triFace& a, const triFace& b)
{
    if
    (
        (a[0] == b[0] && a[1] == b[1] && a[2] == b[2])
     || (a[0] == b[1] && a[1] == b[2] && a[2] == b[0])
     || (a[0] == b[2] && a[1] == b[0] && a[2] == b[1])
    )
    {
        // identical
        return 1;
    }
    else if
    (
        (a[0] == b[2] && a[1] == b[1] && a[2] == b[0])
     || (a[0] == b[1] && a[1] == b[0] && a[2] == b[2])
     || (a[0] == b[0] && a[1] == b[2] && a[2] == b[1])
    )
    {
        // same face, but reversed orientation
        return -1;
    }

    return 0;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline triFace::triFace()
:
    FixedList<label, 3>(-1)
{}


inline triFace::triFace
(
    const label a,
    const label b,
    const label c
)
{
    operator[](0) = a;
    operator[](1) = b;
    operator[](2) = c;
}


inline triFace::triFace(std::initializer_list<label> list)
:
    FixedList<label, 3>(list)
{}


inline triFace::triFace(const labelUList& list)
:
    FixedList<label, 3>(list)
{}


inline triFace::triFace
(
    const labelUList& list,
    const FixedList<label, 3>& indices
)
:
    FixedList<label, 3>(list, indices)
{}


inline triFace::triFace(Istream& is)
:
    FixedList<label, 3>(is)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline label triFace::collapse()
{
    // Cannot resize FixedList, so mark duplicates with '-1'
    // (the lower vertex is retained)
    // catch any '-1' (eg, if called multiple times)

    label n = 3;
    if (operator[](0) == operator[](1) || operator[](1) == -1)
    {
        operator[](1) = -1;
        n--;
    }
    else if (operator[](1) == operator[](2) || operator[](2) == -1)
    {
        operator[](2) = -1;
        n--;
    }
    if (operator[](0) == operator[](2))
    {
        operator[](2) = -1;
        n--;
    }

    return n;
}


inline void triFace::flip()
{
    std::swap(operator[](1), operator[](2));
}


inline pointField triFace::points(const UList<point>& pts) const
{
    pointField p(3);

    p[0] = pts[operator[](0)];
    p[1] = pts[operator[](1)];
    p[2] = pts[operator[](2)];

    return p;
}


inline face triFace::triFaceFace() const
{
    return face(*this);
}


inline triPointRef triFace::tri(const UList<point>& points) const
{
    return triPointRef
    (
        points[operator[](0)],
        points[operator[](1)],
        points[operator[](2)]
    );
}


inline point triFace::centre(const UList<point>& points) const
{
    return (1.0/3.0)*
    (
        points[operator[](0)]
      + points[operator[](1)]
      + points[operator[](2)]
    );
}


inline vector triFace::areaNormal(const UList<point>& points) const
{
    // As per triPointRef(...).areaNormal()
    return 0.5*
    (
        (points[operator[](1)] - points[operator[](0)])
       ^(points[operator[](2)] - points[operator[](0)])
    );
}


inline vector triFace::unitNormal(const UList<point>& points) const
{
    const vector n(areaNormal(points));
    const scalar s(::Foam::mag(n));
    return s < ROOTVSMALL ? Zero : n/s;
}


inline scalar triFace::mag(const UList<point>& points) const
{
    return ::Foam::mag(areaNormal(points));
}


inline label triFace::nTriangles() const noexcept
{
    return 1;
}


inline triFace triFace::reverseFace() const
{
    // The starting points of the original and reverse face are identical.
    return triFace(operator[](0), operator[](2), operator[](1));
}


inline label triFace::which(const label pointLabel) const
{
    return FixedList<label, 3>::find(pointLabel);
}


inline label triFace::thisLabel(const label i) const
{
    return operator[](i);
}


inline label triFace::nextLabel(const label i) const
{
    return operator[]((i == 2 ? 0 : i+1));
}


inline label triFace::prevLabel(const label i) const
{
    return operator[]((i ? i-1 : 2));
}


inline scalar triFace::sweptVol
(
    const UList<point>& opts,
    const UList<point>& npts
) const
{
    return (1.0/6.0)*
    (
        (
            (npts[operator[](0)] - opts[operator[](0)])
          & (
                (opts[operator[](1)] - opts[operator[](0)])
              ^ (opts[operator[](2)] - opts[operator[](0)])
            )
        )
      + (
            (npts[operator[](1)] - opts[operator[](1)])
          & (
                (opts[operator[](2)] - opts[operator[](1)])
              ^ (npts[operator[](0)] - opts[operator[](1)])
            )
        )
      + (
            (opts[operator[](2)] - npts[operator[](2)])
          & (
                (npts[operator[](1)] - npts[operator[](2)])
              ^ (npts[operator[](0)] - npts[operator[](2)])
            )
        )
    );
}


tensor triFace::inertia
(
    const UList<point>& points,
    const point& refPt,
    scalar density
) const
{
    // a triangle, do a direct calculation
    return this->tri(points).inertia(refPt, density);
}


inline pointHit triFace::ray
(
    const point& p,
    const vector& q,
    const UList<point>& points,
    const intersection::algorithm alg,
    const intersection::direction dir
) const
{
    return this->tri(points).ray(p, q, alg, dir);
}



inline pointHit triFace::intersection
(
    const point& p,
    const vector& q,
    const UList<point>& points,
    const intersection::algorithm alg,
    const scalar tol
) const
{
    return this->tri(points).intersection(p, q, alg, tol);
}


inline pointHit triFace::intersection
(
    const point& p,
    const vector& q,
    const point& ctr,
    const UList<point>& points,
    const intersection::algorithm alg,
    const scalar tol
) const
{
    return intersection(p, q, points, alg, tol);
}


inline pointHit triFace::nearestPoint
(
    const point& p,
    const UList<point>& points
) const
{
    return this->tri(points).nearestPoint(p);
}


inline pointHit triFace::nearestPointClassify
(
    const point& p,
    const UList<point>& points,
    label& nearType,
    label& nearLabel
) const
{
    return this->tri(points).nearestPointClassify(p, nearType, nearLabel);
}


inline int triFace::sign
(
    const point& p,
    const UList<point>& points,
    const scalar tol
) const
{
    return this->tri(points).sign(p, tol);
}


inline label triFace::nEdges() const noexcept
{
    return 3;
}


inline ::Foam::edge triFace::edge(const label edgei) const
{
    return ::Foam::edge(thisLabel(edgei), nextLabel(edgei));
}


inline vector triFace::edge
(
    const label edgei,
    const UList<point>& pts
) const
{
    return vector(pts[nextLabel(edgei)] - pts[thisLabel(edgei)]);
}


inline ::Foam::edge triFace::rcEdge(const label edgei) const
{
    // Edge 0 (forward and reverse) always starts at [0]
    // for consistency with face flipping
    const label pointi = edgei ? (3 - edgei) : 0;
    return ::Foam::edge(thisLabel(pointi), prevLabel(pointi));
}


inline vector triFace::rcEdge
(
    const label edgei,
    const UList<point>& pts
) const
{
    // Edge 0 (forward and reverse) always starts at [0]
    // for consistency with face flipping
    const label pointi = edgei ? (3 - edgei) : 0;
    return vector(pts[prevLabel(pointi)] - pts[thisLabel(pointi)]);
}


inline edgeList triFace::edges() const
{
    edgeList theEdges(3);

    theEdges[0].first()  = operator[](0);
    theEdges[0].second() = operator[](1);

    theEdges[1].first()  = operator[](1);
    theEdges[1].second() = operator[](2);

    theEdges[2].first()  = operator[](2);
    theEdges[2].second() = operator[](0);

    return theEdges;
}


inline edgeList triFace::rcEdges() const
{
    edgeList theEdges(3);

    theEdges[0].first()  = operator[](0);
    theEdges[0].second() = operator[](2);

    theEdges[1].first()  = operator[](2);
    theEdges[1].second() = operator[](1);

    theEdges[2].first()  = operator[](1);
    theEdges[2].second() = operator[](0);

    return theEdges;
}


inline int triFace::edgeDirection(const ::Foam::edge& e) const
{
    if (e.first() == operator[](0))
    {
        if (e.second() == operator[](1)) return 1;  // Forward
        if (e.second() == operator[](2)) return -1; // Reverse
    }
    if (e.first() == operator[](1))
    {
        if (e.second() == operator[](2)) return 1;  // Forward
        if (e.second() == operator[](0)) return -1; // Reverse
    }
    if (e.first() == operator[](2))
    {
        if (e.second() == operator[](0)) return 1;  // Forward
        if (e.second() == operator[](1)) return -1; // Reverse
    }

    return 0;  // Not found
}


// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

inline bool operator==(const triFace& a, const triFace& b)
{
    return triFace::compare(a,b) != 0;
}


inline bool operator!=(const triFace& a, const triFace& b)
{
    return triFace::compare(a,b) == 0;
}


// ************************************************************************* //

 } // End namespace Foam
