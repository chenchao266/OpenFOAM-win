/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
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

Class
    Foam::DynamicField

Description
    Dynamically sized Field.

SourceFiles
    DynamicFieldI.H

\*---------------------------------------------------------------------------*/

#ifndef DynamicField_H
#define DynamicField_H

#include "Field.H"
#include "DynamicList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward Declarations
template<class Type, int SizeMin> class DynamicField;

template<class Type, int SizeMin>
inline Istream& operator>>(Istream&, DynamicField<Type, SizeMin>&);

template<class Type, int SizeMin>
inline Ostream& operator<<(Ostream&, const DynamicField<Type, SizeMin>&);

/*---------------------------------------------------------------------------*\
                        Class DynamicField Declaration
\*---------------------------------------------------------------------------*/

template<class Type, int SizeMin=64>
class DynamicField
:
    public Field<Type>
{
    static_assert(SizeMin > 0, "Invalid min size parameter");

    // Private Data

        //- The capacity (allocated size) of the underlying field.
        label capacity_;


    // Private Member Functions

        //- Copy assignment from another list
        template<class ListType>
        inline void assignDynList(const ListType& list);

        //- Alter the size of the underlying storage
        //  The 'nocopy' option will not attempt to recover old content
        inline void doCapacity(const bool nocopy, const label len);

        //- Reserve allocation space for at least this size.
        //  Never shrinks the allocated size, use setCapacity() for that.
        //  The 'nocopy' option will not attempt to recover old content
        inline void doReserve(const bool nocopy, const label len);

        //- Reserve allocation space for at least this size.
        //  Never shrinks the allocated size, use setCapacity() for that.
        //  The 'nocopy' option will not attempt to recover old content
        inline void doResize(const bool nocopy, const label len);


public:

    // Static Member Functions

        //- Return a null field
        inline static const DynamicField<Type, SizeMin>& null()
        {
            return NullObjectRef<DynamicField<Type, SizeMin>>();
        }


    // Constructors

        //- Default construct, an empty field without allocation.
        inline constexpr DynamicField() noexcept;

        //- Construct empty field with given reserve size.
        inline explicit DynamicField(const label len);

        //- Construct given size and initial value
        inline DynamicField(const label len, const Type& val);

        //- Construct given size and initial value of zero
        inline DynamicField(const label len, const zero);

        //- Copy construct
        inline DynamicField(const DynamicField<Type, SizeMin>& list);

        //- Copy construct with different sizing parameters
        template<int AnySizeMin>
        inline DynamicField(const DynamicField<Type, AnySizeMin>& list);

        //- Copy construct from UList. Size set to UList size.
        //  Also constructs from DynamicField with different sizing parameters.
        inline explicit DynamicField(const UList<Type>& list);

        //- Copy construct from IndirectList
        template<class Addr>
        inline explicit DynamicField(const IndirectListBase<Type, Addr>& list);

        //- Move construct from List contents
        inline explicit DynamicField(List<Type>&& content);

        //- Move construct from dynamic Field contents
        inline DynamicField(DynamicField<Type, SizeMin>&& content);

        //- Move construct with different sizing parameters
        template<int AnySizeMin>
        inline DynamicField(DynamicField<Type, AnySizeMin>&& content);

        //- Move construct from DynamicList
        template<int AnySizeMin>
        inline DynamicField(DynamicList<Type, AnySizeMin>&& list);

        //- Construct by 1 to 1 mapping from the given field
        inline DynamicField
        (
            const UList<Type>& mapF,
            const labelUList& mapAddressing
        );

        //- Construct by interpolative mapping from the given field
        inline DynamicField
        (
            const UList<Type>& mapF,
            const labelListList& mapAddressing,
            const scalarListList& weights
        );

        //- Construct by mapping from the given field
        inline DynamicField
        (
            const UList<Type>& mapF,
            const FieldMapper& map
        );

        //- Construct from Istream. Size set to size of list read.
        inline explicit DynamicField(Istream& is);

        //- Clone
        inline tmp<DynamicField<Type, SizeMin>> clone() const;


    // Member Functions

    // Access

        //- Normal lower capacity limit - the SizeMin template parameter
        static constexpr label min_size() noexcept { return SizeMin; }

        //- Size of the underlying storage.
        inline label capacity() const noexcept;

        //- Number of contiguous bytes of the underlying storage.
        //  \note Only meaningful for contiguous data
        inline std::streamsize capacity_bytes() const noexcept;


    // Sizing

        //- Alter the size of the underlying storage.
        //  The addressed size will be truncated if needed to fit, but will
        //  remain otherwise untouched.
        //  Use this or reserve() in combination with append().
        inline void setCapacity(const label len);

        //- Alter the size of the underlying storage,
        //- \em without retaining old content.
        //  The addressed size will be truncated if needed to fit, but will
        //  remain otherwise untouched.
        inline void setCapacity_nocopy(const label len);

        //- Change the value for the list capacity directly (ADVANCED, UNSAFE)
        //- Does not perform any memory management or resizing.
        inline void setCapacity_unsafe(const label len) noexcept;

        //- Reserve allocation space for at least this size, allocating new
        //- space if required and \em retaining old content.
        //  Never shrinks the allocated size, use setCapacity() for that.
        inline void reserve(const label len);

        //- Reserve allocation space for at least this size, allocating new
        //- space if required \em without retaining old content.
        //  Never shrinks the allocated size, use setCapacity() for that.
        inline void reserve_nocopy(const label len);

        //- Alter addressable list size, allocating new space if required
        //- while \em recovering old content.
        //  If no reallocation is required, the contents remain untouched.
        //  Otherwise new entries will be uninitialized.
        //  Use this to resize the list prior to using the operator[] for
        //  setting values (as per List usage).
        inline void resize(const label len);

        //- Alter addressable size and fill new space with constant value
        inline void resize(const label len, const Type& val);

        //- Alter addressable list size, allocating new space if required
        //- \em without necessarily recovering old content.
        //  If no reallocation is required, the contents remain untouched.
        //  Otherwise all entries will be uninitialized.
        inline void resize_nocopy(const label len);

        //- Alias for resize()
        void setSize(const label n) { this->resize(n); }

        //- Alias for resize()
        void setSize(const label n, const Type& val) { this->resize(n, val); }

        //- Clear the addressed list, i.e. set the size to zero.
        //  Allocated size does not change
        inline void clear() noexcept;

        //- Clear the list and delete storage.
        inline void clearStorage();

        //- Expand the addressable size to fit the allocated capacity.
        //  Returns the previous addressable size.
        inline label expandStorage() noexcept;

        //- Shrink the allocated space to the number of elements used.
        inline void shrinkStorage();

        //- Shrink the allocated space to the number of elements used.
        //  Returns a reference to the DynamicField.
        inline DynamicField<Type, SizeMin>& shrink();


    // Edit

        //- Swap content, independent of sizing parameter
        template<int AnySizeMin>
        inline void swap(DynamicField<Type, AnySizeMin>& other);

        //- Swap content with DynamicList, independent of sizing parameter
        template<int AnySizeMin>
        inline void swap(DynamicList<Type, AnySizeMin>& other);

        //- Transfer the parameter contents into this
        inline void transfer(List<Type>& list);

        //- Transfer the parameter contents into this
        template<int AnySizeMin>
        inline void transfer(DynamicList<Type, AnySizeMin>& list);

        //- Transfer the parameter contents into this
        template<int AnySizeMin>
        inline void transfer(DynamicField<Type, AnySizeMin>& list);

        //- Append an element at the end of the list
        inline DynamicField<Type, SizeMin>& append(const Type& val);

        //- Move append an element
        inline DynamicField<Type, SizeMin>& append(Type&& val);

        //- Append a List at the end of this list
        inline DynamicField<Type, SizeMin>&
        append(const UList<Type>& list);

        //- Remove and return the top element
        inline Type remove();


    // Reading/writing

        //- Read from Istream, discarding existing contents
        //  Uses a DynamicList::readList internally
        inline Istream& readList(Istream& is);


    // Member Operators

        //- Return non-const access to an element, resizing list if needed
        inline Type& operator()(const label i);

        //- Assign addressed entries to the given value
        inline void operator=(const Type& val);

        //- Assign addressed entries to zero
        inline void operator=(const zero);

        //- Copy assignment
        inline void operator=(const UList<Type>& list);

        //- Copy assignment
        inline void operator=(const DynamicField<Type, SizeMin>& list);

        //- Move assignment
        inline void operator=(List<Type>&& list);

        //- Move assignment
        template<int AnySizeMin>
        inline void operator=(DynamicList<Type, AnySizeMin>&& list);

        //- Move assignment
        inline void operator=(DynamicField<Type, SizeMin>&& list);

        //- Move assignment
        template<int AnySizeMin>
        inline void operator=(DynamicField<Type, AnySizeMin>&& list);


    // IOstream Operators

        //- Use the readList() method to read contents from Istream.
        friend Istream& operator>> <Type, SizeMin>
        (
            Istream& is,
            DynamicField<Type, SizeMin>& rhs
        );

        //- Write to Ostream
        friend Ostream& operator<< <Type, SizeMin>
        (
            Ostream& os,
            const DynamicField<Type, SizeMin>& rhs
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "DynamicFieldI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
