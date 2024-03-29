﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
    Copyright (C) 2020 OpenCFD Ltd.
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

Class
    Foam::CompactIOField

Description
    A Field of objects of type \<Type\> with automated input and output using
    a compact storage. Behaves like IOField except when binary output in
    case it writes a CompactListList.

    Useful for fields of small subfields e.g. in lagrangian

SourceFiles
    CompactIOField.C

\*---------------------------------------------------------------------------*/

#ifndef CompactIOField_H
#define CompactIOField_H

#include "IOField.H"
#include "regIOobject.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declarations
template<class Type, class BaseType> class CompactIOField;

template<class Type, class BaseType> Istream& operator>>
(
    Istream&,
    CompactIOField<Type, BaseType>&
);

template<class Type, class BaseType> Ostream& operator<<
(
    Ostream&,
    const CompactIOField<Type, BaseType>&
);

/*---------------------------------------------------------------------------*\
                        Class CompactIOField Declaration
\*---------------------------------------------------------------------------*/

template<class Type, class BaseType>
class CompactIOField
:
    public regIOobject,
    public Field<Type>
{
    // Private Member Functions

        //- Read according to header type
        void readFromStream(const bool valid = true);

public:

    //- Runtime type information
    TypeName("FieldField");


    // Constructors

        //- Default copy construct
        CompactIOField(const CompactIOField&) = default;

        //- Construct from IOobject
        explicit CompactIOField(const IOobject& io);

        //- Construct from IOobject; does local processor require reading?
        CompactIOField(const IOobject& io, const bool valid);

        //- Construct from IOobject and size
        CompactIOField(const IOobject& io, const label size);

        //- Construct from IOobject and a List/Field content
        CompactIOField(const IOobject& io, const UList<Type>& content);

        //- Construct by transferring the Field contents
        CompactIOField(const IOobject& io, Field<Type>&& content);


    //- Destructor
    virtual ~CompactIOField() = default;


    // Member Functions

        //- Write using stream options
        virtual bool writeObject
        (
            IOstreamOption streamOpt,
            const bool valid
        ) const;

        virtual bool writeData(Ostream& os) const;


    // Member Operators

        //- Copy assignment of entries
        void operator=(const CompactIOField<Type, BaseType>& rhs);

        //- Copy or move assignment of entries
        using Field<Type>::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "CompactIOField.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
