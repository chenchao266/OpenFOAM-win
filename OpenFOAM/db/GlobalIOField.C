﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2015 OpenFOAM Foundation
    Copyright (C) 2016-2018 OpenCFD Ltd.
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

#include "GlobalIOField.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


 namespace Foam{
template<class Type>
GlobalIOField<Type>::GlobalIOField(const IOobject& io)
:
    regIOobject(io)
{
    // Check for MUST_READ_IF_MODIFIED
    warnNoRereading<GlobalIOField<Type>>();

    readHeaderOk(IOstream::BINARY, typeName);
}


template<class Type>
GlobalIOField<Type>::GlobalIOField(const IOobject& io, const label size)
:
    regIOobject(io)
{
    // Check for MUST_READ_IF_MODIFIED
    warnNoRereading<GlobalIOField<Type>>();

    if (!readHeaderOk(IOstream::BINARY, typeName))
    {
        Field<Type>::setSize(size);
    }
}


template<class Type>
GlobalIOField<Type>::GlobalIOField
(
    const IOobject& io,
    const UList<Type>& content
)
:
    regIOobject(io)
{
    // Check for MUST_READ_IF_MODIFIED
    warnNoRereading<GlobalIOField<Type>>();

    if (!readHeaderOk(IOstream::BINARY, typeName))
    {
        Field<Type>::operator=(content);
    }
}


template<class Type>
GlobalIOField<Type>::GlobalIOField
(
    const IOobject& io,
    Field<Type>&& content
)
:
    regIOobject(io)
{
    // Check for MUST_READ_IF_MODIFIED
    warnNoRereading<GlobalIOField<Type>>();

    Field<Type>::transfer(content);

    readHeaderOk(IOstream::BINARY, typeName);
}


template<class Type>
GlobalIOField<Type>::GlobalIOField
(
    const IOobject& io,
    const tmp<Field<Type>>& tfld
)
:
    regIOobject(io)
{
    // Check for MUST_READ_IF_MODIFIED
    warnNoRereading<GlobalIOField<Type>>();

    const bool reuse = tfld.movable();

    if (reuse)
    {
        Field<Type>::transfer(tfld.ref());
    }

    if (!readHeaderOk(IOstream::BINARY, typeName))
    {
        Field<Type>::operator=(tfld());
    }

    tfld.clear();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
bool GlobalIOField<Type>::readData(Istream& is)
{
    is >> *this;
    return is.good();
}


template<class Type>
bool GlobalIOField<Type>::writeData(Ostream& os) const
{
    return (os << static_cast<const Field<Type>&>(*this)).good();
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Type>
void GlobalIOField<Type>::operator=(const GlobalIOField<Type>& rhs)
{
    Field<Type>::operator=(rhs);
}


// ************************************************************************* //

 } // End namespace Foam
