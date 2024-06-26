﻿#include "scalar.H"
#include "vector2.H"
#include "curveTools.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

scalar distance(const vector& p1, const vector& p2)
{
    return ::Foam::mag(p2 - p1);
}

scalar distance(const vector& p1, const scalar p2)
{
    return 0;
}

bool stepForwardsToNextPoint
(
    const vector& o,
    vector& n,
    label& i,
    label& ip1,
    scalar l,
    const curve& Curve
)
{
    label ip1n = ip1-1;
    while (++ip1n < Curve.size() && ::Foam::distance(o, Curve[ip1n]) < l);
    label in = ip1n - 1;

    bool eoc = true;

    if (ip1n < Curve.size() && in >= 0)
    {
        eoc = ::Foam::interpolate(Curve[in], Curve[ip1n], o, n, l);

        i = in;
        ip1 = ip1n;
    }

    return eoc;
}


bool stepBackwardsToNextPoint
(
    const vector& o,
    vector& n,
    label& i,
    label& ip1,
    scalar l,
    const curve& Curve
)
{
    label ip1n = ip1+1;
    while (--ip1n >= 0 && ::Foam::distance(o, Curve[ip1n]) < l);
    label in = ip1n + 1;

    bool eoc = true;

    if (ip1n >= 0 && in < Curve.size())
    {
        eoc = ::Foam::interpolate(Curve[in], Curve[ip1n], o, n, l);

        i = in;
        ip1 = ip1n;
    }

    return eoc;
}

bool interpolate
(
    const scalar p1,
    const scalar p2,
    const vector& o,
    vector& n,
    scalar l
)
{
    return false;
}
bool interpolate
(
    const vector& p1,
    const vector& p2,
    const vector& o,
    vector& n,
    scalar l
)
{
    vector D = p1 - p2;
    scalar a = magSqr(D);
    scalar b = 2.0*(D&(p2 - o));
    scalar c = magSqr(p2) + (o&(o - 2.0*p2)) - l*l;

    scalar b2m4ac = b*b - 4.0*a*c;

    if (b2m4ac >= 0.0)
    {
        scalar srb2m4ac = sqrt(b2m4ac);

        scalar lamda = (-b - srb2m4ac)/(2.0*a);

        if (lamda > 1.0+curveSmall || lamda < -curveSmall)
        {
            lamda = (-b + srb2m4ac)/(2.0*a);
        }

        if (lamda < 1.0+curveSmall && lamda > -curveSmall)
        {
            n = p2 + lamda*(p1 - p2);

            return false;
        }
        else
        {
            return true;
        }
    }

    return true;
}



bool XstepForwardsToNextPoint
(
    const vector& o,
    vector& n,
    label& i,
    label& ip1,
    scalar l,
    const curve& Curve
)
{
    label ip1n = ip1-1;
    while (++ip1n < Curve.size() && mag(o.x() - Curve[ip1n]/*.x()*/) < l);
    label in = ip1n - 1;

    bool eoc = true;

    if (ip1n < Curve.size() && in >= 0)
    {
        eoc = ::Foam::Xinterpolate(Curve[in], Curve[ip1n], o, n, l);

        i = in;
        ip1 = ip1n;
    }

    return eoc;
}

bool Xinterpolate
(
    const scalar p1,
    const scalar p2,
    const vector& o,
    vector& n,
    scalar l
)
{
    return false;
}

bool Xinterpolate
(
    const vector& p1,
    const vector& p2,
    const vector& o,
    vector& n,
    scalar l
)
{
    n.x() = o.x() + l;

    if (p2.x() < o.x() + l - curveSmall && p2.x() > o.x() - l + curveSmall)
    {
        return true;
    }

    if (p2.x() < o.x() + l)
    {
        n.x() = o.x() - l;
    }

    vector D = p2 - p1;
    scalar lamda = (n.x() - p1.x())/D.x();
    n.y() = p1.y() + lamda*D.y();

    return false;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
