﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2017-2020 OpenCFD Ltd.
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

#include "treeBoundBox.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


 namespace Foam{
const faceList treeBoundBox::faces
({
    face({0, 4, 6, 2}),  // 0: x-min, left
    face({1, 3, 7, 5}),  // 1: x-max, right
    face({0, 1, 5, 4}),  // 2: y-min, bottom
    face({2, 6, 7, 3}),  // 3: y-max, top
    face({0, 2, 3, 1}),  // 4: z-min, back
    face({4, 5, 7, 6})   // 5: z-max, front
});

const edgeList treeBoundBox::edges
({
    {0, 1}, // 0
    {1, 3},
    {2, 3}, // 2
    {0, 2},
    {4, 5}, // 4
    {5, 7},
    {6, 7}, // 6
    {4, 6},
    {0, 4}, // 8
    {1, 5},
    {3, 7}, // 10
    {2, 6}
});


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

treeBoundBox::treeBoundBox(const UList<point>& points)
:
    boundBox(points, false)
{
    if (points.empty())
    {
        WarningInFunction
            << "No bounding box for zero-sized pointField" << nl;
    }
}


treeBoundBox::treeBoundBox
(
    const UList<point>& points,
    const labelUList& indices
)
:
    boundBox(points, indices, false)
{
    if (points.empty() || indices.empty())
    {
        WarningInFunction
            << "No bounding box for zero-sized pointField" << nl;
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

tmp<pointField> treeBoundBox::points() const
{
    auto tpts = tmp<pointField>::New(8);
    auto& pts = tpts.ref();

    forAll(pts, octant)
    {
        pts[octant] = corner(octant);
    }

    return tpts;
}


treeBoundBox treeBoundBox::subBbox(const direction octant) const
{
    return subBbox(centre(), octant);
}


treeBoundBox treeBoundBox::subBbox
(
    const point& mid,
    const direction octant
) const
{
    if (octant > 7)
    {
        FatalErrorInFunction
            << "octant should be [0..7]"
            << abort(FatalError);
    }

    // start with a copy of this bounding box and adjust limits accordingly
    treeBoundBox subBb(*this);
    point& bbMin = subBb.min();
    point& bbMax = subBb.max();

    if (octant & treeBoundBox::RIGHTHALF)
    {
        bbMin.x() = mid.x();    // mid -> max
    }
    else
    {
        bbMax.x() = mid.x();    // min -> mid
    }

    if (octant & treeBoundBox::TOPHALF)
    {
        bbMin.y() = mid.y();    // mid -> max
    }
    else
    {
        bbMax.y() = mid.y();    // min -> mid
    }

    if (octant & treeBoundBox::FRONTHALF)
    {
        bbMin.z() = mid.z();    // mid -> max
    }
    else
    {
        bbMax.z() = mid.z();    // min -> mid
    }

    return subBb;
}


bool treeBoundBox::intersects
(
    const point& overallStart,
    const vector& overallVec,
    const point& start,
    const point& end,
    point& pt,
    direction& ptOnFaces
) const
{
    // Sutherlands algorithm:
    //   loop
    //     - start = intersection of line with one of the planes bounding
    //       the bounding box
    //     - stop if start inside bb (return true)
    //     - stop if start and end in same 'half' (e.g. both above bb)
    //       (return false)
    //
    // Uses posBits to efficiently determine 'half' in which start and end
    // point are.
    //
    // Note:
    //   - sets coordinate to exact position: e.g. pt.x() = min().x()
    //     since plane intersect routine might have truncation error.
    //     This makes sure that posBits tests 'inside'

    const direction endBits = posBits(end);
    pt = start;

    // Allow maximum of 3 clips.
    for (label i = 0; i < 4; ++i)
    {
        direction ptBits = posBits(pt);

        if (ptBits == 0)
        {
            // pt inside bb
            ptOnFaces = faceBits(pt);
            return true;
        }

        if ((ptBits & endBits) != 0)
        {
            // pt and end in same block outside of bb
            ptOnFaces = faceBits(pt);
            return false;
        }

        if (ptBits & LEFTBIT)
        {
            // Intersect with plane V=min, n=-1,0,0
            if (::Foam::mag(overallVec.x()) > VSMALL)
            {
                scalar s = (min().x() - overallStart.x())/overallVec.x();
                pt.x() = min().x();
                pt.y() = overallStart.y() + overallVec.y()*s;
                pt.z() = overallStart.z() + overallVec.z()*s;
            }
            else
            {
                // Vector not in x-direction. But still intersecting bb planes.
                // So must be close - just snap to bb.
                pt.x() = min().x();
            }
        }
        else if (ptBits & RIGHTBIT)
        {
            // Intersect with plane V=max, n=1,0,0
            if (::Foam::mag(overallVec.x()) > VSMALL)
            {
                scalar s = (max().x() - overallStart.x())/overallVec.x();
                pt.x() = max().x();
                pt.y() = overallStart.y() + overallVec.y()*s;
                pt.z() = overallStart.z() + overallVec.z()*s;
            }
            else
            {
                pt.x() = max().x();
            }
        }
        else if (ptBits & BOTTOMBIT)
        {
            // Intersect with plane V=min, n=0,-1,0
            if (::Foam::mag(overallVec.y()) > VSMALL)
            {
                scalar s = (min().y() - overallStart.y())/overallVec.y();
                pt.x() = overallStart.x() + overallVec.x()*s;
                pt.y() = min().y();
                pt.z() = overallStart.z() + overallVec.z()*s;
            }
            else
            {
                pt.x() = min().y();
            }
        }
        else if (ptBits & TOPBIT)
        {
            // Intersect with plane V=max, n=0,1,0
            if (::Foam::mag(overallVec.y()) > VSMALL)
            {
                scalar s = (max().y() - overallStart.y())/overallVec.y();
                pt.x() = overallStart.x() + overallVec.x()*s;
                pt.y() = max().y();
                pt.z() = overallStart.z() + overallVec.z()*s;
            }
            else
            {
                pt.y() = max().y();
            }
        }
        else if (ptBits & BACKBIT)
        {
            // Intersect with plane V=min, n=0,0,-1
            if (::Foam::mag(overallVec.z()) > VSMALL)
            {
                scalar s = (min().z() - overallStart.z())/overallVec.z();
                pt.x() = overallStart.x() + overallVec.x()*s;
                pt.y() = overallStart.y() + overallVec.y()*s;
                pt.z() = min().z();
            }
            else
            {
                pt.z() = min().z();
            }
        }
        else if (ptBits & FRONTBIT)
        {
            // Intersect with plane V=max, n=0,0,1
            if (::Foam::mag(overallVec.z()) > VSMALL)
            {
                scalar s = (max().z() - overallStart.z())/overallVec.z();
                pt.x() = overallStart.x() + overallVec.x()*s;
                pt.y() = overallStart.y() + overallVec.y()*s;
                pt.z() = max().z();
            }
            else
            {
                pt.z() = max().z();
            }
        }
    }

    // Can end up here if the end point is on the edge of the boundBox
    return true;
}


bool treeBoundBox::intersects
(
    const point& start,
    const point& end,
    point& pt
) const
{
    direction ptBits;
    return intersects(start, end-start, start, end, pt, ptBits);
}


bool treeBoundBox::contains(const vector& dir, const point& pt) const
{
    // Compare all components against min and max of bb

    for (direction cmpt=0; cmpt < point::nComponents; ++cmpt)
    {
        if (pt[cmpt] < min()[cmpt])
        {
            return false;
        }
        else if (pt[cmpt] == min()[cmpt])
        {
            // On edge. Outside if direction points outwards.
            if (dir[cmpt] < 0)
            {
                return false;
            }
        }

        if (pt[cmpt] > max()[cmpt])
        {
            return false;
        }
        else if (pt[cmpt] == max()[cmpt])
        {
            // On edge. Outside if direction points outwards.
            if (dir[cmpt] > 0)
            {
                return false;
            }
        }
    }

    // All components inside bb
    return true;
}


direction treeBoundBox::faceBits(const point& pt) const
{
    direction octant = 0;

    if (pt.x() == min().x())
    {
        octant |= LEFTBIT;
    }
    else if (pt.x() == max().x())
    {
        octant |= RIGHTBIT;
    }

    if (pt.y() == min().y())
    {
        octant |= BOTTOMBIT;
    }
    else if (pt.y() == max().y())
    {
        octant |= TOPBIT;
    }

    if (pt.z() == min().z())
    {
        octant |= BACKBIT;
    }
    else if (pt.z() == max().z())
    {
        octant |= FRONTBIT;
    }

    return octant;
}


direction treeBoundBox::posBits(const point& pt) const
{
    direction octant = 0;

    if (pt.x() < min().x())
    {
        octant |= LEFTBIT;
    }
    else if (pt.x() > max().x())
    {
        octant |= RIGHTBIT;
    }

    if (pt.y() < min().y())
    {
        octant |= BOTTOMBIT;
    }
    else if (pt.y() > max().y())
    {
        octant |= TOPBIT;
    }

    if (pt.z() < min().z())
    {
        octant |= BACKBIT;
    }
    else if (pt.z() > max().z())
    {
        octant |= FRONTBIT;
    }

    return octant;
}


void treeBoundBox::calcExtremities
(
    const point& pt,
    point& nearest,
    point& furthest
) const
{
    for (direction cmpt=0; cmpt < point::nComponents; ++cmpt)
    {
        if
        (
            ::Foam::mag(min()[cmpt] - pt[cmpt])
          < ::Foam::mag(max()[cmpt] - pt[cmpt])
        )
        {
            nearest[cmpt] = min()[cmpt];
            furthest[cmpt] = max()[cmpt];
        }
        else
        {
            nearest[cmpt] = max()[cmpt];
            furthest[cmpt] = min()[cmpt];
        }
    }
}


scalar treeBoundBox::maxDist(const point& pt) const
{
    point near, far;
    calcExtremities(pt, near, far);

    return ::Foam::mag(far - pt);
}


label treeBoundBox::distanceCmp
(
    const point& pt,
    const treeBoundBox& other
) const
{
    //
    // Distance point <-> nearest and furthest away vertex of this
    //

    point nearThis, farThis;

    // get nearest and furthest away vertex
    calcExtremities(pt, nearThis, farThis);

    const scalar minDistThis =
        sqr(nearThis.x() - pt.x())
     +  sqr(nearThis.y() - pt.y())
     +  sqr(nearThis.z() - pt.z());
    const scalar maxDistThis =
        sqr(farThis.x() - pt.x())
     +  sqr(farThis.y() - pt.y())
     +  sqr(farThis.z() - pt.z());

    //
    // Distance point <-> other
    //

    point nearOther, farOther;

    // get nearest and furthest away vertex
    other.calcExtremities(pt, nearOther, farOther);

    const scalar minDistOther =
        sqr(nearOther.x() - pt.x())
     +  sqr(nearOther.y() - pt.y())
     +  sqr(nearOther.z() - pt.z());
    const scalar maxDistOther =
        sqr(farOther.x() - pt.x())
     +  sqr(farOther.y() - pt.y())
     +  sqr(farOther.z() - pt.z());

    //
    // Categorize
    //
    if (maxDistThis < minDistOther)
    {
        // All vertices of this are nearer to point than any vertex of other
        return -1;
    }
    else if (minDistThis > maxDistOther)
    {
        // All vertices of this are further from point than any vertex of other
        return 1;
    }
    else
    {
        // Mixed bag
        return 0;
    }
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

Ostream& operator<<(Ostream& os, const treeBoundBox& bb)
{
    return os << static_cast<const boundBox&>(bb);
}


Istream& operator>>(Istream& is, treeBoundBox& bb)
{
    return is >> static_cast<boundBox&>(bb);
}


// ************************************************************************* //

 } // End namespace Foam
