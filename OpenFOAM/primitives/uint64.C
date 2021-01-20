/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2014-2016 OpenFOAM Foundation
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

#include "uint64.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
namespace Foam {
    const uint64_t pTraits<uint64_t>::_zero = 0;
    const uint64_t pTraits<uint64_t>::one = 1;
    const uint64_t pTraits<uint64_t>::min = 0;
    const uint64_t pTraits<uint64_t>::max = UINT64_MAX;
    const uint64_t pTraits<uint64_t>::rootMin = 0;
    const uint64_t pTraits<uint64_t>::rootMax = pTraits<uint64_t>::max;

    const char* const pTraits<uint64_t>::componentNames[] = { "" };

    pTraits<uint64_t>::pTraits(const uint64_t& p) : p_(p)
    {}

    pTraits<uint64_t>::pTraits(Istream& is)
    {
        is >> p_;
    }
}

// ************************************************************************* //
