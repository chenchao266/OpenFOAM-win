/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2017 OpenFOAM Foundation
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
inline cubicEqn::cubicEqn()
{}


inline cubicEqn::cubicEqn(const zero)
:
    VectorSpace<cubicEqn, scalar, 4>(zero{})
{}


inline cubicEqn::cubicEqn
(
    const scalar a,
    const scalar b,
    const scalar c,
    const scalar d
)
{
    this->v_[A] = a;
    this->v_[B] = b;
    this->v_[C] = c;
    this->v_[D] = d;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline scalar cubicEqn::a() const
{
    return this->v_[A];
}


inline scalar cubicEqn::b() const
{
    return this->v_[B];
}


inline scalar cubicEqn::c() const
{
    return this->v_[C];
}


inline scalar cubicEqn::d() const
{
    return this->v_[D];
}


inline scalar& cubicEqn::a()
{
    return this->v_[A];
}


inline scalar& cubicEqn::b()
{
    return this->v_[B];
}


inline scalar& cubicEqn::c()
{
    return this->v_[C];
}


inline scalar& cubicEqn::d()
{
    return this->v_[D];
}


inline scalar cubicEqn::value(const scalar x) const
{
    return x*(x*(x*a() + b()) + c()) + d();
}


inline scalar cubicEqn::derivative(const scalar x) const
{
    return x*(x*3*a() + 2*b()) + c();
}


inline scalar cubicEqn::error(const scalar x) const
{
    return
        SMALL*magSqr(x)*(mag(x*a()) + mag(b()))
      + SMALL*mag(x)*(mag(x*(x*a() + b())) + mag(c()))
      + SMALL*(mag(x*(x*(x*a() + b()) + c())) + mag(d()));
}


// ************************************************************************* //

 } // End namespace Foam
