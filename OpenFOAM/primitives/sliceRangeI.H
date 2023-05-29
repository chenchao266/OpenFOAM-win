/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2019-2021 OpenCFD Ltd.
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

#include <algorithm>

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


 namespace Foam{
inline constexpr sliceRange::sliceRange() noexcept
:
    start_(0),
    size_(0),
    stride_(0)
{}


inline constexpr sliceRange::sliceRange
(
    const label beg,
    const label len,
    const label stride
) noexcept
:
    start_(beg),
    size_(len),
    stride_(stride)
{}


// * * * * * * * * * * * * * * * * Iterators * * * * * * * * * * * * * * * * //

inline constexpr sliceRange::indexer::indexer() noexcept
:
    stride_(1),
    value_(0)
{}


inline constexpr sliceRange::indexer::indexer
(
    const label val,
    const label stride
) noexcept
:
    stride_(stride),
    value_(val)
{}


inline label
sliceRange::indexer::
operator()() noexcept
{
    const label old(value_);
    next();
    return old;
}


// * * * * * * * * * * * * * * Forward Iterators * * * * * * * * * * * * * * //

inline constexpr label
sliceRange::const_iterator::
operator[](const label n) const noexcept
{
    return value(n);
}


inline sliceRange::const_iterator&
sliceRange::const_iterator::
operator++() noexcept
{
    next();
    return *this;
}


inline sliceRange::const_iterator
sliceRange::const_iterator::
operator++(int) noexcept
{
    const_iterator old(*this);
    next();
    return old;
}


inline sliceRange::const_iterator&
sliceRange::const_iterator::
operator--() noexcept
{
    prev();
    return *this;
}


inline sliceRange::const_iterator
sliceRange::const_iterator::
operator--(int) noexcept
{
    const_iterator old(*this);
    prev();
    return old;
}


inline sliceRange::const_iterator&
sliceRange::const_iterator::
operator+=(const label n) noexcept
{
    next(n);
    return *this;
}


inline sliceRange::const_iterator&
sliceRange::const_iterator::
operator-=(const label n) noexcept
{
    prev(n);
    return *this;
}


inline constexpr sliceRange::const_iterator
sliceRange::const_iterator::
operator+(const label n) const noexcept
{
    return const_iterator(value(n), stride());
}


inline constexpr sliceRange::const_iterator
sliceRange::const_iterator::
operator-(const label n) const noexcept
{
    return const_iterator(value(-n), stride());
}


inline constexpr label
sliceRange::const_iterator::
operator-(const const_iterator& iter) const noexcept
{
    return (stride() ? (value() - iter.value()) / stride() : label{0});
}


inline constexpr bool
sliceRange::const_iterator::
operator==(const const_iterator& iter) const noexcept
{
    return (value() == iter.value());
}


inline constexpr bool
sliceRange::const_iterator::
operator<(const const_iterator& iter) const noexcept
{
    return (value() < iter.value());
}


// * * * * * * * * * * * * * * Reverse Iterators * * * * * * * * * * * * * * //

inline constexpr label
sliceRange::const_reverse_iterator::
operator[](const label n) const noexcept
{
    return value(-n);
}


inline sliceRange::const_reverse_iterator&
sliceRange::const_reverse_iterator::
operator++() noexcept
{
    prev();
    return *this;
}


inline sliceRange::const_reverse_iterator
sliceRange::const_reverse_iterator::
operator++(int) noexcept
{
    const_reverse_iterator old(*this);
    prev();
    return old;
}


inline sliceRange::const_reverse_iterator&
sliceRange::const_reverse_iterator::
operator--() noexcept
{
    next();
    return *this;
}


inline sliceRange::const_reverse_iterator
sliceRange::const_reverse_iterator::
operator--(int) noexcept
{
    const_reverse_iterator old(*this);
    next();
    return old;
}


inline sliceRange::const_reverse_iterator&
sliceRange::const_reverse_iterator::
operator+=(const label n) noexcept
{
    prev(n);
    return *this;
}


inline sliceRange::const_reverse_iterator&
sliceRange::const_reverse_iterator::
operator-=(const label n) noexcept
{
    next(n);
    return *this;
}


inline constexpr sliceRange::const_reverse_iterator
sliceRange::const_reverse_iterator::
operator+(const label n) const noexcept
{
    return const_reverse_iterator(value(-n), stride());
}


inline constexpr sliceRange::const_reverse_iterator
sliceRange::const_reverse_iterator::
operator-(const label n) const noexcept
{
    return const_reverse_iterator(value(n), stride());
}


inline constexpr label
sliceRange::const_reverse_iterator::
operator-(const const_reverse_iterator& iter) const noexcept
{
    return (stride() ? (iter.value() - value()) / stride() : label{0});
}


inline constexpr bool
sliceRange::const_reverse_iterator::
operator==(const const_reverse_iterator& iter) const noexcept
{
    return (value() == iter.value());
}


inline constexpr bool
sliceRange::const_reverse_iterator::
operator<(const const_reverse_iterator& iter) const noexcept
{
    return (iter.value() < value());
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline sliceRange::indexer
sliceRange::generator() const
{
    return indexer(start_, stride_);
}


inline sliceRange::const_iterator
sliceRange::at(const label i) const
{
    return
        const_iterator
        (
            start_ + ((i < 0 || i > size_) ? size_ : i) * stride_,
            stride_
        );
}


inline sliceRange::const_iterator
sliceRange::begin() const noexcept
{
    return const_iterator(start_, stride_);
}


inline sliceRange::const_iterator
sliceRange::cbegin() const noexcept
{
    return const_iterator(start_, stride_);
}


inline sliceRange::const_iterator
sliceRange::end() const noexcept
{
    return const_iterator(start_ + size_*stride_, stride_);
}


inline sliceRange::const_iterator
sliceRange::cend() const noexcept
{
    return const_iterator(start_ + size_*stride_, stride_);
}


inline sliceRange::const_reverse_iterator
sliceRange::rbegin() const noexcept
{
    return const_reverse_iterator(start_ + (size_-1)*stride_, stride_);
}


inline sliceRange::const_reverse_iterator
sliceRange::crbegin() const noexcept
{
    return const_reverse_iterator(start_ + (size_-1)*stride_, stride_);
}


inline sliceRange::const_reverse_iterator
sliceRange::rend() const noexcept
{
    return const_reverse_iterator(start_ - stride_, stride_);
}


inline sliceRange::const_reverse_iterator
sliceRange::crend() const noexcept
{
    return const_reverse_iterator(start_ - stride_, stride_);
}


// ************************************************************************* //

 } // End namespace Foam
