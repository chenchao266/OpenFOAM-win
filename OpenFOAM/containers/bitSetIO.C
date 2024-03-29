﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
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

#include "bitSet.H"
#include "IOstreams.H"

// * * * * * * * * * * * * * * Ostream Operators * * * * * * * * * * * * * * //


 namespace Foam{
Ostream& operator<<(Ostream& os, const bitSet& bitset)
{
    return bitset.writeList(os, 40);
}


Ostream& operator<<
(
    Ostream& os,
    const InfoProxy<bitSet>& iproxy
)
{
    const bitSet& bitset = iproxy.t_;

    os  << "bitSet<" << bitSet::elem_per_block
        << "> size=" << bitset.size() << "/" << bitset.capacity()
        << " count=" << bitset.count()
        << nl;

    return os;
}


// ************************************************************************* //

 } // End namespace Foam
