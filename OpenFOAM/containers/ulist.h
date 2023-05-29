﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2017-2021 OpenCFD Ltd.
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
    Foam::UList

Description
    A 1D vector of objects of type \<T\>, where the size of the vector is
    known and can be used for subscript bounds checking, etc.

    Storage is not allocated during construction or use but is supplied to
    the constructor as an argument.  This type of list is particularly useful
    for lists that refer to parts of existing lists such as SubList.

SourceFiles
    UList.C
    UListI.H
    UListIO.C

\*---------------------------------------------------------------------------*/

#ifndef UList_H
#define UList_H

#include "bool.H"
#include "label.H"
#include "uLabel.H"
#include "zero.H"
#include "one.H"
#include "contiguous.H"
#include "nullObject.H"
#include "stdFoam.H"
#include "Hash.H"
#include "Swap.H"
//#include "ListPolicy.H"

#include <initializer_list>
#include <iterator>
#include <type_traits>
#include <vector>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward Declarations
class Istream;
class Ostream;
class labelRange;

template<class T> class List;
template<class T> class SubList;
template<class T> class UList;
template<class T, class Addr> class IndirectListBase;

template<class T> Istream& operator>>(Istream&, UList<T>&);
template<class T> Ostream& operator<<(Ostream&, const UList<T>&);

// Common list types
typedef UList<bool> boolUList;      //!< A UList of bools
typedef UList<char> charUList;      //!< A UList of chars
typedef UList<label> labelUList;    //!< A UList of labels


/*---------------------------------------------------------------------------*\
                           Class UList Declaration
\*---------------------------------------------------------------------------*/

template<class T>
class UList
{
    // Private Data

        //- Number of elements in UList
        label size_;

        //- Vector of values of type T
        T* __restrict__ v_;


protected:

    // Protected Member Functions

        //- Set addressed size to be inconsistent with allocated storage.
        //  Use with care
        inline void setAddressableSize(const label n) noexcept;

        //- Older name for setAddressableSize
        FOAM_DEPRECATED_FOR(2021-01, "setAddressableSize(label) method")
        void size(const label n) { this->setAddressableSize(n); }

        //- Write the UList with its compound type
        void writeEntry(Ostream& os) const;

        //- Return a validated (start,size) subset range, which means that it
        //- always addresses a valid section of the list.
        labelRange validateRange(const labelRange& requestedRange) const;

        //- No copy assignment (default: shallow copy)
        //
        //  Assignment may need to be shallow (copy pointer)
        //  or deep (copy elements) depending on context or type of list.
        //  Disallow default assignment and provide separate 'shallowCopy' and
        //  'deepCopy' member functions.
        UList<T>& operator=(const UList<T>&) = delete;

public:

    // STL type definitions

        //- The value type the list contains
        typedef T value_type;

        //- The pointer type for non-const access to value_type items
        typedef T* pointer;

        //- The pointer type for const access to value_type items
        typedef const T* const_pointer;

        //- The type used for storing into value_type objects
        typedef T& reference;

        //- The type used for reading from constant value_type objects.
        typedef const T& const_reference;

        //- Random access iterator for traversing a UList
        typedef T* iterator;

        //- Random access iterator for traversing a UList
        typedef const T* const_iterator;

        //- The type to represent the size of a UList
        typedef label size_type;

        //- The difference between iterator objects
        typedef label difference_type;

        //- Reverse iterator (non-const access)
        typedef std::reverse_iterator<iterator>  reverse_iterator;

        //- Reverse iterator (const access)
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;


    // Related types

        //- Declare friendship with the List class
        friend class List<T>;

        //- Declare friendship with the SubList class
        friend class SubList<T>;


    // Static Functions

        //- Return a UList reference to a nullObject
        inline static const UList<T>& null();


    // Public Classes

        //- A list compare binary predicate for normal sort
        struct less
        {
            const UList<T>& values;

            less(const UList<T>& list)
            :
                values(list)
            {}

            bool operator()(const label a, const label b) const
            {
                return values[a] < values[b];
            }
        };

        //- A list compare binary predicate for reverse sort
        struct greater
        {
            const UList<T>& values;

            greater(const UList<T>& list)
            :
                values(list)
            {}

            bool operator()(const label a, const label b) const
            {
                return values[b] < values[a];
            }
        };


    // Generated Methods

        //- Copy construct
        UList(const UList<T>&) = default;


    // Constructors

        //- Default construct, zero-sized and nullptr
        inline constexpr UList() noexcept;

        //- Construct from components
        inline UList(T* __restrict__ v, const label len) noexcept;


    // Member Functions

        // Access

            //- The forward circular index. The next index in the list
            //- which returns to the first at the end of the list
            inline label fcIndex(const label i) const noexcept;

            //- The reverse circular index. The previous index in the list
            //- which returns to the last at the beginning of the list
            inline label rcIndex(const label i) const noexcept;

            //- Return forward circular value (ie, next value in the list)
            inline const T& fcValue(const label i) const;

            //- Return forward circular value (ie, next value in the list)
            inline T& fcValue(const label i);

            //- Return reverse circular value (ie, previous value in the list)
            inline const T& rcValue(const label i) const;

            //- Return reverse circular value (ie, previous value in the list)
            inline T& rcValue(const label i);

            //- Return pointer to the underlying array serving as data storage.
            inline const T* cdata() const noexcept;

            //- Return pointer to the underlying array serving as data storage.
            inline T* data() noexcept;

            //- Return pointer to the underlying array serving as data storage,
            //  reinterpreted as byte data
            //  \note Only meaningful for contiguous data
            inline const char* cdata_bytes() const noexcept;

            //- Return pointer to the underlying array serving as data storage,
            //  reinterpreted as byte data
            //  \note Only meaningful for contiguous data
            inline char* data_bytes() noexcept;

            //- Return the first element of the list
            inline T& first();

            //- Return first element of the list
            inline const T& first() const;

            //- Return the last element of the list
            inline T& last();

            //- Return the last element of the list
            inline const T& last() const;

            //- Number of contiguous bytes for the List data.
            //  \note Only meaningful for contiguous data
            inline std::streamsize size_bytes() const noexcept;

            //- Number of contiguous bytes for the List data,
            //- runtime FatalError if type is not contiguous
            std::streamsize byteSize() const;


    // Check

        //- Check start is within valid range [0,size)
        inline void checkStart(const label start) const;

        //- Check size is within valid range [0,size]
        inline void checkSize(const label size) const;

        //- Check that start and length define a valid range
        inline void checkRange(const label start, const label len) const;

        //- Check index is within valid range [0,size)
        inline void checkIndex(const label i) const;

        //- True if all entries have identical values, and list is non-empty
        inline bool uniform() const;


    // Search

        //- Find index of the first occurrence of the value.
        //  Any occurrences before the start pos are ignored.
        //  Linear search.
        //  \return position in list or -1 if not found.
        label find(const T& val, label pos = 0) const;

        //- Find index of the last occurrence of the value.
        //  Any occurrences after the end pos are ignored.
        //  Linear search.
        //  \return position in list or -1 if not found.
        label rfind(const T& val, label pos = -1) const;

        //- True if the value if found in the list.
        //  Any occurrences before the start pos are ignored.
        //  Linear search.
        //  \return true if found.
        inline bool found(const T& val, label pos = 0) const;


    // Edit

        //- Move element to the first position.
        void moveFirst(const label i);

        //- Move element to the last position.
        void moveLast(const label i);

        //- Swap element with the first element. Fatal on an empty list.
        void swapFirst(const label i);

        //- Swap element with the last element. Fatal on an empty list.
        void swapLast(const label i);


    // Copy

        //- Copy the pointer and size held by the given UList
        inline void shallowCopy(const UList<T>& list);

        //- Copy elements of the given UList. Sizes must match!
        void deepCopy(const UList<T>& list);

        //- Copy elements of the given indirect list. Sizes must match!
        template<class Addr>
        void deepCopy(const IndirectListBase<T, Addr>& list);


    // Other Access

        //- Return SubList slice (non-const access) - no range checking
        SubList<T> slice(const label pos, label len = -1);

        //- Return SubList slice (const access) - no range checking
        const SubList<T> slice(const label pos, label len = -1) const;

        //- Return SubList slice (non-const access) - with range checking.
        //  The range is subsetted with the list size itself to ensure that the
        //  result always addresses a valid section of the list.
        SubList<T> slice(const labelRange& range);

        //- Return SubList slice (const access) - with range checking.
        //  The range is subsetted with the list size itself to ensure that the
        //  result always addresses a valid section of the list.
        const SubList<T> slice(const labelRange& range) const;


    // Member Operators

        //- Return element of UList
        inline T& operator[](const label i);

        //- Return element of constant UList
        //  \note bool specialization adds lazy evaluation so reading an
        //       out-of-range element returns false without ill-effects
        inline const T& operator[](const label i) const;

        //- Allow cast to a const List<T>&
        inline operator const List<T>&() const;

        //- Assignment of all entries to the given value
        void operator=(const T& val);

        //- Assignment of all entries to zero
        void operator=(const zero);


    // Random access iterator (non-const)

        //- Return an iterator to begin traversing the UList
        inline iterator begin() noexcept;

        //- Return an iterator to end traversing the UList
        inline iterator end() noexcept;


    // Random access iterator (const)

        //- Return const_iterator to begin traversing the constant UList
        inline const_iterator cbegin() const noexcept;

        //- Return const_iterator to end traversing the constant UList
        inline const_iterator cend() const noexcept;

        //- Return const_iterator to begin traversing the constant UList
        inline const_iterator begin() const noexcept;

        //- Return const_iterator to end traversing the constant UList
        inline const_iterator end() const noexcept;


    // Reverse iterators (non-const)

        //- Return reverse_iterator to begin reverse traversing the UList
        inline reverse_iterator rbegin();

        //- Return reverse_iterator to end reverse traversing the UList
        inline reverse_iterator rend();


    // Reverse iterators (const)

        //- Return const_reverse_iterator to begin reverse traversing the UList
        inline const_reverse_iterator crbegin() const;

        //- Return const_reverse_iterator to end reverse traversing the UList
        inline const_reverse_iterator crend() const;

        //- Return const_reverse_iterator to begin reverse traversing the UList
        inline const_reverse_iterator rbegin() const;

        //- Return const_reverse_iterator to end reverse traversing the UList
        inline const_reverse_iterator rend() const;


    // STL member functions

        //- The number of elements in the UList
        inline label size() const noexcept;

        //- True if the UList is empty (ie, size() is zero)
        inline bool empty() const noexcept;

        //- The size of the largest possible UList
        static constexpr label max_size() noexcept { return labelMax; }

        //- Swap content with another UList of the same type in constant time
        inline void swap(UList<T>& list);


    // STL member operators

        //- Equality operation on ULists of the same type.
        //  Returns true when the ULists are element-wise equal
        //  (using UList::value_type::operator==).  Takes linear time
        bool operator==(const UList<T>& a) const;

        //- The opposite of the equality operation. Takes linear time
        bool operator!=(const UList<T>& a) const;

        //- Compare two ULists lexicographically. Takes linear time
        bool operator<(const UList<T>& list) const;

        //- Compare two ULists lexicographically. Takes linear time
        bool operator>(const UList<T>& a) const;

        //- Return true if !(a > b). Takes linear time
        bool operator<=(const UList<T>& a) const;

        //- Return true if !(a < b). Takes linear time
        bool operator>=(const UList<T>& a) const;


    // Reading/writing

        //- Read List contents from Istream.
        //  The List must have the proper size before calling
        Istream& readList(Istream& is);

        //- Write the List as a dictionary entry with keyword
        void writeEntry(const word& keyword, Ostream& os) const;

        //- Write List, with line-breaks in ASCII when length exceeds shortLen.
        //  Using '0' suppresses line-breaks entirely.
        Ostream& writeList(Ostream& os, const label shortLen=0) const;


    // IOstream Operators

        //- Use the readList() method to read contents from Istream.
        friend Istream& operator>> <T>
        (
            Istream& os,
            UList<T>& list
        );


    // Special Methods

        //- A bitSet::test() method for a list of bool
        //
        //  \return The element value, or false for out-of-range access
        template<class TypeT = T>
        typename std::enable_if<std::is_same<bool, TypeT>::value, bool>::type
        inline test(const label i) const
        {
            return (i >= 0 && i < size_ && v_[i]);
        }

        //- A bitSet::get() method for a list of bool
        //
        //  \return The element value, or false for out-of-range access
        template<class TypeT = T>
        typename std::enable_if<std::is_same<bool, TypeT>::value, bool>::type
        inline get(const label i) const
        {
            return (i >= 0 && i < size_ && v_[i]);
        }

        //- A bitSet::unset() method for a list of bool
        //
        //  \return True if value changed and was not out-of-range
        template<class TypeT = T>
        typename std::enable_if<std::is_same<bool, TypeT>::value, bool>::type
        inline unset(const label i)
        {
            if (i >= 0 && i < size_ && v_[i])
            {
                v_[i] = false;
                return true;
            }
            return false;
        }


    // Hashing

        //- Hashing functor for UList
        struct hasher
        {
            inline unsigned operator()
            (
                const UList<T>& obj,
                unsigned seed=0
            ) const
            {
                if (is_contiguous<T>::value)
                {
                    return Hasher(obj.cdata(), obj.size_bytes(), seed);
                }

                ::Foam::Hash<T> op;
                for (const T& val : obj)
                {
                    seed = op(val, seed);
                }
                return seed;
            }
        };

        //- Deprecated(2021-04) hashing functor. Use hasher()
        // \deprecated(2021-04) - use hasher() functor
        template<class Unused=bool>
        struct Hash : UList<T>::hasher
        {
            FOAM_DEPRECATED_FOR(2021-04, "hasher()") Hash() {}
        };
};


// * * * * * * * * * * * * Template Specializations  * * * * * * * * * * * * //

//- Specialized list reading for character lists which always uses
//- binary format.
template<>
Istream& UList<char>::readList(Istream& is);

//- Specialized writeEntry for character lists which always uses
//- binary format.
template<>
void UList<char>::writeEntry(Ostream& os) const;

//- Specialized writeList for character lists which always uses
//- binary format.
template<>
Ostream& UList<char>::writeList(Ostream& os, const label /*unused*/) const;


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

//- Read List contents from Istream, list must have the proper size!
template<class T>
Istream& operator>>(Istream& is, UList<T>& list)
{
    return list.readList(is);
}
namespace Detail {
    template<class T>
    struct short_length : std::integral_constant<label, 10> {};
}
//- Write List to Ostream, as per UList::writeList() with default length.
//  The default short-length is given by Detail::ListPolicy::short_length
template<class T>
Ostream& operator<<(Ostream& os, const UList<T>& list)
{
    return list.writeList(os, Detail::/*ListPolicy::*/short_length<T>::value);
}

//- Write std::vector to Ostream. ASCII only, no line-breaks
template<class T>
Ostream& operator<<(Ostream& os, const std::vector<T>& list);


// * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * * //

template<class T>
void sort(UList<T>& a);

template<class T, class Compare>
void sort(UList<T>& a, const Compare& comp);

template<class T>
void stableSort(UList<T>& a);

template<class T, class Compare>
void stableSort(UList<T>& a, const Compare& comp);

template<class T>
void shuffle(UList<T>& a);

// Reverse the first n elements of the list
template<class T>
inline void reverse(UList<T>& list, const label n);

// Reverse all the elements of the list
template<class T>
inline void reverse(UList<T>& list);

//- Exchange contents of lists - see UList::swap().
template<class T>
inline void Swap(UList<T>& a, UList<T>& b)
{
    a.swap(b);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//- Hashing for List data
template<class T>
struct Hash<UList<T>> : UList<T>::hasher {};


//- Object access operator or list access operator.
//- \sa ListListOps::combine()
template<class T>
struct accessOp
{
    const T& operator()(const T& obj) const
    {
        return obj;  // Default is pass-through
    }
};


//- Test if object is empty, typically using its empty() method.
template<class T>
struct emptyOp
{
    bool operator()(const T& obj) const
    {
        return obj.empty();
    }
};


//- Extract size (as label) from an object, typically using its size() method.
template<class T>
struct sizeOp
{
    label operator()(const T& obj) const
    {
        return obj.size();
    }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "UListI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "UList.C"
    #include "UListIO.C"
    #include "stdVectorIO.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
