﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011 OpenFOAM Foundation
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

#include "CompactListList.H"
#include "_Istream.H"

// * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * //


 namespace Foam{
template<class T, class Container>
CompactListList<T, Container>::CompactListList(Istream& is)
{
    operator>>(is, *this);
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class T, class Container>
Istream& operator>>(Istream& is, CompactListList<T, Container>& lst)
{
    is  >> lst.offsets_ >> lst.m_;
    // Note: empty list gets output as two empty lists
    if (lst.offsets_.size())
    {
        lst.size_ = lst.offsets_.size()-1;
    }
    else
    {
        lst.size_ = 0;
    }
    return is;
}


template<class T, class Container>
Ostream& operator<<
(
    Ostream& os,
    const CompactListList<T, Container>& lst
)
{
    os  << lst.offsets_ << lst.m_;
    return os;
}


// ************************************************************************* //

 } // End namespace Foam
