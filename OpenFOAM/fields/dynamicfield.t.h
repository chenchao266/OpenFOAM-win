﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2016 OpenFOAM Foundation
     \\/     M anipulation  |
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
    Foam::DynamicField

Description
    Dynamically sized Field.

SourceFiles
    DynamicFieldI.H
    DynamicField.C

\*---------------------------------------------------------------------------*/

#ifndef DynamicField_H
#define DynamicField_H

#include "Field.T.H"
#include <type_traits>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of friend functions and operators

template<class Ty, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
class DynamicField;

template<class Ty, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
Ostream& operator<<
(
    Ostream&,
    const DynamicField<Ty, SizeInc, SizeMult, SizeDiv>&
);

template<class Ty, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
Istream& operator>>
(
    Istream&,
    DynamicField<Ty, SizeInc, SizeMult, SizeDiv>&
);


/*---------------------------------------------------------------------------*\
                           Class DynamicField Declaration
\*---------------------------------------------------------------------------*/

template<class Ty, unsigned SizeInc=0, unsigned SizeMult=2, unsigned SizeDiv=1>
class DynamicField :    public Field<Ty>
{
    static_assert
    (
        (SizeInc || SizeMult) && SizeDiv,
        "Avoid invalid sizing parameters"
    );

    // Private data

        //- The capacity (allocated size) of the underlying field.
        label capacity_;


public:

    // Static Member Functions

        //- Return a null field
        inline static const DynamicField<Ty, SizeInc, SizeMult, SizeDiv>& null()
        {
            return *reinterpret_cast
            <
                DynamicField<Ty, SizeInc, SizeMult, SizeDiv>*
            >(0);
        }


    // Constructors

        //- Construct null
        inline DynamicField();

        //- Construct given size.
        explicit inline DynamicField(const label);

        //- Construct from UList. Size set to UList size.
        //  Also constructs from DynamicField with different sizing parameters.
        explicit inline DynamicField(const UList<Ty>&);

        //- Construct by transferring the parameter contents
        explicit inline DynamicField(const Xfer<List<Ty>>&);

        //- Construct by 1 to 1 mapping from the given field
        inline DynamicField
        (
            const UList<Ty>& mapF,
            const labelList& mapAddressing
        );

        //- Construct by interpolative mapping from the given field
        inline DynamicField
        (
            const UList<Ty>& mapF,
            const labelListList& mapAddressing,
            const scalarListList& weights
        );

        //- Construct by mapping from the given field
        inline DynamicField
        (
            const UList<Ty>& mapF,
            const FieldMapper& map
        );

        //- Construct copy
        inline DynamicField(const DynamicField<Ty, SizeInc, SizeMult, SizeDiv>&);

        //- Construct by transferring the Field contents
        inline DynamicField
        (
            const Xfer<DynamicField<Ty, SizeInc, SizeMult, SizeDiv>>&
        );

        //- Construct from Istream. Size set to size of list read.
        explicit DynamicField(Istream&);

        //- Clone
        tmp<DynamicField<Ty, SizeInc, SizeMult, SizeDiv>> clone() const;


    // Member Functions

        // Access

            //- Size of the underlying storage.
            inline label capacity() const;

        // Edit

            //- Alter the size of the underlying storage.
            //  The addressed size will be truncated if needed to fit, but will
            //  remain otherwise untouched.
            //  Use this or reserve() in combination with append().
            inline void setCapacity(const label);

            //- Alter the addressed list size.
            //  New space will be allocated if required.
            //  Use this to resize the list prior to using the operator[] for
            //  setting values (as per List usage).
            inline void setSize(const label);

            //- Alter the addressed list size and fill new space with a
            //  constant.
            inline void setSize(const label, const Ty&);

            //- Alter the addressed list size.
            //  New space will be allocated if required.
            //  Use this to resize the list prior to using the operator[] for
            //  setting values (as per List usage).
            inline void resize(const label);

            //- Alter the addressed list size and fill new space with a
            //  constant.
            inline void resize(const label, const Ty&);

            //- Reserve allocation space for at least this size.
            //  Never shrinks the allocated size, use setCapacity() for that.
            inline void reserve(const label);

            //- Clear the addressed list, i.e. set the size to zero.
            //  Allocated size does not change
            inline void clear();

            //- Clear the list and delete storage.
            inline void clearStorage();

            //- Shrink the allocated space to the number of elements used.
            //  Returns a reference to the DynamicField.
            inline DynamicField<Ty, SizeInc, SizeMult, SizeDiv>& shrink();

            //- Transfer contents to the Xfer container as a plain List
            inline Xfer<List<Ty>> xfer();


        // Member Operators

            //- Append an element at the end of the list
            inline DynamicField<Ty, SizeInc, SizeMult, SizeDiv>& append
            (
                const Ty&
            );

            //- Append a List at the end of this list
            inline DynamicField<Ty, SizeInc, SizeMult, SizeDiv>& append
            (
                const UList<Ty>&
            );

            //- Remove and return the top element
            inline Ty remove();

            //- Return non-const access to an element, resizing list if
            //  necessary
            inline Ty& operator()(const label);

            //- Assignment of all addressed entries to the given value
            inline void operator=(const Ty&);

            //- Assignment to DynamicField
            inline void operator=
            (
                const DynamicField<Ty, SizeInc, SizeMult, SizeDiv>&
            );

            //- Assignment to UList
            inline void operator=(const UList<Ty>&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "DynamicFieldI.T.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "DynamicField.T.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
