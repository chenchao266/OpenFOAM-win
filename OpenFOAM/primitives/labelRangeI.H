/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2017 OpenFOAM Foundation
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

\*---------------------------------------------------------------------------*/


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //
namespace Foam {
inline labelRange::labelRange() :    start_(0),
    size_(0)
{}


inline labelRange::labelRange(const label start, const label size) :    start_(start),
    size_(size)
{
    // disallow invalid sizes
    if (size_ <= 0)
    {
        this->clear();
    }
}


// * * * * * * * * * * * * * * * * Iterators * * * * * * * * * * * * * * * * //

inline labelRange::const_iterator::const_iterator()
:
   range_(endLabelRange_),
   index_(-1)
{}


inline labelRange::const_iterator::const_iterator(const labelRange& range)
:
   range_(range),
   index_(range_.empty() ? -1 : 0)
{}


inline bool labelRange::const_iterator::operator==
(
    const const_iterator& iter
) const
{
    return (this->index_ == iter.index_);
}


inline bool labelRange::const_iterator::operator!=
(
    const const_iterator& iter
) const
{
    return !(this->operator==(iter));
}


inline label labelRange::const_iterator::operator*()
{
    return range_[index_];
}


inline label labelRange::const_iterator::operator()()
{
    return range_[index_];
}


inline labelRange::const_iterator&
labelRange::const_iterator::operator++()
{
    if (++index_ >= range_.size())
    {
        // equivalent to end iterator
        index_ = -1;
    }

    return *this;
}


inline labelRange::const_iterator
labelRange::const_iterator::operator++(int)
{
    const_iterator old = *this;
    this->operator++();
    return old;
}


inline labelRange::const_iterator labelRange::cbegin() const
{
    return const_iterator(*this);
}


inline const labelRange::const_iterator& labelRange::cend() const
{
    return endIter_;
}


inline labelRange::const_iterator labelRange::begin() const
{
    return const_iterator(*this);
}


inline const labelRange::const_iterator& labelRange::end() const
{
    return endIter_;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline void labelRange::clear()
{
    start_ = size_ = 0;
}


inline bool labelRange::empty() const
{
    return !size_;
}


inline label labelRange::size() const
{
    return size_;
}


inline label labelRange::first() const
{
    return start_;
}


inline label labelRange::last() const
{
    return start_ + size_ - 1;
}


inline bool labelRange::contains(const label value) const
{
    return value >= this->first() && value <= this->last();
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline label labelRange::operator[](const label i) const
{
    return start_ + i;
}


inline bool labelRange::operator<(const labelRange& rhs) const
{
    return this->first() < rhs.first();
}


inline bool labelRange::operator==(const labelRange& rhs) const
{
    return start_ == rhs.start_ && size_ == rhs.size_;
}


inline bool labelRange::operator!=(const labelRange& rhs) const
{
    return !(operator==(rhs));
}

}
// ************************************************************************* //
