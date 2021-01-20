/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2017 OpenFOAM Foundation
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

#include "instantList.H"
#include "Time.T.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //
namespace Foam {
    const char* const instant::typeName = "instant";

    // * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

    instant::instant()
    {}

    instant::instant(const scalar val, const word& tname) : value_(val),
        name_(tname)
    {}

    instant::instant(const scalar val) : value_(val),
        name_(Time::timeName(val))
    {}

    instant::instant(const word& tname) : value_(atof(tname.c_str())),
        name_(tname)
    {}


    // * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

    bool instant::equal(const scalar b) const
    {
        return (value_ < b + SMALL && value_ > b - SMALL);
    }


    // * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

    bool operator==(const instant& a, const instant& b)
    {
        return a.equal(b.value_);
    }


    bool operator!=(const instant& a, const instant& b)
    {
        return !operator==(a, b);
    }


    bool operator<(const instant& a, const instant& b)
    {
        return a.value_ < b.value_;
    }


    bool operator>(const instant& a, const instant& b)
    {
        return a.value_ > b.value_;
    }


    // * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

    Istream& operator>>(Istream& is, instant& I)
    {
        is >> I.value_ >> I.name_;

        return is;
    }


    Ostream& operator<<(Ostream& os, const instant& I)
    {
        os << I.value_ << tab << I.name_;

        return os;
    }

}
// ************************************************************************* //
