/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2017 OpenFOAM Foundation
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

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //
namespace Foam {
inline linearEqn::linearEqn()
{}


inline linearEqn::linearEqn(const zero) :    VectorSpace<linearEqn, scalar, 2>(zero())
{}


inline linearEqn::linearEqn(const scalar a, const scalar b)
{
    this->v_[A] = a;
    this->v_[B] = b;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline scalar linearEqn::a() const
{
    return this->v_[A];
}


inline scalar linearEqn::b() const
{
    return this->v_[B];
}


inline scalar& linearEqn::a()
{
    return this->v_[A];
}


inline scalar& linearEqn::b()
{
    return this->v_[B];
}


inline scalar linearEqn::value(const scalar x) const
{
    return x*a() + b();
}


inline scalar linearEqn::derivative(const scalar x) const
{
    return a();
}


inline scalar linearEqn::error(const scalar x) const
{
    return mag(SMALL*x*a());
}


inline Roots<1> linearEqn::roots() const
{
    /*

    This function solves a linear equation of the following form:

        a*x + b = 0
          x + B = 0

    */

    const scalar a = this->a();
    const scalar b = this->b();

    if (a == 0)
    {
        return Roots<1>(roots::nan, 0);
    }

    if (mag(b/VGREAT) >= mag(a))
    {
        return Roots<1>(sign(a) == sign(b) ? roots::negInf : roots::posInf, 0);
    }

    return Roots<1>(roots::real, - b/a);
}

}
// ************************************************************************* //
