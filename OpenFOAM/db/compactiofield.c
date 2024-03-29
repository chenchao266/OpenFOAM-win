﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
    Copyright (C) 2018-2020 OpenCFD Ltd.
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

//#include "CompactIOField.H"
#include "labelList.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


 namespace Foam{
template<class Type, class BaseType>
void CompactIOField<Type, BaseType>::readFromStream(const bool valid)
{
    Istream& is = readStream(word::null, valid);

    if (valid)
    {
        if (headerClassName() == IOField<Type>::typeName)
        {
            is >> static_cast<Field<Type>&>(*this);
            close();
        }
        else if (headerClassName() == typeName)
        {
            is >> *this;
            close();
        }
        else
        {
            FatalIOErrorInFunction(is)
                << "unexpected class name " << headerClassName()
                << " expected " << typeName << " or " << IOField<Type>::typeName
                << endl
                << "    while reading object " << name()
                << exit(FatalIOError);
        }
    }
}


// * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * //

template<class Type, class BaseType>
CompactIOField<Type, BaseType>::CompactIOField(const IOobject& io)
:
    regIOobject(io)
{
    if
    (
        io.readOpt() == IOobject::MUST_READ
     || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())
    )
    {
        readFromStream();
    }
}


template<class Type, class BaseType>
CompactIOField<Type, BaseType>::CompactIOField
(
    const IOobject& io,
    const bool valid
)
:
    regIOobject(io)
{
    if (io.readOpt() == IOobject::MUST_READ)
    {
        readFromStream(valid);
    }
    else if (io.readOpt() == IOobject::READ_IF_PRESENT)
    {
        bool haveFile = headerOk();
        readFromStream(valid && haveFile);
    }
}


template<class Type, class BaseType>
CompactIOField<Type, BaseType>::CompactIOField
(
    const IOobject& io,
    const label size
)
:
    regIOobject(io)
{
    if
    (
        io.readOpt() == IOobject::MUST_READ
     || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())
    )
    {
        readFromStream();
    }
    else
    {
        Field<Type>::setSize(size);
    }
}


template<class Type, class BaseType>
CompactIOField<Type, BaseType>::CompactIOField
(
    const IOobject& io,
    const UList<Type>& content
)
:
    regIOobject(io)
{
    if
    (
        io.readOpt() == IOobject::MUST_READ
     || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())
    )
    {
        readFromStream();
    }
    else
    {
        Field<Type>::operator=(content);
    }
}


template<class Type, class BaseType>
CompactIOField<Type, BaseType>::CompactIOField
(
    const IOobject& io,
    Field<Type>&& content
)
:
    regIOobject(io)
{
    Field<Type>::transfer(content);

    if
    (
        io.readOpt() == IOobject::MUST_READ
     || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())
    )
    {
        readFromStream();
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type, class BaseType>
bool CompactIOField<Type, BaseType>::writeObject
(
    IOstreamOption streamOpt,
    const bool valid
) const
{
    if (streamOpt.format() == IOstream::ASCII)
    {
        // Change type to be non-compact format type
        const word oldTypeName(typeName);

        const_cast<word&>(typeName) = IOField<Type>::typeName;

        bool good = regIOobject::writeObject(streamOpt, valid);

        // Restore type
        const_cast<word&>(typeName) = oldTypeName;

        return good;
    }

    return regIOobject::writeObject(streamOpt, valid);
}


template<class Type, class BaseType>
bool CompactIOField<Type, BaseType>::writeData(Ostream& os) const
{
    return (os << *this).good();
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Type, class BaseType>
void CompactIOField<Type, BaseType>::operator=
(
    const CompactIOField<Type, BaseType>& rhs
)
{
    if (this == &rhs)
    {
        return;  // Self-assigment is a no-op
    }

    Field<Type>::operator=(rhs);
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class Type, class BaseType>
Istream& operator>>
(
    Istream& is,
    CompactIOField<Type, BaseType>& L
)
{
    // Read compact
    const labelList start(is);
    const Field<BaseType> elems(is);

    // Convert
    L.setSize(start.size()-1);

    forAll(L, i)
    {
        Type& subField = L[i];

        label index = start[i];
        subField.setSize(start[i+1] - index);

        forAll(subField, j)
        {
            subField[j] = elems[index++];
        }
    }

    return is;
}


template<class Type, class BaseType>
Ostream& operator<<
(
    Ostream& os,
    const CompactIOField<Type, BaseType>& L
)
{
    // Keep ascii writing same.
    if (os.format() == IOstream::ASCII)
    {
        os << static_cast<const Field<Type>&>(L);
    }
    else
    {
        // Convert to compact format
        labelList start(L.size()+1);

        start[0] = 0;
        for (label i = 1; i < start.size(); i++)
        {
            start[i] = start[i-1]+L[i-1].size();
        }

        Field<BaseType> elems(start[start.size()-1]);

        label elemI = 0;
        forAll(L, i)
        {
            const Type& subField = L[i];

            forAll(subField, j)
            {
                elems[elemI++] = subField[j];
            }
        }
        os << start << elems;
    }

    return os;
}


// ************************************************************************* //

 } // End namespace Foam
