﻿/*---------------------------------------------------------------------------*\
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

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


 namespace Foam{
inline septernion::septernion(const  ::Foam::zero)
:
    t_(Zero),
    r_(Zero)
{}


inline septernion::septernion(const vector& t, const quaternion& r)
:
    t_(t),
    r_(r)
{}


inline septernion::septernion(const vector& t)
:
    t_(t),
    r_(quaternion::I)
{}


inline septernion::septernion(const quaternion& r)
:
    t_(Zero),
    r_(r)
{}


inline septernion::septernion(const spatialTransform& st)
:
    t_(st.r()),
    r_(st.E())
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const vector& septernion::t() const
{
    return t_;
}


inline const quaternion& septernion::r() const
{
    return r_;
}


inline vector& septernion::t()
{
    return t_;
}


inline quaternion& septernion::r()
{
    return r_;
}


inline vector septernion::transformPoint(const vector& v) const
{
    return r().transform(v - t());
}


inline vector septernion::invTransformPoint(const vector& v) const
{
    return t() + r().invTransform(v);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline void septernion::operator*=(const septernion& tr)
{
    t_ = tr.t() + tr.r().invTransform(t_);
    r_ *= tr.r();
}


inline void septernion::operator=(const vector& t)
{
    t_ = t;
    r_ = quaternion::I;
}


inline void septernion::operator+=(const vector& t)
{
    t_ += t;
}


inline void septernion::operator-=(const vector& t)
{
    t_ -= t;
}


inline void septernion::operator=(const quaternion& r)
{
    t_ = Zero;
    r_ = r;
}


inline void septernion::operator*=(const quaternion& r)
{
    t_ = r.invTransform(t_);
    r_ *= r;
}


inline void septernion::operator/=(const quaternion& r)
{
    t_ = r.transform(t_);
    r_ /= r;
}


inline void septernion::operator*=(const scalar s)
{
    t_ *= s;
    r_ *= s;
}

inline void septernion::operator/=(const scalar s)
{
    t_ /= s;
    r_ /= s;
}


// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

inline septernion inv(const septernion& tr)
{
    return septernion(-tr.r().transform(tr.t()), conjugate(tr.r()));
}


// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

inline bool operator==(const septernion& tr1, const septernion& tr2)
{
    return (tr1.t() == tr2.t() && tr1.r() == tr2.r());
}


inline bool operator!=(const septernion& tr1, const septernion& tr2)
{
    return !operator==(tr1, tr2);
}


inline septernion operator+
(
    const septernion& tr,
    const vector& t
)
{
    return septernion(tr.t() + t, tr.r());
}


inline septernion operator+
(
    const vector& t,
    const septernion& tr
)
{
    return septernion(t + tr.t(), tr.r());
}


inline septernion operator-
(
    const septernion& tr,
    const vector& t
)
{
    return septernion(tr.t() - t, tr.r());
}


inline septernion operator*
(
    const quaternion& r,
    const septernion& tr
)
{
    return septernion(tr.t(), r*tr.r());
}


inline septernion operator*
(
    const septernion& tr,
    const quaternion& r
)
{
    return septernion(r.invTransform(tr.t()), tr.r()*r);
}


inline septernion operator/
(
    const septernion& tr,
    const quaternion& r
)
{
    return septernion(r.transform(tr.t()), tr.r()/r);
}


inline septernion operator*
(
    const septernion& tr1,
    const septernion& tr2
)
{
    return septernion
    (
        tr2.r().invTransform(tr1.t()) + tr2.t(),
        tr1.r().transform(tr2.r())
    );
}


inline septernion operator/
(
    const septernion& tr1,
    const septernion& tr2
)
{
    return tr1*inv(tr2);
}


inline septernion operator*(const scalar s, const septernion& tr)
{
    return septernion(s*tr.t(), s*tr.r());
}


inline septernion operator*(const septernion& tr, const scalar s)
{
    return septernion(s*tr.t(), s*tr.r());
}


inline septernion operator/(const septernion& tr, const scalar s)
{
    return septernion(tr.t()/s, tr.r()/s);
}


// ************************************************************************* //

 } // End namespace Foam
