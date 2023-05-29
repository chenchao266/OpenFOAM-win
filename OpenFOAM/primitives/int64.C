/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2014-2016 OpenFOAM Foundation
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

#include "int64.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


 namespace Foam{
const char* const pTraits<int64_t>::componentNames[] = { "" };

const int64_t pTraits<int64_t>::zero_ = 0;
const int64_t pTraits<int64_t>::one_ = 1;
const int64_t pTraits<int64_t>::min_ = INT64_MIN;
const int64_t pTraits<int64_t>::max_ = INT64_MAX;
const int64_t pTraits<int64_t>::rootMin_ = pTraits<int64_t>::min_;
const int64_t pTraits<int64_t>::rootMax_ = pTraits<int64_t>::max_;


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

pTraits<int64_t>::pTraits(const int64_t& val) noexcept
:
    p_(val)
{}


pTraits<int64_t>::pTraits(Istream& is)
{
    is >> p_;
}


// ************************************************************************* //

 } // End namespace Foam
