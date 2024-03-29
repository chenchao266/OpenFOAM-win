﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011 OpenFOAM Foundation
    Copyright (C) 2021 OpenCFD Ltd.
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

#include "IOstreams.H"

// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //


 namespace Foam{
template<class T>
inline Istream& operator>>
(
    Istream& is,
    LabelledItem<T>& item
)
{
    is.readBegin("LabelledItem");
    is >> static_cast<T&>(item) >> item.index();
    is.readEnd("LabelledItem");

    is.check(FUNCTION_NAME);
    return is;
}


template<class T>
inline Ostream& operator<<
(
    Ostream& os,
    const LabelledItem<T>& item
)
{
    // Output like Tuple2
    os  << token::BEGIN_LIST
        << static_cast<const T&>(item) << token::SPACE
        << item.index()
        << token::END_LIST;

    return os;
}


// ************************************************************************* //

 } // End namespace Foam
