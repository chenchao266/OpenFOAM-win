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

#include "uint32.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


 namespace Foam{
const char* const pTraits<uint32_t>::componentNames[] = { "" };

const uint32_t pTraits<uint32_t>::zero_ = 0;
const uint32_t pTraits<uint32_t>::one_ = 1;
const uint32_t pTraits<uint32_t>::min_ = 0;
const uint32_t pTraits<uint32_t>::max_ = UINT32_MAX;
const uint32_t pTraits<uint32_t>::rootMin_ = 0;
const uint32_t pTraits<uint32_t>::rootMax_ = pTraits<uint32_t>::max_;


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

pTraits<uint32_t>::pTraits(const uint32_t& val) noexcept
:
    p_(val)
{}


pTraits<uint32_t>::pTraits(Istream& is)
{
    is >> p_;
}


// ************************************************************************* //

 } // End namespace Foam
