/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2017-2021 OpenCFD Ltd.
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

#include "orientedType.H"
#include "dictionary2.H"
#include "_Istream.H"
#include "_Ostream.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


 namespace Foam{
const Enum
<
    orientedType::orientedOption
>
orientedType::orientedOptionNames
({
    { orientedOption::UNKNOWN, "unknown" },
    { orientedOption::ORIENTED, "oriented" },
    { orientedOption::UNORIENTED, "unoriented" },
});


// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

bool orientedType::checkType
(
    const orientedType& ot1,
    const orientedType& ot2
)
{
    return
    (
        (ot1.oriented() == UNKNOWN)
     || (ot2.oriented() == UNKNOWN)
     || (ot1.oriented() == ot2.oriented())
    );
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

orientedType::orientedType()
:
    oriented_(UNKNOWN)
{}


orientedType::orientedType(const orientedType& ot)
:
    oriented_(ot.oriented_)
{}


orientedType::orientedType(const bool oriented)
:
    oriented_(oriented ? ORIENTED : UNORIENTED)
{}


orientedType::orientedType(Istream& is)
:
    oriented_(orientedOptionNames.read(is))
{
    is.check(FUNCTION_NAME);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

orientedType::orientedOption& orientedType::oriented() noexcept
{
    return oriented_;
}


orientedType::orientedOption orientedType::oriented() const noexcept
{
    return oriented_;
}


void orientedType::setOriented(const bool oriented) noexcept
{
    oriented_ = oriented ? ORIENTED : UNORIENTED;
}


void orientedType::read(const dictionary& dict)
{
    oriented_ = orientedOptionNames.getOrDefault
    (
        "oriented",
        dict,
        orientedOption::UNKNOWN,
        true  // Failsafe behaviour
    );
}


bool orientedType::writeEntry(Ostream& os) const
{
    const bool output = (oriented_ == ORIENTED);

    if (output)
    {
        os.writeEntry("oriented", orientedOptionNames[oriented_]);
    }

    return output;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

void orientedType::operator=(const orientedType& ot)
{
    // Oriented state is inherited on assignment
    oriented_ = ot.oriented();
}


void orientedType::operator+=(const orientedType& ot)
{
    // Set the oriented status if it was unknown
    if (oriented_ == UNKNOWN)
    {
        oriented_ = ot.oriented();
    }

    if (!checkType(*this, ot))
    {
        FatalErrorInFunction
            << "Operator += is undefined for "
            << orientedOptionNames[oriented_] << " and "
            << orientedOptionNames[ot.oriented()] << " types"
            << abort(FatalError);
    }
}


void orientedType::operator-=(const orientedType& ot)
{
    // Set the oriented status if it was unknown
    if (oriented_ == UNKNOWN)
    {
        oriented_ = ot.oriented();
    }

    if (!checkType(*this, ot))
    {
        FatalErrorInFunction
            << "Operator -= is undefined for "
            << orientedOptionNames[oriented_] << " and "
            << orientedOptionNames[ot.oriented()] << " types"
            << abort(FatalError);
    }
}


void orientedType::operator*=(const orientedType& ot)
{
    const orientedType& ot1 = *this;
    if (ot1() ^ ot())
    {
        oriented_ = ORIENTED;
    }
    else
    {
        oriented_ = UNORIENTED;
    }
}


void orientedType::operator/=(const orientedType& ot)
{
    const orientedType& ot1 = *this;
    if (ot1() ^ ot())
    {
        oriented_ = ORIENTED;
    }
    else
    {
        oriented_ = UNORIENTED;
    }
}


void orientedType::operator*=(const scalar s)
{
    // No change to oriented_ flag
}


void orientedType::operator/=(const scalar s)
{
    // No change to oriented_ flag
}


bool orientedType::operator()() const
{
    return oriented_ == ORIENTED;
}


// * * * * * * * * * * * * * * * Friend Functions  * * * * * * * * * * * * * //

orientedType max(const orientedType& ot1, const orientedType& ot2)
{
    if (!orientedType::checkType(ot1, ot2))
    {
        FatalErrorInFunction
            << "Operator max is undefined for "
            << orientedType::orientedOptionNames[ot1.oriented()] << " and "
            << orientedType::orientedOptionNames[ot2.oriented()] << " types"
            << abort(FatalError);
    }

    return ot1;
}


orientedType min(const orientedType& ot1, const orientedType& ot2)
{
    if (!orientedType::checkType(ot1, ot2))
    {
        FatalErrorInFunction
            << "Operator min is undefined for "
            << orientedType::orientedOptionNames[ot1.oriented()] << " and "
            << orientedType::orientedOptionNames[ot2.oriented()] << "types"
            << abort(FatalError);
    }

    return ot1;
}


orientedType cmptMultiply
(
    const orientedType& ot1,
    const orientedType& ot2
)
{
    return ot1 ^ ot2;
}


orientedType cmptDivide
(
    const orientedType& ot1,
    const orientedType& ot2
)
{
    return ot1 ^ ot2;
}


orientedType cmptAv(const orientedType& ot)
{
    return ot;
}


orientedType pow(const orientedType& ot, const scalar r)
{
    // Undefined???
    // - only defined for integers where:
    //   - odd powers = oriented_ = yes (if ot is oriented)
    //   - even powers = oriented_ = no
    return ot;
}


orientedType sqr(const orientedType& ot)
{
    return orientedType(false);
}


orientedType pow3(const orientedType& ot)
{
    return ot;
}


orientedType pow4(const orientedType& ot)
{
    return orientedType(false);
}


orientedType pow5(const orientedType& ot)
{
    return ot;
}


orientedType pow6(const orientedType& ot)
{
    return orientedType(false);
}


orientedType pow025(const orientedType& ot)
{
    return ot;
}


orientedType sqrt(const orientedType& ot)
{
    return ot;
}


orientedType cbrt(const orientedType& ot)
{
    return ot;
}


orientedType magSqr(const orientedType& ot)
{
    return orientedType(false);
}


orientedType  mag(const orientedType& ot)
{
    return orientedType(false);
}


orientedType  sign(const orientedType& ot)
{
    return ot;
}


orientedType  pos(const orientedType& ot)
{
    return ot;
}


orientedType  pos0(const orientedType& ot)
{
    return ot;
}


orientedType  neg(const orientedType& ot)
{
    return ot;
}


orientedType  neg0(const orientedType& ot)
{
    return ot;
}


orientedType  posPart(const orientedType& ot)
{
    return ot;
}


orientedType  negPart(const orientedType& ot)
{
    return ot;
}


orientedType  inv(const orientedType& ot)
{
    return ot;
}


orientedType trans(const orientedType& ot)
{
    return ot;
}


orientedType atan2
(
    const orientedType& ot1,
    const orientedType& ot2
)
{
    if (!orientedType::checkType(ot1, ot2))
    {
        FatalErrorInFunction
            << "Operator atan2 is undefined for "
            << orientedType::orientedOptionNames[ot1.oriented()] << " and "
            << orientedType::orientedOptionNames[ot2.oriented()] << "types"
            << abort(FatalError);
    }

    return ot1;
}


orientedType hypot
(
    const orientedType& ot1,
    const orientedType& ot2
)
{
    if (!orientedType::checkType(ot1, ot2))
    {
        FatalErrorInFunction
            << "Operator hypot is undefined for "
            << orientedType::orientedOptionNames[ot1.oriented()] << " and "
            << orientedType::orientedOptionNames[ot2.oriented()] << "types"
            << abort(FatalError);
    }

    return ot1;
}


orientedType transform(const orientedType& ot)
{
    return ot;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

Istream& operator>>(Istream& is, orientedType& ot)
{
    ot.oriented_ = orientedType::orientedOptionNames.read(is);

    is.check(FUNCTION_NAME);

    return is;
}


Ostream& operator<<(Ostream& os, const orientedType& ot)
{
    os << orientedType::orientedOptionNames[ot.oriented()];

    os.check(FUNCTION_NAME);

    return os;
}


// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

orientedType operator+
(
    const orientedType& ot1,
    const orientedType& ot2
)
{
    if (!orientedType::checkType(ot1, ot2))
    {
        FatalErrorInFunction
            << "Operator + is undefined for "
            << orientedType::orientedOptionNames[ot1.oriented()] << " and "
            << orientedType::orientedOptionNames[ot2.oriented()] << " types"
            << abort(FatalError);
    }

    // Note use of () operators to convert to boolean op
    return orientedType(ot1() || ot2());
}


orientedType operator-(const orientedType& ot)
{
    return ot;
}


orientedType operator-
(
    const orientedType& ot1,
    const orientedType& ot2
)
{
    if (!orientedType::checkType(ot1, ot2))
    {
        FatalErrorInFunction
            << "Operator - is undefined for "
            << orientedType::orientedOptionNames[ot1.oriented()] << " and "
            << orientedType::orientedOptionNames[ot2.oriented()] << " types"
            << abort(FatalError);
    }

    // Note use of () operators to convert to boolean op
    return orientedType(ot1() || ot2());
}


orientedType operator*(const scalar s, const orientedType& ot)
{
    return ot;
}


orientedType operator/(const orientedType& ot, const scalar s)
{
    return ot;
}


orientedType operator/
(
    const orientedType& ot1,
    const orientedType& ot2
)
{
    return ot1 ^ ot2;
}


orientedType operator*
(
    const orientedType& ot1,
    const orientedType& ot2
)
{
    return ot1 ^ ot2;
}


orientedType operator^
(
    const orientedType& ot1,
    const orientedType& ot2
)
{
    // Note use of () operators to convert to boolean op
    return orientedType(ot1() ^ ot2());
}


orientedType operator&
(
    const orientedType& ot1,
    const orientedType& ot2
)
{
    return ot1 ^ ot2;
}


orientedType operator&&
(
    const orientedType& ot1,
    const orientedType& ot2
)
{
    return orientedType(false);
}


// ************************************************************************* //

 } // End namespace Foam
