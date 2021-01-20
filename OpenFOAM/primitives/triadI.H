/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2012-2016 OpenFOAM Foundation
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
    inline triad::triad()
        :
        Vector<vector>(triad::unset)
    {}


    inline triad::triad(const Vector<vector>& vv)
        :
        Vector<vector>(vv)
    {}


    inline triad::triad(const vector& x, const vector& y, const vector& z)
        :
        Vector<vector>(x, y, z)
    {}


    inline triad::triad(const vector& pa)
    {
        operator=(triad::unset);
        operator[](primaryDirection(pa)) = pa;
    }


    inline triad::triad(Istream& is)
        :
        Vector<vector>(is)
    {}


    // * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

    inline bool triad::set(const direction d) const
    {
        return operator[](d)[0] < GREAT;
    }


    inline bool triad::set() const
    {
        return set(0) && set(1) && set(2);
    }


    inline direction triad::primaryDirection(const vector& v)
    {
        if (mag(v.x()) > mag(v.y()) && mag(v.x()) > mag(v.z()))
        {
            return triad::X;
        }
        else if (mag(v.y()) > mag(v.z()))
        {
            return triad::Y;
        }
        else
        {
            return triad::Z;
        }
    }


    inline vector triad::orthogonal
    (
        const vector& v1,
        const vector& v2
    )
    {
        vector v3 = v1 ^ v2;

        scalar magV3 = mag(v3);

        if (magV3 > 0.5)
        {
            return v3 / magV3;
        }
        else
        {
            return triad::unset[0];
        }
    }


    inline void triad::normalize()
    {
        if (set(0)) operator[](0) /= mag(operator[](0));
        if (set(1)) operator[](1) /= mag(operator[](1));
        if (set(2)) operator[](2) /= mag(operator[](2));
    }


    // * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

    inline void triad::operator=(const Vector<vector>& vv)
    {
        Vector<vector>::operator=(vv);
    }


    // * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * * //

    inline Istream& operator>>(Istream& is, triad& t)
    {
        is >> static_cast<Vector<vector>&>(t);
        return is;
    }


    inline Ostream& operator<<(Ostream& os, const triad& t)
    {
        os << static_cast<const Vector<vector>&>(t);
        return os;
    }

}
// ************************************************************************* //
