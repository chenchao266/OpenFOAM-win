/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2019 OpenCFD Ltd.
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

#include "quaternion.H"
#include "IOstreams.H"
#include "StringStream.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


 namespace Foam{
const quaternion quaternion::zero(0, vector(0, 0, 0));
const quaternion quaternion::I(1, vector(0, 0, 0));

const Enum<quaternion::eulerOrder>
quaternion::eulerOrderNames
({
    // Proper Euler angles
    { eulerOrder::XZX, "xzx" },
    { eulerOrder::XYX, "xyx" },
    { eulerOrder::YXY, "yxy" },
    { eulerOrder::YZY, "yzy" },
    { eulerOrder::ZYZ, "zyz" },
    { eulerOrder::ZXZ, "zxz" },

    // Tait-Bryan angles
    { eulerOrder::XZY, "xzy" },
    { eulerOrder::XYZ, "xyz" },
    { eulerOrder::YXZ, "yxz" },
    { eulerOrder::YZX, "yzx" },
    { eulerOrder::ZYX, "zyx" },
    { eulerOrder::ZXY, "zxy" },
});


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

quaternion::quaternion(Istream& is)
{
    is >> *this;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

word name(const quaternion& q)
{
    OStringStream buf;
    buf << '(' << q.w() << ',' << q.v() << ')';
    return buf.str();
}


quaternion slerp
(
    const quaternion& qa,
    const quaternion& qb,
    const scalar t
)
{
    label sign = 1;

    if ((qa & qb) < 0)
    {
        sign = -1;
    }

    return qa*pow((inv(qa)*sign*qb), t);
}


quaternion average
(
    const UList<quaternion>& qs,
    const UList<scalar> w
)
{
    quaternion qa(w[0]*qs[0]);

    for (label i=1; i<qs.size(); i++)
    {
        // Invert quaternion if it has the opposite sign to the average
        if ((qa & qs[i]) > 0)
        {
            qa += w[i]*qs[i];
        }
        else
        {
            qa -= w[i]*qs[i];
        }
    }

    return qa;
}


quaternion exp(const quaternion& q)
{
    const scalar magV = mag(q.v());

    if (magV == 0)
    {
        return quaternion(1, Zero);
    }

    const scalar expW = exp(q.w());

    return quaternion
    (
        expW*cos(magV),
        expW*sin(magV)*q.v()/magV
    );
}


quaternion pow(const quaternion& q, const label power)
{
    const scalar magQ = mag(q);
    const scalar magV = mag(q.v());

    quaternion powq(q.v());

    if (magV != 0 && magQ != 0)
    {
        powq /= magV;
        powq *= power*acos(q.w()/magQ);
    }

    return pow(magQ, (const int)power)*exp(powq);
}


quaternion pow(const quaternion& q, const scalar power)
{
    const scalar magQ = mag(q);
    const scalar magV = mag(q.v());

    quaternion powq(q.v());

    if (magV != 0 && magQ != 0)
    {
        powq /= magV;
        powq *= power*acos(q.w()/magQ);
    }

    return pow(magQ, power)*exp(powq);
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

Istream& operator>>(Istream& is, quaternion& q)
{
    is.readBegin("quaternion");
    is  >> q.w() >> q.v();
    is.readEnd("quaternion");

    is.check(FUNCTION_NAME);
    return is;
}


Ostream& operator<<(Ostream& os, const quaternion& q)
{
    os  << token::BEGIN_LIST
        << q.w() << token::SPACE << q.v()
        << token::END_LIST;

    return os;
}


// ************************************************************************* //

 } // End namespace Foam
