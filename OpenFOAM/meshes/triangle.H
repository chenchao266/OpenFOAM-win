﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
    Copyright (C) 2018-2020 OpenCFD Ltd.
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

Class
    Foam::triangle

Description
    A triangle primitive used to calculate face normals and swept volumes.

SourceFiles
    triangleI.H

\*---------------------------------------------------------------------------*/

#ifndef triangle_H
#define triangle_H

#include "intersection.H"
#include "vector2.H"
#include "tensor2.H"
#include "pointHit2.H"
#include "Random.H"
#include "FixedList.H"
#include "UList.H"
#include "linePointRef.H"
#include "barycentric2D2.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward Declarations
class plane;
class triPoints;

template<class Point, class PointRef> class triangle;

template<class Point, class PointRef>
inline Istream& operator>>(Istream&, triangle<Point, PointRef>&);

template<class Point, class PointRef>
inline Ostream& operator<<(Ostream&, const triangle<Point, PointRef>&);


// Common Typedefs

//- A triangle using referred points
typedef triangle<point, const point&> triPointRef;


/*---------------------------------------------------------------------------*\
                          Class triangle Declaration
\*---------------------------------------------------------------------------*/

template<class Point, class PointRef>
class triangle
{
public:

    // Public Typedefs

        //- The point type
        typedef Point point_type;

        //- Storage type for triangles originating from intersecting triangle
        //- with another triangle
        typedef FixedList<triPoints, 27> triIntersectionList;

        //- The proximity classifications
        enum proxType
        {
            NONE = 0,   //!< Unknown proximity
            POINT,      //!< Close to point
            EDGE        //!< Close to edge
        };


    // Public classes

        // Classes for use in sliceWithPlane. What to do with decomposition
        // of triangle.

            //- Dummy
            class dummyOp
            {
            public:
                inline void operator()(const triPoints&);
            };

            //- Sum resulting areas
            class sumAreaOp
            {
            public:
                scalar area_;

                inline sumAreaOp();

                inline void operator()(const triPoints&);
            };

            //- Store resulting tris
            class storeOp
            {
            public:
                triIntersectionList& tris_;
                label& nTris_;

                inline storeOp(triIntersectionList&, label&);

                inline void operator()(const triPoints&);
            };


private:

    // Private Data

        PointRef a_, b_, c_;


   // Private Member Functions

        //- Helper: calculate intersection point
        inline static point planeIntersection
        (
            const FixedList<scalar, 3>& d,
            const triPoints& t,
            const label negI,
            const label posI
        );

        //- Helper: slice triangle with plane
        template<class AboveOp, class BelowOp>
        inline static void triSliceWithPlane
        (
            const plane& pln,
            const triPoints& tri,
            AboveOp& aboveOp,
            BelowOp& belowOp
        );


public:

    // Constructors

        //- Construct from three points
        inline triangle(const Point& a, const Point& b, const Point& c);

        //- Construct from three points
        inline triangle(const FixedList<Point, 3>& tri);

        //- Construct from three points in the list of points
        //  The indices could be from triFace etc.
        inline triangle
        (
            const UList<Point>& points,
            const FixedList<label, 3>& indices
        );

        //- Construct from Istream
        inline explicit triangle(Istream& is);


    // Member Functions

        // Access

            //- Return first vertex
            inline const Point& a() const;

            //- Return second vertex
            inline const Point& b() const;

            //- Return third vertex
            inline const Point& c() const;


        // Properties

            //- Return centre (centroid)
            inline Point centre() const;

            //- The area normal - with magnitude equal to area of triangle
            inline vector areaNormal() const;

            //- Return unit normal
            inline vector unitNormal() const;

            //- Legacy name for areaNormal().
            //  \deprecated(2018-06) Deprecated for new use
            FOAM_DEPRECATED_FOR(2018-12, "areaNormal() or unitNormal()")
            vector normal() const
            {
                return areaNormal();
            }

            //- Return scalar magnitude
            inline scalar mag() const;

            //- Return circum-centre
            inline Point circumCentre() const;

            //- Return circum-radius
            inline scalar circumRadius() const;

            //- Return quality: Ratio of triangle and circum-circle
            //  area, scaled so that an equilateral triangle has a
            //  quality of 1
            inline scalar quality() const;

            //- Return swept-volume
            inline scalar sweptVol(const triangle& t) const;

            //- Return the inertia tensor, with optional reference
            //  point and density specification
            inline tensor inertia
            (
                PointRef refPt = Zero,
                scalar density = 1.0
            ) const;

            //- Return a random point on the triangle from a uniform
            //  distribution
            inline Point randomPoint(Random& rndGen) const;

            //- Calculate the point from the given barycentric coordinates.
            inline Point barycentricToPoint(const barycentric2D& bary) const;

            //- Calculate the barycentric coordinates from the given point
            inline barycentric2D pointToBarycentric(const point& pt) const;

            //- Calculate the barycentric coordinates from the given point.
            //  Returns the determinant.
            inline scalar pointToBarycentric
            (
                const point& pt,
                barycentric2D& bary
            ) const;

            //- Return point intersection with a ray.
            //  For a hit, the distance is signed. Positive number
            //  represents the point in front of triangle.
            //  In case of miss pointHit is set to nearest point
            //  on triangle and its distance to the distance between
            //  the original point and the plane intersection point
            inline pointHit ray
            (
                const point& p,
                const vector& q,
                const intersection::algorithm = intersection::FULL_RAY,
                const intersection::direction dir = intersection::VECTOR
            ) const;

            //- Fast intersection with a ray.
            //  For a hit, the pointHit.distance() is the line parameter t :
            //  intersection=p+t*q. Only defined for VISIBLE, FULL_RAY or
            //  HALF_RAY. tol increases the virtual size of the triangle
            // by a relative factor.
            inline pointHit intersection
            (
                const point& p,
                const vector& q,
                const intersection::algorithm alg,
                const scalar tol = 0.0
            ) const;

            //- Find the nearest point to p on the triangle and classify it:
            //  + near point (nearType=POINT, nearLabel=0, 1, 2)
            //  + near edge (nearType=EDGE, nearLabel=0, 1, 2)
            //    Note: edges are counted from starting
            //    vertex so e.g. edge 2 is from f[2] to f[0]
            pointHit nearestPointClassify
            (
                const point& p,
                label& nearType,
                label& nearLabel
            ) const;

            //- Return nearest point to p on triangle
            inline pointHit nearestPoint(const point& p) const;

            //- Classify nearest point to p in triangle plane
            //  w.r.t. triangle edges and points.  Returns inside
            //  (true)/outside (false).
            bool classify
            (
                const point& p,
                label& nearType,
                label& nearLabel
            ) const;

            //- Return nearest point to line on triangle. Returns hit if
            //  point is inside triangle. Sets edgePoint to point on edge
            //  (hit if nearest is inside line)
            inline pointHit nearestPoint
            (
                const linePointRef& edge,
                pointHit& edgePoint
            ) const;

            //- The sign for which side of the face plane the point is on.
            //  Uses the supplied tolerance for rounding around zero.
            //  \return
            //  -  0: on plane
            //  - +1: front-side
            //  - -1: back-side
            inline int sign(const point& p, const scalar tol = SMALL) const;

            //- Decompose triangle into triangles above and below plane
            template<class AboveOp, class BelowOp>
            inline void sliceWithPlane
            (
                const plane& pln,
                AboveOp& aboveOp,
                BelowOp& belowOp
            ) const;

            //- Decompose triangle into triangles inside and outside
            //  (with respect to user provided normal) other
            //  triangle.
            template<class InsideOp, class OutsideOp>
            inline void triangleOverlap
            (
                const vector& n,
                const triangle<Point, PointRef>& tri,
                InsideOp& insideOp,
                OutsideOp& outsideOp
            ) const;


    // IOstream Operators

        friend Istream& operator>> <Point, PointRef>
        (
            Istream&,
            triangle&
        );

        friend Ostream& operator<< <Point, PointRef>
        (
            Ostream&,
            const triangle&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "triangleI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "triangle.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
