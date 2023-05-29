/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2018-2021 OpenCFD Ltd.
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

#include "bool.H"
#include "Switch.H"
#include "error.H"
#include "IOstreams.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


 namespace Foam{
const char* const pTraits<bool>::typeName = "bool";
const char* const pTraits<bool>::componentNames[] = { "" };

const bool pTraits<bool>::zero_ = false;
const bool pTraits<bool>::one_ = true;


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

pTraits<bool>::pTraits(const bool& p) noexcept
:
    p_(p)
{}


pTraits<bool>::pTraits(Istream& is)
{
    is >> p_;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

Istream& operator>>(Istream& is, bool& b)
{
    b = static_cast<bool>(Switch(is));
    return is;
}


Ostream& operator<<(Ostream& os, const bool b)
{
    // Emit as label (not byte etc) for proper send/receive in parallel
    os.write(static_cast<label>(b));
    os.check(FUNCTION_NAME);
    return os;
}


bool readBool(Istream& is)
{
    return static_cast<bool>(Switch(is));
}


// ************************************************************************* //

 } // End namespace Foam
