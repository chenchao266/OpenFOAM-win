﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
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
inline vectorTensorTransform::vectorTensorTransform()
:
    t_(Zero),
    R_(sphericalTensor::I),
    hasR_(false)
{}


inline vectorTensorTransform::vectorTensorTransform
(
    const vector& t,
    const tensor& R,
    bool hasR
)
:
    t_(t),
    R_(R),
    hasR_(hasR)
{}


inline vectorTensorTransform::vectorTensorTransform(const vector& t)
:
    t_(t),
    R_(sphericalTensor::I),
    hasR_(false)
{}


inline vectorTensorTransform::vectorTensorTransform(const tensor& R)
:
    t_(Zero),
    R_(R),
    hasR_(true)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const vector& vectorTensorTransform::t() const
{
    return t_;
}


inline const tensor& vectorTensorTransform::R() const
{
    return R_;
}


inline bool vectorTensorTransform::hasR() const
{
    return hasR_;
}


inline vector& vectorTensorTransform::t()
{
    return t_;
}


inline tensor& vectorTensorTransform::R()
{
    // Assume that non-const access to R changes it from I, so set
    // hasR to true

    hasR_ = true;

    return R_;
}


inline vector vectorTensorTransform::transformPosition
(
    const vector& v
) const
{
    if (hasR_)
    {
        return t() + (R() & v);
    }
    else
    {
        return t() + v;
    }
}


inline pointField vectorTensorTransform::transformPosition
(
    const pointField& pts
) const
{
    tmp<pointField> tfld;

    if (hasR_)
    {
        tfld = t() + (R() & pts);
    }
    else
    {
        tfld = t() + pts;
    }
    return tfld();
}


inline vector vectorTensorTransform::invTransformPosition
(
    const vector& v
) const
{
    if (hasR_)
    {
        return (R().T() & (v - t()));
    }
    else
    {
        return v - t();
    }
}


inline pointField vectorTensorTransform::invTransformPosition
(
    const pointField& pts
) const
{
    tmp<pointField> tfld;

    if (hasR_)
    {
        tfld = (R().T() & (pts - t()));
    }
    else
    {
        tfld = pts - t();
    }
    return tfld();
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline void vectorTensorTransform::operator&=
(
    const vectorTensorTransform& tr
)
{
    t_ += tr.t_;
    R_ = tr.R_ & R_;

    // If either of the two objects has hasR_ as true, then inherit
    // it, otherwise, these should both be I tensors.
    hasR_ = tr.hasR_ || hasR_;
}


inline void vectorTensorTransform::operator=(const vector& t)
{
    t_ = t;
}


inline void vectorTensorTransform::operator+=(const vector& t)
{
    t_ += t;
}


inline void vectorTensorTransform::operator-=(const vector& t)
{
    t_ -= t;
}


inline void vectorTensorTransform::operator=(const tensor& R)
{
    hasR_ = true;

    R_ = R;
}


inline void vectorTensorTransform::operator&=(const tensor& R)
{
    hasR_ = true;

    R_ = R & R_;
}


// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

inline vectorTensorTransform inv(const vectorTensorTransform& tr)
{
    return vectorTensorTransform(-tr.t(), tr.R().T(), tr.hasR());
}


// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

inline bool operator==
(
    const vectorTensorTransform& tr1,
    const vectorTensorTransform& tr2
)
{
    return (tr1.t() == tr2.t() && tr1.R() == tr2.R());
}


inline bool operator!=
(
    const vectorTensorTransform& tr1,
    const vectorTensorTransform& tr2
)
{
    return !operator==(tr1, tr2);
}


inline vectorTensorTransform operator+
(
    const vectorTensorTransform& tr,
    const vector& t
)
{
    return vectorTensorTransform(tr.t() + t, tr.R(), tr.hasR());
}


inline vectorTensorTransform operator+
(
    const vector& t,
    const vectorTensorTransform& tr
)
{
    return vectorTensorTransform(t + tr.t(), tr.R(), tr.hasR());
}


inline vectorTensorTransform operator-
(
    const vectorTensorTransform& tr,
    const vector& t
)
{
    return vectorTensorTransform(tr.t() - t, tr.R(), tr.hasR());
}


inline vectorTensorTransform operator&
(
    const vectorTensorTransform& tr1,
    const vectorTensorTransform& tr2
)
{
    return vectorTensorTransform
    (
        tr1.t() + tr2.t(),
        tr1.R() & tr2.R(),
        (tr1.hasR() || tr2.hasR())
    );
}


// ************************************************************************* //

 } // End namespace Foam
