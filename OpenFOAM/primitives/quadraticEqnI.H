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
inline quadraticEqn::quadraticEqn()
{}


inline quadraticEqn::quadraticEqn(const zero)
:
    VectorSpace<quadraticEqn, scalar, 3>(zero{})
{}


inline quadraticEqn::quadraticEqn
(
    const scalar a,
    const scalar b,
    const scalar c
)
{
    this->v_[A] = a;
    this->v_[B] = b;
    this->v_[C] = c;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline scalar quadraticEqn::a() const
{
    return this->v_[A];
}


inline scalar quadraticEqn::b() const
{
    return this->v_[B];
}


inline scalar quadraticEqn::c() const
{
    return this->v_[C];
}


inline scalar& quadraticEqn::a()
{
    return this->v_[A];
}


inline scalar& quadraticEqn::b()
{
    return this->v_[B];
}


inline scalar& quadraticEqn::c()
{
    return this->v_[C];
}


inline scalar quadraticEqn::value(const scalar x) const
{
    return x*(x*a() + b()) + c();
}


inline scalar quadraticEqn::derivative(const scalar x) const
{
    return x*2*a() + b();
}


inline scalar quadraticEqn::error(const scalar x) const
{
    return
        SMALL*mag(x)*(mag(x*a()) + mag(b()))
      + SMALL*(mag(x*(x*a() + b())) + mag(c()));
}


// ************************************************************************* //

 } // End namespace Foam
