/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011 OpenFOAM Foundation
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

#include "complex.H"
#include "IOstreams.H"
#include "token.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


 namespace Foam{
const char* const pTraits<complex>::typeName = "complex";
const char* const pTraits<complex>::componentNames[] = {"re", "im"};

const complex pTraits<complex>::zero_(0, 0);
const complex pTraits<complex>::one_(1, 0);

const complex pTraits<complex>::min_(-VGREAT, -VGREAT);
const complex pTraits<complex>::max_(VGREAT, VGREAT);

const complex pTraits<complex>::rootMin_
(
    -ROOTVGREAT, -ROOTVGREAT
);

const complex pTraits<complex>::rootMax_
(
    ROOTVGREAT, ROOTVGREAT
);


pTraits<complex>::pTraits(const complex& val)
:
    p_(val)
{}


pTraits<complex>::pTraits(Istream& is)
{
    is >> p_;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

complex::complex(Istream& is)
{
    is >> *this;
}


// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

word name(const complex& c)
{
    return '(' + std::to_string(c.Re()) + ',' + std::to_string(c.Im()) + ')';
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

Istream& operator>>(Istream& is, complex& c)
{
    scalar r, i;

    is.readBegin("complex");
    is >> r >> i;
    is.readEnd("complex");

    c.real(r);
    c.imag(i);

    is.check(FUNCTION_NAME);
    return is;
}


Ostream& operator<<(Ostream& os, const complex& c)
{
    os  << token::BEGIN_LIST
        << c.real() << token::SPACE << c.imag()
        << token::END_LIST;

    return os;
}


// ************************************************************************* //

 } // End namespace Foam
