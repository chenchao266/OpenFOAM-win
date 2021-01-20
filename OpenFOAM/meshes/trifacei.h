﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2013 OpenFOAM Foundation
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

#include "IOstreams.H"
#include "face.T.H"
#include "triPointRef.H"
#include "Swap.T.H"

// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //
namespace Foam {
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
    else
    {
        return 0;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline triFace::triFace()
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


inline triFace::triFace(const labelUList& lst) :    FixedList<label, 3>(lst)
{}


inline triFace::triFace(Istream& is) :    FixedList<label, 3>(is)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline label triFace::collapse()
{
    // we cannot resize a FixedList, so mark duplicates with '-1'
    // (the lower vertex is retained)
    // catch any '-1' (eg, if called twice)

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
    Swap(operator[](1), operator[](2));
}


inline pointField triFace::points(const pointField& points) const
{
    pointField p(3);

    p[0] = points[operator[](0)];
    p[1] = points[operator[](1)];
    p[2] = points[operator[](2)];

    return p;
}


inline face triFace::triFaceFace() const
{
    face f(3);

    f[0] = operator[](0);
    f[1] = operator[](1);
    f[2] = operator[](2);

    return f;
}


inline triPointRef triFace::tri(const pointField& points) const
{
    return triPointRef
    (
        points[operator[](0)],
        points[operator[](1)],
        points[operator[](2)]
    );
}


inline point triFace::centre(const pointField& points) const
{
    return (1.0/3.0)*
    (
        points[operator[](0)]
      + points[operator[](1)]
      + points[operator[](2)]
    );
}


inline scalar triFace::mag(const pointField& points) const
{
    return ::Foam::mag(normal(points));
}


// could also delegate to triPointRef(...).normal()
inline vector triFace::normal(const pointField& points) const
{
    return 0.5*
    (
        (points[operator[](1)] - points[operator[](0)])
       ^(points[operator[](2)] - points[operator[](0)])
    );
}


inline label triFace::nTriangles() const
{
    return 1;
}


inline triFace triFace::reverseFace() const
{
    // The starting points of the original and reverse face are identical.
    return triFace(operator[](0), operator[](2), operator[](1));
}


inline scalar triFace::sweptVol
(
    const pointField& opts,
    const pointField& npts
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
    const pointField& points,
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
    const pointField& points,
    const intersection::algorithm alg,
    const intersection::direction dir
) const
{
    return this->tri(points).ray(p, q, alg, dir);
}



inline pointHit triFace::intersect
(
    const point& p,
    const vector& q,
    const pointField& points,
    const intersection::algorithm alg,
    const scalar tol
) const
{
    return this->tri(points).intersect(p, q, alg, tol);
}


inline pointHit triFace::intersect
(
    const point& p,
    const vector& q,
    const point& ctr,
    const pointField& points,
    const intersection::algorithm alg,
    const scalar tol
) const
{
    return intersect(p, q, points, alg, tol);
}


inline pointHit triFace::nearestPoint
(
    const point& p,
    const pointField& points
) const
{
    return this->tri(points).nearestPoint(p);
}


inline pointHit triFace::nearestPointClassify
(
    const point& p,
    const pointField& points,
    label& nearType,
    label& nearLabel
) const
{
    return this->tri(points).nearestPointClassify(p, nearType, nearLabel);
}


inline label triFace::nEdges() const
{
    return 3;
}


inline edgeList triFace::edges() const
{
    edgeList e(3);

    e[0].start() = operator[](0);
    e[0].end()   = operator[](1);

    e[1].start() = operator[](1);
    e[1].end()   = operator[](2);

    e[2].start() = operator[](2);
    e[2].end()   = operator[](0);

    return e;
}


inline edge triFace::faceEdge(const label n) const
{
    return edge(operator[](n), operator[](fcIndex(n)));
}


// return
//  - +1: forward (counter-clockwise) on the face
//  - -1: reverse (clockwise) on the face
//  -  0: edge not found on the face
inline int triFace::edgeDirection(const edge& e) const
{
    if
    (
        (operator[](0) == e.start() && operator[](1) == e.end())
     || (operator[](1) == e.start() && operator[](2) == e.end())
     || (operator[](2) == e.start() && operator[](0) == e.end())
    )
    {
        return 1;
    }
    else if
    (
        (operator[](0) == e.end() && operator[](1) == e.start())
     || (operator[](1) == e.end() && operator[](2) == e.start())
     || (operator[](2) == e.end() && operator[](0) == e.start())
    )
    {
        return -1;
    }
    else
    {
        return 0;
    }
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

inline bool operator==(const triFace& a, const triFace& b)
{
    return triFace::compare(a,b) != 0;
}


inline bool operator!=(const triFace& a, const triFace& b)
{
    return triFace::compare(a,b) == 0;
}

}
// ************************************************************************* //
