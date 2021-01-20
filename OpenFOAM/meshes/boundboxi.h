﻿/*---------------------------------------------------------------------------*\
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

//#include "boundBox.H"
#include "pointField.H"


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //
namespace Foam {
inline boundBox::boundBox() :    min_(Zero),
    max_(Zero)
{}


inline boundBox::boundBox(const point& min, const point& max) :    min_(min),
    max_(max)
{}


inline boundBox::boundBox(Istream& is)
{
    operator>>(is, *this);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const point& boundBox::min() const
{
    return min_;
}


inline const point& boundBox::max() const
{
    return max_;
}


inline point& boundBox::min()
{
    return min_;
}


inline point& boundBox::max()
{
    return max_;
}


inline point boundBox::midpoint() const
{
    return 0.5 * (max_ + min_);
}


inline vector boundBox::span() const
{
    return (max_ - min_);
}


inline scalar boundBox::mag() const
{
    return ::Foam::mag(max_ - min_);
}


inline scalar boundBox::volume() const
{
    return cmptProduct(span());
}


inline scalar boundBox::minDim() const
{
    return cmptMin(span());
}


inline scalar boundBox::maxDim() const
{
    return cmptMax(span());
}


inline scalar boundBox::avgDim() const
{
    return cmptAv(span());
}


inline bool boundBox::overlaps(const boundBox& bb) const
{
    return
    (
        bb.max_.x() >= min_.x() && bb.min_.x() <= max_.x()
     && bb.max_.y() >= min_.y() && bb.min_.y() <= max_.y()
     && bb.max_.z() >= min_.z() && bb.min_.z() <= max_.z()
    );
}


inline bool boundBox::overlaps
(
    const point& centre,
    const scalar radiusSqr
) const
{
    // Find out where centre is in relation to bb.
    // Find nearest point on bb.
    scalar distSqr = 0;

    for (direction dir = 0; dir < vector::nComponents; dir++)
    {
        scalar d0 = min_[dir] - centre[dir];
        scalar d1 = max_[dir] - centre[dir];

        if ((d0 > 0) != (d1 > 0))
        {
            // centre inside both extrema. This component does not add any
            // distance.
        }
        else if (::Foam::mag(d0) < ::Foam::mag(d1))
        {
            distSqr += d0*d0;
        }
        else
        {
            distSqr += d1*d1;
        }

        if (distSqr > radiusSqr)
        {
            return false;
        }
    }

    return true;
}


inline bool boundBox::contains(const point& pt) const
{
    return
    (
        pt.x() >= min_.x() && pt.x() <= max_.x()
     && pt.y() >= min_.y() && pt.y() <= max_.y()
     && pt.z() >= min_.z() && pt.z() <= max_.z()
    );
}


// this.bb fully contains bb
inline bool boundBox::contains(const boundBox& bb) const
{
    return contains(bb.min()) && contains(bb.max());
}


inline bool boundBox::containsInside(const point& pt) const
{
    return
    (
        pt.x() > min_.x() && pt.x() < max_.x()
     && pt.y() > min_.y() && pt.y() < max_.y()
     && pt.z() > min_.z() && pt.z() < max_.z()
    );
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

inline bool operator==(const boundBox& a, const boundBox& b)
{
    return (a.min_ == b.min_) && (a.max_ == b.max_);
}


inline bool operator!=(const boundBox& a, const boundBox& b)
{
    return !(a == b);
}

}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
