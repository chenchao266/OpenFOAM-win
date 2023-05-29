﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2015-2021 OpenCFD Ltd.
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

#include "error.H"
#include "pTraits.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


 namespace Foam{
template<class T>
inline constexpr UList<T>::UList() noexcept
:
    size_(0),
    v_(nullptr)
{}


template<class T>
inline UList<T>::UList(T* __restrict__ v, const label len) noexcept
:
    size_(len),
    v_(v)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T>
inline const UList<T>& UList<T>::null()
{
    return NullObjectRef<UList<T>>();
}


template<class T>
inline label UList<T>::fcIndex(const label i) const noexcept
{
    return (i == size()-1 ? 0 : i+1);
}


template<class T>
inline label UList<T>::rcIndex(const label i) const noexcept
{
    return (i ? i-1 : size()-1);
}


template<class T>
inline const T& UList<T>::fcValue(const label i) const
{
    return this->operator[](this->fcIndex(i));
}


template<class T>
inline T& UList<T>::fcValue(const label i)
{
    return this->operator[](this->fcIndex(i));
}


template<class T>
inline const T& UList<T>::rcValue(const label i) const
{
    return this->operator[](this->rcIndex(i));
}


template<class T>
inline T& UList<T>::rcValue(const label i)
{
    return this->operator[](this->rcIndex(i));
}


template<class T>
inline void UList<T>::checkStart(const label start) const
{
    if (start < 0 || (start && start >= size_))
    {
        // Note: accept start=0 for zero-sized lists
        FatalErrorInFunction
            << "start " << start << " out of range [0,"
            << size_ << "]\n"
            << abort(FatalError);
    }
}


template<class T>
inline void UList<T>::checkSize(const label size) const
{
    if (size < 0 || size > size_)
    {
        FatalErrorInFunction
            << "size " << size << " out of range [0,"
            << size_ << "]\n"
            << abort(FatalError);
    }
}


template<class T>
inline void UList<T>::checkRange
(
    const label start,
    const label len
) const
{
    // Artificially allow the start of a zero-sized subList to be
    // one past the end of the original list.
    if (len)
    {
        if (len < 0)
        {
            FatalErrorInFunction
                << "size " << len << " is negative, out of range [0,"
                << size_ << "]\n"
                << abort(FatalError);
        }
        this->checkStart(start);
        this->checkSize(start + len);
    }
    else
    {
        // Start index needs to fall between 0 and size.  One position
        // behind the last element is allowed
        this->checkSize(start);
    }
}


template<class T>
inline void UList<T>::checkIndex(const label i) const
{
    if (!size_)
    {
        FatalErrorInFunction
            << "attempt to access element " << i << " from zero sized list"
            << abort(FatalError);
    }
    else if (i < 0 || i >= size_)
    {
        FatalErrorInFunction
            << "index " << i << " out of range [0,"
            << size_ << "]\n"
            << abort(FatalError);
    }
}


template<class T>
inline bool UList<T>::uniform() const
{
    const label len = size();

    if (!len)
    {
        return false;
    }

    const T& val = (*this)[0];  // first

    for (label i = 1; i < len; ++i)
    {
        if (val != (*this)[i])
        {
            return false;
        }
    }

    return true;
}


template<class T>
inline T& UList<T>::first()
{
    return this->operator[](0);
}


template<class T>
inline const T& UList<T>::first() const
{
    return this->operator[](0);
}


template<class T>
inline T& UList<T>::last()
{
    return this->operator[](this->size()-1);
}


template<class T>
inline const T& UList<T>::last() const
{
    return this->operator[](this->size()-1);
}


template<class T>
inline const T* UList<T>::cdata() const noexcept
{
    return v_;
}


template<class T>
inline T* UList<T>::data() noexcept
{
    return v_;
}


template<class T>
inline const char* UList<T>::cdata_bytes() const noexcept
{
    return reinterpret_cast<const char*>(v_);
}


template<class T>
inline char* UList<T>::data_bytes() noexcept
{
    return reinterpret_cast<char*>(v_);
}


template<class T>
inline std::streamsize UList<T>::size_bytes() const noexcept
{
    return std::streamsize(size_)*sizeof(T);
}


template<class T>
inline bool UList<T>::found(const T& val, label pos) const
{
    return (this->find(val, pos) >= 0);
}


template<class T>
inline void UList<T>::shallowCopy(const UList<T>& list)
{
    size_ = list.size_;
    v_ = list.v_;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //



 
    // Template specialization for bool
    template<>
    inline const bool& UList<bool>::operator[](const label i) const
    {
        // Lazy evaluation - return false for out-of-range
        if (i >= 0 && i < size_)
        {
            return v_[i];
        }

        return pTraits<bool>::zero_;
    }
 


template<class T>
inline T& UList<T>::operator[](const label i)
{
    #ifdef FULLDEBUG
    checkIndex(i);
    #endif
    return v_[i];
}


template<class T>
inline const T& UList<T>::operator[](const label i) const
{
    #ifdef FULLDEBUG
    checkIndex(i);
    #endif
    return v_[i];
}


template<class T>
inline UList<T>::operator const List<T>&() const
{
    return *reinterpret_cast<const List<T>*>(this);
}


// * * * * * * * * * * * * * * STL Member Functions  * * * * * * * * * * * * //

template<class T>
inline typename UList<T>::iterator
UList<T>::begin() noexcept
{
    return v_;
}

template<class T>
inline typename UList<T>::const_iterator
UList<T>::begin() const noexcept
{
    return v_;
}

template<class T>
inline typename UList<T>::const_iterator
UList<T>::cbegin() const noexcept
{
    return v_;
}

template<class T>
inline typename UList<T>::iterator
UList<T>::end() noexcept
{
    return (v_ + size_);
}

template<class T>
inline typename UList<T>::const_iterator
UList<T>::end() const noexcept
{
    return (v_ + size_);
}

template<class T>
inline typename UList<T>::const_iterator
UList<T>::cend() const noexcept
{
    return (v_ + size_);
}

template<class T>
inline typename UList<T>::reverse_iterator
UList<T>::rbegin()
{
    return reverse_iterator(end());
}

template<class T>
inline typename UList<T>::const_reverse_iterator
UList<T>::rbegin() const
{
    return const_reverse_iterator(end());
}

template<class T>
inline typename UList<T>::const_reverse_iterator
UList<T>::crbegin() const
{
    return const_reverse_iterator(end());
}

template<class T>
inline typename UList<T>::reverse_iterator
UList<T>::rend()
{
    return reverse_iterator(begin());
}

template<class T>
inline typename UList<T>::const_reverse_iterator
UList<T>::rend() const
{
    return const_reverse_iterator(begin());
}

template<class T>
inline typename UList<T>::const_reverse_iterator
UList<T>::crend() const
{
    return const_reverse_iterator(begin());
}


template<class T>
inline void UList<T>::setAddressableSize(const label n) noexcept
{
    size_ = n;
}


template<class T>
inline label UList<T>::size() const noexcept
{
    return size_;
}


template<class T>
inline bool UList<T>::empty() const noexcept
{
    return !size_;
}


template<class T>
inline void UList<T>::swap(UList<T>& list)
{
    if (&list == this)
    {
        return;  // Self-swap is a no-op
    }

    std::swap(size_, list.size_);
    std::swap(v_, list.v_);
}


// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

template<class T>
inline void reverse(UList<T>& list, const label n)
{
    for (label i=0; i<n/2; ++i)
    {
        Swap(list[i], list[n-1-i]);
    }
}


template<class T>
inline void reverse(UList<T>& list)
{
    reverse(list, list.size());
}

 } // End namespace Foam
// ************************************************************************* //
