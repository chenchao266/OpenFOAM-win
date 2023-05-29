/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
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

#include "dimensionSet.H"
#include "dimensionedScalar.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(dimensionSet, 1);
 

const scalar dimensionSet::smallExponent = SMALL;


// * * * * * * * * * * * * * * * Local Functions * * * * * * * * * * * * * * //

 

static inline bool checkDims
(
    const char* what,
    const dimensionSet& a,
    const dimensionSet& b
)
{
    if (a != b)
    {
        FatalErrorInFunction
            << "Different dimensions for '" << what
            << "'\n     dimensions : " << a << " != " << b << nl
            << abort(FatalError);
        return false;
    }

    return true;
}

 


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

 
dimensionSet::dimensionSet()
:
    exponents_(Zero)
{}


dimensionSet::dimensionSet
(
    const scalar mass,
    const scalar length,
    const scalar time,
    const scalar temperature,
    const scalar moles,
    const scalar current,
    const scalar luminousIntensity
)
:
    exponents_()
{
    exponents_[MASS] = mass;
    exponents_[LENGTH] = length;
    exponents_[TIME] = time;
    exponents_[TEMPERATURE] = temperature;
    exponents_[MOLES] = moles;
    exponents_[CURRENT] = current;
    exponents_[LUMINOUS_INTENSITY] = luminousIntensity;
}


dimensionSet::dimensionSet(const FixedList<scalar,7>& dims)
:
    exponents_(dims)
{}


dimensionSet::dimensionSet(const dimensionSet& ds)
:
    exponents_(ds.exponents_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool dimensionSet::dimensionless() const
{
    for (const scalar val : exponents_)
    {
        // ie, mag(val) > smallExponent
        if ((val > smallExponent) || (val < -smallExponent))
        {
            return false;
        }
    }

    return true;
}


const FixedList<scalar,7>&
dimensionSet::values() const noexcept
{
    return exponents_;
}


FixedList<scalar,7>&
dimensionSet::values() noexcept
{
    return exponents_;
}


void dimensionSet::clear()
{
    exponents_ = Zero;
}


void dimensionSet::reset(const dimensionSet& ds)
{
    exponents_ = ds.exponents_;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

scalar dimensionSet::operator[](const dimensionType type) const
{
    return exponents_[type];
}


scalar& dimensionSet::operator[](const dimensionType type)
{
    return exponents_[type];
}


scalar dimensionSet::operator[](const label type) const
{
    return exponents_[type];
}


scalar& dimensionSet::operator[](const label type)
{
    return exponents_[type];
}


bool dimensionSet::operator==(const dimensionSet& ds) const
{
    for (int d=0; d<nDimensions; ++d)
    {
        if
        (
            mag(exponents_[d] - ds.exponents_[d])
          > smallExponent
        )
        {
            return false;
        }
    }

    return true;
}


bool dimensionSet::operator!=(const dimensionSet& ds) const
{
    return !(operator==(ds));
}


bool dimensionSet::operator=(const dimensionSet& ds) const
{
    if (dimensionSet::checking())
    {
        checkDims("(a = b)", *this, ds);
    }

    return true;
}


bool dimensionSet::operator+=(const dimensionSet& ds) const
{
    if (dimensionSet::checking())
    {
        checkDims("(a += b)", *this, ds);
    }

    return true;
}


bool dimensionSet::operator-=(const dimensionSet& ds) const
{
    if (dimensionSet::checking())
    {
        checkDims("(a -= b)", *this, ds);
    }

    return true;
}


bool dimensionSet::operator*=(const dimensionSet& ds)
{
    reset((*this)*ds);

    return true;
}


bool dimensionSet::operator/=(const dimensionSet& ds)
{
    reset((*this)/ds);

    return true;
}


// * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * * //

dimensionSet min(const dimensionSet& ds1, const dimensionSet& ds2)
{
    if (dimensionSet::checking())
    {
        checkDims("min(a, b)", ds1, ds2);
    }

    return ds1;
}


dimensionSet max(const dimensionSet& ds1, const dimensionSet& ds2)
{
    if (dimensionSet::checking())
    {
        checkDims("max(a, b)", ds1, ds2);
    }

    return ds1;
}


dimensionSet clip(const dimensionSet& ds1, const dimensionSet& ds2)
{
    if (dimensionSet::checking())
    {
        checkDims("clip(a, b)", ds1, ds2);
    }

    return ds1;
}


dimensionSet cmptMultiply
(
    const dimensionSet& ds1,
    const dimensionSet& ds2
)
{
    return ds1*ds2;
}


dimensionSet cmptDivide
(
    const dimensionSet& ds1,
    const dimensionSet& ds2
)
{
    return ds1/ds2;
}


dimensionSet pow(const dimensionSet& ds, const scalar p)
{
    return dimensionSet
    (
        ds[dimensionSet::MASS]*p,
        ds[dimensionSet::LENGTH]*p,
        ds[dimensionSet::TIME]*p,
        ds[dimensionSet::TEMPERATURE]*p,
        ds[dimensionSet::MOLES]*p,
        ds[dimensionSet::CURRENT]*p,
        ds[dimensionSet::LUMINOUS_INTENSITY]*p
    );
}


dimensionSet pow
(
    const dimensionSet& ds,
    const dimensionedScalar& dS
)
{
    if (dimensionSet::checking() && !dS.dimensions().dimensionless())
    {
        FatalErrorInFunction
            << "Exponent of pow is not dimensionless" << endl
            << abort(FatalError);
    }

    return pow(ds, dS.value());
}


dimensionSet pow
(
    const dimensionedScalar& dS,
    const dimensionSet& ds
)
{
    if
    (
        dimensionSet::checking()
     && !dS.dimensions().dimensionless()
     && !ds.dimensionless()
    )
    {
        FatalErrorInFunction
            << "Argument or exponent of pow not dimensionless" << endl
            << abort(FatalError);
    }

    return ds;
}


dimensionSet sqr(const dimensionSet& ds)
{
    return pow(ds, 2);
}


dimensionSet pow2(const dimensionSet& ds)
{
    return pow(ds, 2);
}


dimensionSet pow3(const dimensionSet& ds)
{
    return pow(ds, 3);
}


dimensionSet pow4(const dimensionSet& ds)
{
    return pow(ds, 4);
}


dimensionSet pow5(const dimensionSet& ds)
{
    return pow(ds, 5);
}


dimensionSet pow6(const dimensionSet& ds)
{
    return pow(ds, 6);
}


dimensionSet pow025(const dimensionSet& ds)
{
    return pow(ds, 0.25);
}


dimensionSet sqrt(const dimensionSet& ds)
{
    return pow(ds, 0.5);
}


dimensionSet cbrt(const dimensionSet& ds)
{
    return pow(ds, 1.0/3.0);
}


dimensionSet magSqr(const dimensionSet& ds)
{
    return pow(ds, 2);
}


dimensionSet mag(const dimensionSet& ds)
{
    return ds;
}


dimensionSet sign(const dimensionSet&)
{
    return dimless;
}


dimensionSet pos(const dimensionSet&)
{
    return dimless;
}


dimensionSet pos0(const dimensionSet&)
{
    return dimless;
}


dimensionSet neg(const dimensionSet&)
{
    return dimless;
}


dimensionSet neg0(const dimensionSet&)
{
    return dimless;
}


dimensionSet posPart(const dimensionSet& ds)
{
    return ds;
}


dimensionSet negPart(const dimensionSet& ds)
{
    return ds;
}


dimensionSet inv(const dimensionSet& ds)
{
    return dimensionSet
    (
        0.0-ds[dimensionSet::MASS],
        0.0-ds[dimensionSet::LENGTH],
        0.0-ds[dimensionSet::TIME],
        0.0-ds[dimensionSet::TEMPERATURE],
        0.0-ds[dimensionSet::MOLES],
        0.0-ds[dimensionSet::CURRENT],
        0.0-ds[dimensionSet::LUMINOUS_INTENSITY]
    );
}


dimensionSet trans(const dimensionSet& ds)
{
    if (dimensionSet::checking() && !ds.dimensionless())
    {
        FatalErrorInFunction
            << "Argument of trancendental function not dimensionless" << nl
            << abort(FatalError);
    }

    return ds;
}


dimensionSet atan2(const dimensionSet& ds1, const dimensionSet& ds2)
{
    if (dimensionSet::checking())
    {
        checkDims("atan2(a, b)", ds1, ds2);
    }

    return dimless;
}


dimensionSet hypot(const dimensionSet& ds1, const dimensionSet& ds2)
{
    if (dimensionSet::checking())
    {
        checkDims("hypot(a, b)", ds1, ds2);
    }

    return ds1;
}


dimensionSet transform(const dimensionSet& ds)
{
    return ds;
}


dimensionSet invTransform(const dimensionSet& ds)
{
    return ds;
}


// * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * * //

dimensionSet operator~(const dimensionSet& ds)
{
    return inv(ds);
}


dimensionSet operator-(const dimensionSet& ds)
{
    return ds;
}


dimensionSet operator+
(
    const dimensionSet& ds1,
    const dimensionSet& ds2
)
{
    if (dimensionSet::checking())
    {
        checkDims("(a + b)", ds1, ds2);
    }

    return ds1;
}


dimensionSet operator-
(
    const dimensionSet& ds1,
    const dimensionSet& ds2
)
{
    if (dimensionSet::checking())
    {
        checkDims("(a - b)", ds1, ds2);
    }

    return ds1;
}


dimensionSet operator*
(
    const dimensionSet& ds1,
    const dimensionSet& ds2
)
{
    dimensionSet result(ds1);

    auto rhs = ds2.values().begin();

    for (scalar& val : result.values())
    {
        val += *rhs;
        ++rhs;
    }

    return result;
}


dimensionSet operator/
(
    const dimensionSet& ds1,
    const dimensionSet& ds2
)
{
    dimensionSet result(ds1);

    auto rhs = ds2.values().begin();

    for (scalar& val : result.values())
    {
        val -= *rhs;
        ++rhs;
    }

    return result;
}


dimensionSet operator&
(
    const dimensionSet& ds1,
    const dimensionSet& ds2
)
{
    return ds1*ds2;
}


dimensionSet operator^
(
    const dimensionSet& ds1,
    const dimensionSet& ds2
)
{
    return ds1*ds2;
}


dimensionSet operator&&
(
    const dimensionSet& ds1,
    const dimensionSet& ds2
)
{
    return ds1*ds2;
}


// ************************************************************************* //

 } // End namespace Foam
