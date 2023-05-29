/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2018-2019 OpenCFD Ltd.
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
inline plane::plane()
:
    normal_(Zero),
    origin_(Zero)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const vector& plane::normal() const
{
    return normal_;
}


inline const point& plane::origin() const
{
    return origin_;
}


inline point& plane::origin()
{
    return origin_;
}


inline const point& plane::refPoint() const
{
    return origin_;
}


inline void plane::flip()
{
    normal_ = -normal_;
}


inline point plane::nearestPoint(const point& p) const
{
    return p - normal_*((p - origin_) & normal_);
}


inline scalar plane::distance(const point& p) const
{
    return mag(signedDistance(p));
}


inline scalar plane::signedDistance(const point& p) const
{
    return ((p - origin_) & normal_);
}


inline plane::side plane::sideOfPlane(const point& p) const
{
    const scalar dist = signedDistance(p);

    return (dist < 0 ? BACK : FRONT);
}


inline int plane::sign(const point& p, const scalar tol) const
{
    const scalar dist = signedDistance(p);

    return ((dist < -tol) ? -1 : (dist > tol) ? +1 : 0);
}


// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

inline bool operator==(const plane& a, const plane& b)
{
    return (a.origin() == b.origin() && a.normal() == b.normal());
}


inline bool operator!=(const plane& a, const plane& b)
{
    return !(a == b);
}


inline bool operator<(const plane& a, const plane& b)
{
    return (a.origin() < b.origin());
}


// ************************************************************************* //

 } // End namespace Foam
