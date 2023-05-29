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

\*---------------------------------------------------------------------------*/

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //
 namespace Foam{
template<class Type, int SizeMin>
template<class ListType>
inline void DynamicField<Type, SizeMin>::assignDynList
(
    const ListType& list
)
{
    const label len = list.size();

    if (capacity_ < len)
    {
        // Needs more space for the copy operation
        List<Type>::setAddressableSize(capacity_);  // Use entire space
        List<Type>::resize_nocopy(len);
        capacity_ = List<Type>::size();
    }

    // Perform copy into addressable portion
    List<Type>::setAddressableSize(len);
    List<Type>::operator=(list);
}


template<class Type, int SizeMin>
inline void DynamicField<Type, SizeMin>::doCapacity
(
    const bool nocopy,
    const label newCapacity
)
{
    if (newCapacity == capacity_)
    {
        return;
    }

    // Addressable length, possibly truncated by new capacity
    const label currLen = min(List<Type>::size(), newCapacity);

    // Corner case - see comments in DynamicList doCapacity
    if (List<Type>::size() == newCapacity)
    {
        List<Type>::setAddressableSize(currLen+1);
    }

    if (nocopy)
    {
        List<Type>::resize_nocopy(newCapacity);
    }
    else
    {
        List<Type>::resize(newCapacity);
    }

    capacity_ = List<Type>::size();
    List<Type>::setAddressableSize(currLen);
}


template<class Type, int SizeMin>
inline void DynamicField<Type, SizeMin>::doReserve
(
    const bool nocopy,
    const label len
)
{
    if (capacity_ < len)
    {
        // Preserve addressed size
        const label currLen = List<Type>::size();

        // Increase capacity (doubling)
        capacity_ = max(SizeMin, max(len, label(2*capacity_)));

        if (nocopy)
        {
            List<Type>::resize_nocopy(capacity_);
        }
        else
        {
            List<Type>::resize(capacity_);
        }
        List<Type>::setAddressableSize(currLen);
    }
}


template<class Type, int SizeMin>
inline void DynamicField<Type, SizeMin>::doResize
(
    const bool nocopy,
    const label len
)
{
    this->doReserve(nocopy, len);
    List<Type>::setAddressableSize(len);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type, int SizeMin>
inline constexpr DynamicField<Type, SizeMin>::DynamicField() noexcept
:
    Field<Type>(),
    capacity_(0)
{}


template<class Type, int SizeMin>
inline DynamicField<Type, SizeMin>::DynamicField(const label len)
:
    Field<Type>(),
    capacity_(0)
{
    reserve_nocopy(len);
}


template<class Type, int SizeMin>
inline DynamicField<Type, SizeMin>::DynamicField
(
    const label len,
    const Type& val
)
:
    Field<Type>(len, val),
    capacity_(Field<Type>::size())
{}


template<class Type, int SizeMin>
inline DynamicField<Type, SizeMin>::DynamicField
(
    const label len,
    const zero
)
:
    Field<Type>(len, Zero),
    capacity_(Field<Type>::size())
{}


template<class Type, int SizeMin>
inline DynamicField<Type, SizeMin>::DynamicField
(
    const DynamicField<Type, SizeMin>& list
)
:
    Field<Type>(list),
    capacity_(Field<Type>::size())
{}


template<class Type, int SizeMin>
template<int AnySizeMin>
inline DynamicField<Type, SizeMin>::DynamicField
(
    const DynamicField<Type, AnySizeMin>& list
)
:
    Field<Type>(list),
    capacity_(Field<Type>::size())
{}


template<class Type, int SizeMin>
inline DynamicField<Type, SizeMin>::DynamicField
(
    const UList<Type>& list
)
:
    Field<Type>(list),
    capacity_(Field<Type>::size())
{}


template<class Type, int SizeMin>
template<class Addr>
inline DynamicField<Type, SizeMin>::DynamicField
(
    const IndirectListBase<Type, Addr>& list
)
:
    Field<Type>(list),
    capacity_(Field<Type>::size())
{}


template<class Type, int SizeMin>
inline DynamicField<Type, SizeMin>::DynamicField
(
    List<Type>&& content
)
:
    Field<Type>(std::move(content)),
    capacity_(Field<Type>::size())
{}


template<class Type, int SizeMin>
template<int AnySizeMin>
inline DynamicField<Type, SizeMin>::DynamicField
(
    DynamicList<Type, AnySizeMin>&& list
)
:
    Field<Type>(),
    capacity_(0)
{
    transfer(list);
}


template<class Type, int SizeMin>
inline DynamicField<Type, SizeMin>::DynamicField
(
    DynamicField<Type, SizeMin>&& content
)
:
    Field<Type>(),
    capacity_(0)
{
    transfer(content);
}


template<class Type, int SizeMin>
template<int AnySizeMin>
inline DynamicField<Type, SizeMin>::DynamicField
(
    DynamicField<Type, AnySizeMin>&& content
)
:
    Field<Type>(),
    capacity_(0)
{
    transfer(content);
}


template<class Type, int SizeMin>
inline DynamicField<Type, SizeMin>::DynamicField
(
    const UList<Type>& mapF,
    const labelUList& mapAddressing
)
:
    Field<Type>(mapF, mapAddressing),
    capacity_(Field<Type>::size())
{}


template<class Type, int SizeMin>
inline DynamicField<Type, SizeMin>::DynamicField
(
    const UList<Type>& mapF,
    const labelListList& mapAddressing,
    const scalarListList& weights
)
:
    Field<Type>(mapF, mapAddressing, weights),
    capacity_(Field<Type>::size())
{}


template<class Type, int SizeMin>
inline DynamicField<Type, SizeMin>::DynamicField
(
    const UList<Type>& mapF,
    const FieldMapper& map
)
:
    Field<Type>(mapF, map),
    capacity_(Field<Type>::size())
{}


template<class Type, int SizeMin>
inline DynamicField<Type, SizeMin>::DynamicField(Istream& is)
:
    Field<Type>(is),
    capacity_(Field<Type>::size())
{}


template<class Type, int SizeMin>
inline tmp<DynamicField<Type, SizeMin>>
DynamicField<Type, SizeMin>::clone() const
{
    return tmp<DynamicField<Type, SizeMin>>::New(*this);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type, int SizeMin>
inline label DynamicField<Type, SizeMin>::capacity() const noexcept
{
    return capacity_;
}


template<class Type, int SizeMin>
inline std::streamsize
DynamicField<Type, SizeMin>::capacity_bytes() const noexcept
{
    return std::streamsize(capacity_)*sizeof(Type);
}


template<class Type, int SizeMin>
inline void DynamicField<Type, SizeMin>::setCapacity
(
    const label len
)
{
    this->doCapacity(false, len);  // nocopy = false
}


template<class Type, int SizeMin>
inline void DynamicField<Type, SizeMin>::setCapacity_nocopy
(
    const label len
)
{
    this->doCapacity(true, len);  // nocopy = true
}


template<class Type, int SizeMin>
inline void DynamicField<Type, SizeMin>::setCapacity_unsafe
(
    const label len
) noexcept
{
    capacity_ = len;
}


template<class Type, int SizeMin>
inline void DynamicField<Type, SizeMin>::reserve
(
    const label len
)
{
    this->doReserve(false, len);  // nocopy = false
}


template<class Type, int SizeMin>
inline void DynamicField<Type, SizeMin>::reserve_nocopy
(
    const label len
)
{
    this->doReserve(true, len);  // nocopy = true
}


template<class Type, int SizeMin>
inline void DynamicField<Type, SizeMin>::resize
(
    const label len
)
{
    this->doResize(false, len);  // nocopy = false
}


template<class Type, int SizeMin>
inline void DynamicField<Type, SizeMin>::resize_nocopy
(
    const label len
)
{
    this->doResize(true, len);  // nocopy = true
}


template<class Type, int SizeMin>
inline void DynamicField<Type, SizeMin>::resize
(
    const label len,
    const Type& val
)
{
    label idx = List<Type>::size();
    resize(len);

    // Fill newly exposed with constant value
    while (idx < len)
    {
        this->operator[](idx) = val;
        ++idx;
    }
}


template<class Type, int SizeMin>
inline void DynamicField<Type, SizeMin>::clear() noexcept
{
    List<Type>::setAddressableSize(0);
}


template<class Type, int SizeMin>
inline void DynamicField<Type, SizeMin>::clearStorage()
{
    List<Type>::clear();
    capacity_ = 0;
}


template<class Type, int SizeMin>
inline label DynamicField<Type, SizeMin>::expandStorage() noexcept
{
    const label currLen = List<Type>::size();

    // Allow addressing into the entire list
    List<Type>::setAddressableSize(capacity_);

    return currLen;
}


template<class Type, int SizeMin>
inline void DynamicField<Type, SizeMin>::shrinkStorage()
{
    const label currLen = List<Type>::size();

    if (currLen < capacity_)
    {
        // Adjust addressable size to trigger proper resizing
        List<Type>::setAddressableSize(currLen+1);

        List<Type>::resize(currLen);
        capacity_ = List<Type>::size();
    }
}


template<class Type, int SizeMin>
inline DynamicField<Type, SizeMin>&
DynamicField<Type, SizeMin>::shrink()
{
    this->shrinkStorage();
    return *this;
}


template<class Type, int SizeMin>
template<int AnySizeMin>
inline void DynamicField<Type, SizeMin>::swap
(
    DynamicField<Type, AnySizeMin>& other
)
{
    // Cannot compare 'this' for different types, so use cdata()
    if (this->cdata() == other.cdata())
    {
        return;  // Self-swap is a no-op
    }

    // Swap storage and addressable size
    UList<Type>::swap(other);

    // Swap capacity
    std::swap(this->capacity_, other.capacity_);
}


template<class Type, int SizeMin>
template<int AnySizeMin>
inline void DynamicField<Type, SizeMin>::swap
(
    DynamicList<Type, AnySizeMin>& other
)
{
    auto& self = (*this);

    // ... not yet needed:
    // Cannot compare 'this' for different types, so use cdata()
    if (self.cdata() == other.cdata())
    {
        return;  // Self-swap is a no-op
    }

    // Swap storage and addressable size
    UList<Type>::swap(other);

    // Swap capacity
    const label oldCap = self.capacity();
    const label newCap = other.capacity();

    self.setCapacity_unsafe(newCap);
    other.setCapacity_unsafe(oldCap);
}


template<class Type, int SizeMin>
inline void DynamicField<Type, SizeMin>::transfer(List<Type>& list)
{
    // Take over storage, clear addressing for list
    capacity_ = list.size();
    Field<Type>::transfer(list);
}


template<class Type, int SizeMin>
template<int AnySizeMin>
inline void DynamicField<Type, SizeMin>::transfer
(
    DynamicList<Type, AnySizeMin>& list
)
{
    // Cannot compare 'this' for different types, so use cdata()
    if (this->cdata() == list.cdata())
    {
        return;  // Self-assignment is a no-op
    }

    // Take over storage as-is (without shrink, without using SizeMin)
    // clear addressing and storage for old list.
    capacity_ = list.capacity();

    Field<Type>::transfer(static_cast<List<Type>&>(list));
    list.clearStorage();  // Ensure capacity=0
}


template<class Type, int SizeMin>
template<int AnySizeMin>
inline void DynamicField<Type, SizeMin>::transfer
(
    DynamicField<Type, AnySizeMin>& list
)
{
    // Cannot compare 'this' for different types, so use cdata()
    if (this->cdata() == list.cdata())
    {
        return;  // Self-assignment is a no-op
    }

    // Take over storage as-is (without shrink, without using SizeMin)
    // clear addressing and storage for old list.
    capacity_ = list.capacity();

    Field<Type>::transfer(static_cast<List<Type>&>(list));
    list.clearStorage();  // Ensure capacity=0
}


template<class Type, int SizeMin>
inline DynamicField<Type, SizeMin>&
DynamicField<Type, SizeMin>::append
(
    const Type& val
)
{
    const label idx = List<Type>::size();
    resize(idx + 1);

    this->operator[](idx) = val;  // copy element
    return *this;
}


template<class Type, int SizeMin>
inline DynamicField<Type, SizeMin>&
DynamicField<Type, SizeMin>::append
(
    Type&& val
)
{
    const label idx = List<Type>::size();
    resize(idx + 1);

    this->operator[](idx) = std::move(val);  // move assign element
    return *this;
}


template<class Type, int SizeMin>
inline DynamicField<Type, SizeMin>&
DynamicField<Type, SizeMin>::append
(
    const UList<Type>& list
)
{
    if (this == &list)
    {
        FatalErrorInFunction
            << "Attempted appending to self"
            << abort(FatalError);
    }

    label idx = List<Type>::size();
    resize(idx + list.size());

    for (const Type& val : list)
    {
        this->operator[](idx++) = val;  // copy element
    }
    return *this;
}


template<class Type, int SizeMin>
inline Type DynamicField<Type, SizeMin>::remove()
{
    // Location of last element and simultaneously the new size
    const label idx = List<Type>::size() - 1;

    if (idx < 0)
    {
        FatalErrorInFunction
            << "List is empty" << abort(FatalError);
    }

    const Type& val = List<Type>::operator[](idx);

    List<Type>::setAddressableSize(idx);

    return val;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Type, int SizeMin>
inline Type& DynamicField<Type, SizeMin>::operator()
(
    const label i
)
{
    if (i >= List<Type>::size())
    {
        resize(i + 1);
    }

    return this->operator[](i);
}


template<class Type, int SizeMin>
inline void DynamicField<Type, SizeMin>::operator=
(
    const Type& val
)
{
    UList<Type>::operator=(val);
}


template<class Type, int SizeMin>
inline void DynamicField<Type, SizeMin>::operator=
(
    const zero
)
{
    UList<Type>::operator=(Zero);
}


template<class Type, int SizeMin>
inline void DynamicField<Type, SizeMin>::operator=
(
    const UList<Type>& list
)
{
    assignDynList(list);
}


template<class Type, int SizeMin>
inline void DynamicField<Type, SizeMin>::operator=
(
    const DynamicField<Type, SizeMin>& list
)
{
    if (this == &list)
    {
        return;  // Self-assignment is a no-op
    }

    assignDynList(list);
}


template<class Type, int SizeMin>
inline void DynamicField<Type, SizeMin>::operator=
(
    List<Type>&& list
)
{
    transfer(list);
}


template<class Type, int SizeMin>
inline void DynamicField<Type, SizeMin>::operator=
(
    DynamicField<Type, SizeMin>&& list
)
{
    transfer(list);
}


template<class Type, int SizeMin>
template<int AnySizeMin>
inline void DynamicField<Type, SizeMin>::operator=
(
    DynamicField<Type, AnySizeMin>&& list
)
{
    transfer(list);
}


template<class Type, int SizeMin>
template<int AnySizeMin>
inline void DynamicField<Type, SizeMin>::operator=
(
    DynamicList<Type, AnySizeMin>&& list
)
{
    transfer(list);
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Type, int SizeMin>
inline Istream& DynamicField<Type, SizeMin>::readList
(
    Istream& is
)
{
    // Use DynamicList::readList for reading DynamicField.
    // The logic should be the same and this avoids duplicate code

    DynamicList<Type, SizeMin> list;
    (*this).swap(list);

    list.readList(is);
    (*this).swap(list);

    return is;
}


template<class Type, int SizeMin>
inline Istream& operator>>
(
    Istream& is,
    DynamicField<Type, SizeMin>& rhs
)
{
    return rhs.readList(is);
}


template<class Type, int SizeMin>
inline Ostream& operator<<
(
    Ostream& os,
    const DynamicField<Type, SizeMin>& rhs
)
{
    os << static_cast<const Field<Type>&>(rhs);
    return os;
}


// ************************************************************************* //

 } // End namespace Foam
