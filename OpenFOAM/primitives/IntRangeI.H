﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
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

\*---------------------------------------------------------------------------*/

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


 namespace Foam{
template<class IntType>
inline constexpr IntRange<IntType>::IntRange() noexcept
:
    start_(0),
    size_(0)
{}


template<class IntType>
inline constexpr IntRange<IntType>::IntRange
(
    const IntType len
) noexcept
:
    start_(0),
    size_(len)
{}


template<class IntType>
inline constexpr IntRange<IntType>::IntRange
(
    const IntType beg,
    const IntType len
) noexcept
:
    start_(beg),
    size_(len)
{}


// * * * * * * * * * * * * * * Forward Iterators * * * * * * * * * * * * * * //

template<class IntType>
inline constexpr IntRange<IntType>::const_iterator::
const_iterator(const IntType val) noexcept
:
    value_(val)
{}


template<class IntType>
inline constexpr IntType
IntRange<IntType>::const_iterator::
operator*() const noexcept
{
    return value_;
}


template<class IntType>
inline constexpr IntType
IntRange<IntType>::const_iterator::
operator[](const IntType n) const noexcept
{
    return (value_ + n);
}


template<class IntType>
inline typename IntRange<IntType>::const_iterator&
IntRange<IntType>::const_iterator::
operator++() noexcept
{
    ++value_;
    return *this;
}


template<class IntType>
inline typename IntRange<IntType>::const_iterator
IntRange<IntType>::const_iterator::
operator++(int) noexcept
{
    const_iterator old(*this);
    ++value_;
    return old;
}


template<class IntType>
inline typename IntRange<IntType>::const_iterator&
IntRange<IntType>::const_iterator::
operator--() noexcept
{
    --value_;
    return *this;
}


template<class IntType>
inline typename IntRange<IntType>::const_iterator
IntRange<IntType>::const_iterator::
operator--(int) noexcept
{
    const_iterator old(*this);
    --value_;
    return old;
}


template<class IntType>
inline typename IntRange<IntType>::const_iterator&
IntRange<IntType>::const_iterator::
operator+=(const IntType n) noexcept
{
    value_ += n;
    return *this;
}


template<class IntType>
inline typename IntRange<IntType>::const_iterator&
IntRange<IntType>::const_iterator::
operator-=(const IntType n) noexcept
{
    value_ -= n;
    return *this;
}


template<class IntType>
inline constexpr typename IntRange<IntType>::const_iterator
IntRange<IntType>::const_iterator::
operator+(const IntType n) const noexcept
{
    return const_iterator(value_ + n);
}


template<class IntType>
inline constexpr typename IntRange<IntType>::const_iterator
IntRange<IntType>::const_iterator::
operator-(const IntType n) const noexcept
{
    return const_iterator(value_ - n);
}


template<class IntType>
inline constexpr IntType
IntRange<IntType>::const_iterator::
operator-(const const_iterator& iter) const noexcept
{
    return (value_ - iter.value_);
}


template<class IntType>
inline constexpr bool
IntRange<IntType>::const_iterator::
operator==(const const_iterator& iter) const noexcept
{
    return (value_ == iter.value_);
}


template<class IntType>
inline constexpr bool
IntRange<IntType>::const_iterator::
operator<(const const_iterator& iter) const noexcept
{
    return (value_ < iter.value_);
}


// * * * * * * * * * * * * * * Reverse Iterators * * * * * * * * * * * * * * //

template<class IntType>
inline constexpr IntRange<IntType>::const_reverse_iterator::
const_reverse_iterator(const IntType val) noexcept
:
    value_(val)
{}


template<class IntType>
inline constexpr IntType
IntRange<IntType>::const_reverse_iterator::
operator*() const noexcept
{
    return value_;
}


template<class IntType>
inline constexpr IntType
IntRange<IntType>::const_reverse_iterator::
operator[](const IntType n) const noexcept
{
    return (value_ - n);
}


template<class IntType>
inline typename IntRange<IntType>::const_reverse_iterator&
IntRange<IntType>::const_reverse_iterator::
operator++() noexcept
{
    --value_;
    return *this;
}


template<class IntType>
inline typename IntRange<IntType>::const_reverse_iterator
IntRange<IntType>::const_reverse_iterator::
operator++(int) noexcept
{
    const_reverse_iterator old(*this);
    --value_;
    return old;
}


template<class IntType>
inline typename IntRange<IntType>::const_reverse_iterator&
IntRange<IntType>::const_reverse_iterator::
operator--() noexcept
{
    ++value_;
    return *this;
}


template<class IntType>
inline typename IntRange<IntType>::const_reverse_iterator
IntRange<IntType>::const_reverse_iterator::
operator--(int) noexcept
{
    const_reverse_iterator old(*this);
    ++value_;
    return old;
}


template<class IntType>
inline typename IntRange<IntType>::const_reverse_iterator&
IntRange<IntType>::const_reverse_iterator::
operator+=(const IntType n) noexcept
{
    value_ -= n;
    return *this;
}


template<class IntType>
inline typename IntRange<IntType>::const_reverse_iterator&
IntRange<IntType>::const_reverse_iterator::
operator-=(const IntType n) noexcept
{
    value_ += n;
    return *this;
}


template<class IntType>
inline constexpr typename IntRange<IntType>::const_reverse_iterator
IntRange<IntType>::const_reverse_iterator::
operator+(const IntType n) const noexcept
{
    return const_reverse_iterator(value_ - n);
}


template<class IntType>
inline constexpr typename IntRange<IntType>::const_reverse_iterator
IntRange<IntType>::const_reverse_iterator::
operator-(const IntType n) const noexcept
{
    return const_reverse_iterator(value_ + n);
}


template<class IntType>
inline constexpr IntType
IntRange<IntType>::const_reverse_iterator::
operator-(const const_reverse_iterator& iter) const noexcept
{
    return (iter.value_ - value_);
}


template<class IntType>
inline constexpr bool
IntRange<IntType>::const_reverse_iterator::
operator==(const const_reverse_iterator& iter) const noexcept
{
    return (value_ == iter.value_);
}


template<class IntType>
inline constexpr bool
IntRange<IntType>::const_reverse_iterator::
operator<(const const_reverse_iterator& iter) const noexcept
{
    return (value_ > iter.value_);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class IntType>
inline typename IntRange<IntType>::const_iterator
IntRange<IntType>::at(const IntType i) const
{
    return const_iterator(start_ + ((i < 0 || i > size_) ? size_ : i));
}


template<class IntType>
inline typename IntRange<IntType>::const_iterator
IntRange<IntType>::begin() const noexcept
{
    return const_iterator(start_);
}


template<class IntType>
inline typename IntRange<IntType>::const_iterator
IntRange<IntType>::cbegin() const noexcept
{
    return const_iterator(start_);
}


template<class IntType>
inline typename IntRange<IntType>::const_iterator
IntRange<IntType>::end() const noexcept
{
    return const_iterator(start_ + size_);
}


template<class IntType>
inline typename IntRange<IntType>::const_iterator
IntRange<IntType>::cend() const noexcept
{
    return const_iterator(start_ + size_);
}


template<class IntType>
inline typename IntRange<IntType>::const_reverse_iterator
IntRange<IntType>::rbegin() const noexcept
{
    return const_reverse_iterator(start_ + (size_-1));
}


template<class IntType>
inline typename IntRange<IntType>::const_reverse_iterator
IntRange<IntType>::crbegin() const noexcept
{
    return const_reverse_iterator(start_ + (size_-1));
}


template<class IntType>
inline typename IntRange<IntType>::const_reverse_iterator
IntRange<IntType>::rend() const noexcept
{
    return const_reverse_iterator(start_ - 1);
}


template<class IntType>
inline typename IntRange<IntType>::const_reverse_iterator
IntRange<IntType>::crend() const noexcept
{
    return const_reverse_iterator(start_ - 1);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class IntType>
inline IntType IntRange<IntType>::start() const noexcept
{
    return start_;
}


template<class IntType>
inline IntType IntRange<IntType>::size() const noexcept
{
    return size_;
}


template<class IntType>
inline IntType& IntRange<IntType>::start() noexcept
{
    return start_;
}


template<class IntType>
inline IntType& IntRange<IntType>::size() noexcept
{
    return size_;
}


template<class IntType>
inline bool IntRange<IntType>::empty() const noexcept
{
    return !size_;
}


template<class IntType>
inline IntType IntRange<IntType>::first() const noexcept
{
    return start_;
}


template<class IntType>
inline IntType IntRange<IntType>::last() const noexcept
{
    return (start_ + (size_-1));
}


template<class IntType>
inline IntType IntRange<IntType>::rend_value() const noexcept
{
    return (start_ - 1);
}


template<class IntType>
inline IntType IntRange<IntType>::end_value() const noexcept
{
    return (start_ + size_);
}


template<class IntType>
inline void IntRange<IntType>::clear() noexcept
{
    start_ = size_ = 0;
}


template<class IntType>
inline void IntRange<IntType>::reset
(
    const IntType beg,
    const IntType len
) noexcept
{
    start_ = beg;
    size_  = len;
}


template<class IntType>
inline void IntRange<IntType>::setStart(const IntType i) noexcept
{
    start_ = i;
}


template<class IntType>
inline void IntRange<IntType>::setSize(const IntType n) noexcept
{
    size_ = n;
}


template<class IntType>
inline void IntRange<IntType>::resize(const IntType n) noexcept
{
    size_ = n;
}


template<class IntType>
inline void IntRange<IntType>::clampSize() noexcept
{
    if (size_ < 0) size_ = 0;
}


template<class IntType>
inline bool IntRange<IntType>::found(const IntType value) const noexcept
{
    // Avoid overflow risk:
    //   (value < (start_ + size_)) --> ((value - start_) < size_)

    return (size_ && start_ <= value && (value - start_) < size_);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class IntType>
inline constexpr IntType IntRange<IntType>::
operator[](const IntType i) const noexcept
{
    return start_ + i;
}


template<class IntType>
inline bool IntRange<IntType>::
operator()(const IntType value) const noexcept
{
    return found(value);
}


template<class IntType>
inline IntType IntRange<IntType>::
operator++() noexcept
{
    return ++size_;
}


template<class IntType>
inline IntType IntRange<IntType>::
operator++(int) noexcept
{
    return size_++;
}


template<class IntType>
inline IntType IntRange<IntType>::
operator--() noexcept
{
    --size_;
    clampSize();
    return size_;
}


template<class IntType>
inline IntType IntRange<IntType>::
operator--(int) noexcept
{
    const IntType old(size_);
    --size_;
    clampSize();
    return old;
}


template<class IntType>
inline IntType IntRange<IntType>::
operator+=(const IntType n) noexcept
{
    size_ += n;
    return size_;
}


template<class IntType>
inline IntType IntRange<IntType>::
operator-=(const IntType n) noexcept
{
    size_ -= n;
    clampSize();
    return size_;
}


// ************************************************************************* //

 } // End namespace Foam
