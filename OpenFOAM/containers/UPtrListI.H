/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2018-2021 OpenCFD Ltd.
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

// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //


 namespace Foam{
template<class T>
inline void UPtrList<T>::setAddressableSize(const label n) noexcept
{
    ptrs_.setAddressableSize(n);
}


// * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * //

template<class T>
inline constexpr UPtrList<T>::UPtrList() noexcept
:
    ptrs_()
{}


template<class T>
inline UPtrList<T>::UPtrList(const label len)
:
    ptrs_(len)
{}


template<class T>
inline UPtrList<T>::UPtrList(Detail::PtrListDetail<T>&& ptrs)
:
    ptrs_(std::move(ptrs))
{}


template<class T>
inline UPtrList<T>::UPtrList(const UPtrList<T>& list)
:
    ptrs_(list.ptrs_)
{}


template<class T>
inline UPtrList<T>::UPtrList(UPtrList<T>&& list)
:
    ptrs_(std::move(list.ptrs_))
{}


template<class T>
inline UPtrList<T>::UPtrList(UPtrList<T>& list, bool reuse)
:
    ptrs_(list.ptrs_, reuse)
{}


template<class T>
inline UPtrList<T>::UPtrList(const UList<T*>& list)
:
    ptrs_(list)
{}


template<class T>
inline UPtrList<T>::UPtrList(UList<T>& list)
:
    ptrs_(list.size())
{
    const label len = ptrs_.size();

    for (label i=0; i<len; ++i)
    {
        ptrs_[i] = &(list[i]);
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T>
inline label UPtrList<T>::size() const noexcept
{
    return ptrs_.size();
}


template<class T>
inline bool UPtrList<T>::empty() const noexcept
{
    return ptrs_.empty();
}


template<class T>
inline T* UPtrList<T>::get(const label i)
{
    return ptrs_[i];
}


template<class T>
inline const T* UPtrList<T>::get(const label i) const
{
    return ptrs_[i];
}


template<class T>
inline void UPtrList<T>::clear()
{
    ptrs_.clear();
}


template<class T>
inline void UPtrList<T>::swap(UPtrList<T>& list)
{
    ptrs_.swap(list.ptrs_);
}


template<class T>
inline void UPtrList<T>::transfer(UPtrList<T>& list)
{
    ptrs_.transfer(list.ptrs_);
}


template<class T>
inline T& UPtrList<T>::first()
{
    return this->operator[](0);
}


template<class T>
inline const T& UPtrList<T>::first() const
{
    return this->operator[](0);
}


template<class T>
inline T& UPtrList<T>::last()
{
    return this->operator[](this->size()-1);
}


template<class T>
inline const T& UPtrList<T>::last() const
{
    return this->operator[](this->size()-1);
}


template<class T>
inline void UPtrList<T>::resize(const label newLen)
{
    ptrs_.resize(newLen);
}


template<class T>
inline void UPtrList<T>::append(T* ptr)
{
    const label idx = this->size();
    ptrs_.resize(idx + 1);
    ptrs_[idx] = ptr;
}


template<class T>
inline T* UPtrList<T>::set(const label i, T* ptr)
{
    T* old = ptrs_[i];
    if (old == ptr)
    {
        return nullptr;  // Content did not change
    }
    ptrs_[i] = ptr;
    return old;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class T>
inline const T& UPtrList<T>::operator[](const label i) const
{
    const T* ptr = ptrs_[i];

    if (!ptr)
    {
        FatalErrorInFunction
            << "Cannot dereference nullptr at index " << i
            << " in range [0," << size() << ")\n"
            << abort(FatalError);
    }

    return *ptr;
}


template<class T>
inline T& UPtrList<T>::operator[](const label i)
{
    T* ptr = ptrs_[i];

    if (!ptr)
    {
        FatalErrorInFunction
            << "Cannot dereference nullptr at index " << i
            << " in range [0," << size() << ")\n"
            << abort(FatalError);
    }

    return *ptr;
}


template<class T>
inline const T* UPtrList<T>::operator()(const label i) const
{
    return ptrs_[i];
}


// * * * * * * * * * * * * * * * * iterator  * * * * * * * * * * * * * * * * //

template<class T>
inline UPtrList<T>::iterator::
iterator(T** ptr) noexcept
:
    ptr_(ptr)
{}


template<class T>
inline T* UPtrList<T>::iterator::get() const
{
    return *ptr_;
}


template<class T>
inline T* UPtrList<T>::iterator::operator->() const
{
    return *ptr_;
}


template<class T>
inline T& UPtrList<T>::iterator::operator*() const
{
    return **ptr_;
}


template<class T>
inline T& UPtrList<T>::iterator::operator[](label n) const
{
    return **(ptr_ + n);
}

template<class T>
inline typename UPtrList<T>::iterator&
UPtrList<T>::iterator::operator++() noexcept
{
    ++ptr_;
    return *this;
}


template<class T>
inline typename UPtrList<T>::iterator
UPtrList<T>::iterator::operator++(int) noexcept
{
    iterator iter(*this);
    ++ptr_;
    return iter;
}


template<class T>
inline typename UPtrList<T>::iterator&
UPtrList<T>::iterator::operator--() noexcept
{
    --ptr_;
    return *this;
}


template<class T>
inline typename UPtrList<T>::iterator
UPtrList<T>::iterator::operator--(int) noexcept
{
    iterator iter(*this);
    --ptr_;
    return iter;
}


template<class T>
inline typename UPtrList<T>::iterator&
UPtrList<T>::iterator::operator+=(label n) noexcept
{
    ptr_ += n;
    return *this;
}


template<class T>
inline typename UPtrList<T>::iterator&
UPtrList<T>::iterator::operator-=(label n) noexcept
{
    ptr_ -= n;
    return *this;
}


template<class T>
inline typename UPtrList<T>::iterator
UPtrList<T>::iterator::operator+(label n) const noexcept
{
    return iterator(ptr_ + n);
}


template<class T>
inline typename UPtrList<T>::iterator
UPtrList<T>::iterator::operator-(label n) const noexcept
{
    return iterator(ptr_ - n);
}


template<class T>
inline label
UPtrList<T>::iterator::
operator-(const iterator& iter) const noexcept
{
    return (ptr_ - iter.ptr_);
}


template<class T>
inline bool UPtrList<T>::iterator::
operator==(const iterator& iter) const noexcept
{
    return ptr_ == iter.ptr_;
}


template<class T>
inline bool UPtrList<T>::iterator::
operator!=(const iterator& iter) const noexcept
{
    return ptr_ != iter.ptr_;
}


template<class T>
inline bool UPtrList<T>::iterator::
operator<(const iterator& iter) const noexcept
{
    return ptr_ < iter.ptr_;
}


template<class T>
inline bool UPtrList<T>::iterator::
operator>(const iterator& iter) const noexcept
{
    return ptr_ > iter.ptr_;
}


template<class T>
inline bool UPtrList<T>::iterator::
operator<=(const iterator& iter) const noexcept
{
    return ptr_ <= iter.ptr_;
}


template<class T>
inline bool UPtrList<T>::iterator::
operator>=(const iterator& iter) const noexcept
{
    return ptr_ >= iter.ptr_;
}


// * * * * * * * * * * * * * * * const_iterator  * * * * * * * * * * * * * * //

template<class T>
inline UPtrList<T>::const_iterator::
const_iterator(const T* const* ptr) noexcept
:
    ptr_(ptr)
{}


template<class T>
inline UPtrList<T>::const_iterator::
const_iterator(const iterator& iter) noexcept
:
    ptr_(iter.ptr_)
{}


template<class T>
inline const T* UPtrList<T>::const_iterator::get() const
{
    return *ptr_;
}


template<class T>
inline const T* UPtrList<T>::const_iterator::operator->() const
{
    return *ptr_;
}


template<class T>
inline const T& UPtrList<T>::const_iterator::operator*() const
{
    return **ptr_;
}


template<class T>
inline const T&
UPtrList<T>::const_iterator::operator[](label n) const
{
    return **(ptr_ + n);
}


template<class T>
inline typename UPtrList<T>::const_iterator&
UPtrList<T>::const_iterator::operator++() noexcept
{
    ++ptr_;
    return *this;
}


template<class T>
inline typename UPtrList<T>::const_iterator
UPtrList<T>::const_iterator::operator++(int) noexcept
{
    const_iterator iter(*this);
    ++ptr_;
    return iter;
}


template<class T>
inline typename UPtrList<T>::const_iterator&
UPtrList<T>::const_iterator::operator--() noexcept
{
    --ptr_;
    return *this;
}


template<class T>
inline typename UPtrList<T>::const_iterator
UPtrList<T>::const_iterator::operator--(int) noexcept
{
    const_iterator iter(*this);
    --ptr_;
    return iter;
}


template<class T>
inline typename UPtrList<T>::const_iterator&
UPtrList<T>::const_iterator::operator+=(label n) noexcept
{
    ptr_ += n;
    return *this;
}


template<class T>
inline typename UPtrList<T>::const_iterator&
UPtrList<T>::const_iterator::operator-=(label n) noexcept
{
    ptr_ -= n;
    return *this;
}


template<class T>
inline typename UPtrList<T>::const_iterator
UPtrList<T>::const_iterator::operator+(label n) const noexcept
{
    return const_iterator(ptr_ + n);
}


template<class T>
inline typename UPtrList<T>::const_iterator
UPtrList<T>::const_iterator::operator-(label n) const noexcept
{
    return const_iterator(ptr_ - n);
}


template<class T>
inline label
UPtrList<T>::const_iterator::
operator-(const const_iterator& iter) const noexcept
{
    return (ptr_ - iter.ptr_);
}


template<class T>
inline bool UPtrList<T>::const_iterator::
operator==(const const_iterator& iter) const noexcept
{
    return ptr_ == iter.ptr_;
}


template<class T>
inline bool UPtrList<T>::const_iterator::
operator!=(const const_iterator& iter) const noexcept
{
    return ptr_ != iter.ptr_;
}


template<class T>
inline bool UPtrList<T>::const_iterator::
operator<(const const_iterator& iter) const noexcept
{
    return ptr_ < iter.ptr_;
}


template<class T>
inline bool UPtrList<T>::const_iterator::
operator>(const const_iterator& iter) const noexcept
{
    return ptr_ > iter.ptr_;
}


template<class T>
inline bool UPtrList<T>::const_iterator::
operator<=(const const_iterator& iter) const noexcept
{
    return ptr_ <= iter.ptr_;
}


template<class T>
inline bool UPtrList<T>::const_iterator::
operator>=(const const_iterator& iter) const noexcept
{
    return ptr_ >= iter.ptr_;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T>
inline typename UPtrList<T>::iterator
UPtrList<T>::begin() noexcept
{
    return ptrs_.begin();
}


template<class T>
inline typename UPtrList<T>::iterator
UPtrList<T>::end() noexcept
{
    return ptrs_.end();
}


template<class T>
inline typename UPtrList<T>::const_iterator
UPtrList<T>::cbegin() const noexcept
{
    return ptrs_.cbegin();
}


template<class T>
inline typename UPtrList<T>::const_iterator
UPtrList<T>::cend() const noexcept
{
    return ptrs_.cend();
}


template<class T>
inline typename UPtrList<T>::const_iterator
UPtrList<T>::begin() const noexcept
{
    return ptrs_.begin();
}


template<class T>
inline typename UPtrList<T>::const_iterator
UPtrList<T>::end() const noexcept
{
    return ptrs_.end();
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class T>
inline void UPtrList<T>::operator=(const UPtrList<T>& list)
{
    ptrs_ = list.ptrs_;  // shallow copy
}


template<class T>
inline void UPtrList<T>::operator=(UPtrList<T>&& list)
{
    ptrs_.transfer(list.ptrs_);
}


// ************************************************************************* //

 } // End namespace Foam
