﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011 OpenFOAM Foundation
    Copyright (C) 2016-2021 OpenCFD Ltd.
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

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


 namespace Foam{
template<class ListType>
inline void labelledTri::assignList(const ListType& list)
{
    const label len = list.size();

    // checkSize
    if (len < 3 || len > 4)
    {
         FatalErrorInFunction
            << "size " << len << " != (3 or 4)"
            << abort(FatalError);
    }

    auto iter = list.begin();

    for (label i=0; i<3; ++i)
    {
        this->operator[](i) = *iter;
        ++iter;
    }

    index_ = (len == 4) ? *iter : 0;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline labelledTri::labelledTri()
:
    triFace(),
    index_(-1)
{}


inline labelledTri::labelledTri
(
    const triFace& tri,
    const label region
)
:
    triFace(tri),
    index_(region)
{}


inline labelledTri::labelledTri
(
    const label a,
    const label b,
    const label c,
    const label region
)
:
    triFace(a, b, c),
    index_(region)
{}


inline labelledTri::labelledTri(const labelUList& list)
:
    triFace(),
    index_(0)
{
    assignList(list);
}


inline labelledTri::labelledTri(std::initializer_list<label> list)
:
    triFace(),
    index_(0)
{
    assignList(list);
}


inline labelledTri::labelledTri(Istream& is)
{
    operator>>(is, *this);
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

inline Istream& operator>>(Istream& is, labelledTri& t)
{
    if (is.format() == IOstream::ASCII)
    {
        is.readBegin("labelledTri");

        is  >> static_cast<triFace&>(t) >> t.index();

        is.readEnd("labelledTri");
    }
    else
    {
        Detail::readContiguous<labelledTri>
        (
            is,
            reinterpret_cast<char*>(&t),
            sizeof(labelledTri)
        );
    }

    is.check(FUNCTION_NAME);
    return is;
}


inline Ostream& operator<<(Ostream& os, const labelledTri& t)
{
    if (os.format() == IOstream::ASCII)
    {
        os  << token::BEGIN_LIST
            << static_cast<const triFace&>(t) << token::SPACE << t.index()
            << token::END_LIST;
    }
    else
    {
        os.write
        (
            reinterpret_cast<const char*>(&t),
            sizeof(labelledTri)
        );
    }

    os.check(FUNCTION_NAME);
    return os;
}


// ************************************************************************* //

 } // End namespace Foam
