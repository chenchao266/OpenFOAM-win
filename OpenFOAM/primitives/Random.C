﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2017-2020 OpenCFD Ltd.
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

#include "Random.H"
#include "PstreamReduceOps.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


 namespace Foam{
Random::Random(const label seedValue)
:
    seed_(seedValue),
    generator_(seed_),
    uniform01_(),
    hasGaussSample_(false),
    gaussSample_(0)
{}


Random::Random(const Random& rnd, const bool reset)
:
    Random(rnd)
{
    if (reset)
    {
        hasGaussSample_ = false;
        gaussSample_ = 0;
        generator_.seed(seed_);
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<>
scalar Random::sample01()
{
    return scalar01();
}


template<>
label Random::sample01()
{
    return round(scalar01());
}


template<>
scalar Random::GaussNormal()
{
    if (hasGaussSample_)
    {
        hasGaussSample_ = false;
        return gaussSample_;
    }

    // Gaussian random number as per Knuth/Marsaglia.
    // Input: two uniform random numbers, output: two Gaussian random numbers.
    // cache one of the values for the next call.
    scalar rsq, v1, v2;
    do
    {
        v1 = 2*scalar01() - 1;
        v2 = 2*scalar01() - 1;
        rsq = sqr(v1) + sqr(v2);
    } while (rsq >= 1 || rsq == 0);

    const scalar fac = sqrt(-2*log(rsq)/rsq);

    gaussSample_ = v1*fac;
    hasGaussSample_ = true;

    return v2*fac;
}


template<>
label Random::GaussNormal()
{
    return round(GaussNormal<scalar>());
}


template<>
scalar Random::position
(
    const scalar& start,
    const scalar& end
)
{
    return start + scalar01()*(end - start);
}


template<>
label Random::position(const label& start, const label& end)
{
    #ifdef FULLDEBUG
    if (start > end)
    {
        FatalErrorInFunction
            << "start index " << start << " > end index " << end << nl
            << abort(FatalError);
    }
    #endif

    // Extend the upper sampling range by 1 and floor the result.
    // Since the range is non-negative, can use integer truncation
    // instead using floor().

    const label val = start + label(scalar01()*(end - start + 1));

    // Rare case when scalar01() returns exactly 1.000 and the truncated
    // value would be out of range.
    return min(val, end);
}


template<>
scalar Random::globalSample01()
{
    scalar value(-GREAT);

    if (Pstream::master())
    {
        value = scalar01();
    }

    Pstream::scatter(value);

    return value;
}


template<>
label Random::globalSample01()
{
    label value(labelMin);

    if (Pstream::master())
    {
        value = round(scalar01());
    }

    Pstream::scatter(value);

    return value;
}


template<>
scalar Random::globalGaussNormal()
{
    scalar value(-GREAT);

    if (Pstream::master())
    {
        value = GaussNormal<scalar>();
    }

    Pstream::scatter(value);

    return value;
}


template<>
label Random::globalGaussNormal()
{
    label value(labelMin);

    if (Pstream::master())
    {
        value = GaussNormal<label>();
    }

    Pstream::scatter(value);

    return value;
}


template<>
scalar Random::globalPosition
(
    const scalar& start,
    const scalar& end
)
{
    scalar value(-GREAT);

    if (Pstream::master())
    {
        value = position<scalar>(start, end);
    }

    Pstream::scatter(value);

    return value;
}


template<>
label Random::globalPosition
(
    const label& start,
    const label& end
)
{
    label value(labelMin);

    if (Pstream::master())
    {
        value = position<label>(start, end);
    }

    Pstream::scatter(value);

    return value;
}


// ************************************************************************* //

 } // End namespace Foam
