﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
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

//#include "IOField.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


 namespace Foam{
template<class Type>
IOField<Type>::IOField(const IOobject& io)
:
    regIOobject(io)
{
    // Check for MUST_READ_IF_MODIFIED
    warnNoRereading<IOField<Type>>();

    if
    (
        (
            io.readOpt() == IOobject::MUST_READ
         || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED
        )
     || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())
    )
    {
        readStream(typeName) >> *this;
        close();
    }
}


template<class Type>
IOField<Type>::IOField(const IOobject& io, const bool valid)
:
    regIOobject(io)
{
    // Check for MUST_READ_IF_MODIFIED
    warnNoRereading<IOField<Type>>();

    if
    (
        io.readOpt() == IOobject::MUST_READ
     || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED
    )
    {
        Istream& is = readStream(typeName, valid);

        if (valid)
        {
            is >> *this;
        }
        close();
    }
    else if (io.readOpt() == IOobject::READ_IF_PRESENT)
    {
        bool haveFile = headerOk();

        Istream& is = readStream(typeName, haveFile && valid);

        if (valid && haveFile)
        {
            is >> *this;
        }
        close();
    }
}


template<class Type>
IOField<Type>::IOField(const IOobject& io, const label size)
:
    regIOobject(io)
{
    // Check for MUST_READ_IF_MODIFIED
    warnNoRereading<IOField<Type>>();

    if
    (
        (
            io.readOpt() == IOobject::MUST_READ
         || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED
        )
     || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())
    )
    {
        readStream(typeName) >> *this;
        close();
    }
    else
    {
        Field<Type>::setSize(size);
    }
}


template<class Type>
IOField<Type>::IOField(const IOobject& io, const UList<Type>& content)
:
    regIOobject(io)
{
    // Check for MUST_READ_IF_MODIFIED
    warnNoRereading<IOField<Type>>();

    if
    (
        (
            io.readOpt() == IOobject::MUST_READ
         || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED
        )
     || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())
    )
    {
        readStream(typeName) >> *this;
        close();
    }
    else
    {
        Field<Type>::operator=(content);
    }
}


template<class Type>
IOField<Type>::IOField(const IOobject& io, Field<Type>&& content)
:
    regIOobject(io)
{
    // Check for MUST_READ_IF_MODIFIED
    warnNoRereading<IOField<Type>>();

    Field<Type>::transfer(content);

    if
    (
        (
            io.readOpt() == IOobject::MUST_READ
         || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED
        )
     || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())
    )
    {
        readStream(typeName) >> *this;
        close();
    }
}


template<class Type>
IOField<Type>::IOField(const IOobject& io, const tmp<Field<Type>>& tfld)
:
    regIOobject(io)
{
    const bool reuse = tfld.movable();

    if (reuse)
    {
        Field<Type>::transfer(tfld.ref());
    }

    if
    (
        (
            io.readOpt() == IOobject::MUST_READ
         || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED
        )
     || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())
    )
    {
        readStream(typeName) >> *this;
        close();
    }
    else if (!reuse)
    {
        Field<Type>::operator=(tfld());
    }

    tfld.clear();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
bool IOField<Type>::writeData(Ostream& os) const
{
    return (os << static_cast<const Field<Type>&>(*this)).good();
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Type>
void IOField<Type>::operator=(const IOField<Type>& rhs)
{
    Field<Type>::operator=(rhs);
}


// ************************************************************************* //

 } // End namespace Foam
