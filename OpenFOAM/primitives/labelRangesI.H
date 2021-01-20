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
inline labelRanges::labelRanges() :    ParentType()
{}


inline labelRanges::labelRanges(const label nElem) :    ParentType(nElem)
{}


// * * * * * * * * * * * * * * * * Iterators * * * * * * * * * * * * * * * * //

inline labelRanges::const_iterator::const_iterator()
:
   list_(endLabelRanges_),
   index_(-1),
   subIndex_(-1)
{}


inline labelRanges::const_iterator::const_iterator(const labelRanges& lst)
:
   list_(lst),
   index_(0),
   subIndex_(0)
{
    if (list_.empty())
    {
        // equivalent to end iterator
        index_ = subIndex_ = -1;
    }
}


inline bool labelRanges::const_iterator::operator==
(
    const const_iterator& iter
) const
{
    return
    (
        this->index_ == iter.index_
     && this->subIndex_ == iter.subIndex_
    );
}


inline bool labelRanges::const_iterator::operator!=
(
    const const_iterator& iter
) const
{
    return !(this->operator==(iter));
}


inline label labelRanges::const_iterator::operator*()
{
    return list_[index_][subIndex_];
}


inline label labelRanges::const_iterator::operator()()
{
    return list_[index_][subIndex_];
}


inline labelRanges::const_iterator&
labelRanges::const_iterator::operator++()
{
    if (++subIndex_ >= list_[index_].size())
    {
        // go to next list entry
        subIndex_ = 0;
        if (++index_ >= list_.size())
        {
            // equivalent to end iterator
            index_ = subIndex_ = -1;
        }
    }

    return *this;
}


inline labelRanges::const_iterator
labelRanges::const_iterator::operator++(int)
{
    const_iterator old = *this;
    this->operator++();
    return old;
}


inline labelRanges::const_iterator labelRanges::cbegin() const
{
    return const_iterator(*this);
}


inline const labelRanges::const_iterator& labelRanges::cend() const
{
    return endIter_;
}


inline labelRanges::const_iterator labelRanges::begin() const
{
    return const_iterator(*this);
}


inline const labelRanges::const_iterator& labelRanges::end() const
{
    return endIter_;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline bool labelRanges::contains(const label value) const
{
    forAll(*this, i)
    {
        if (this->ParentType::operator[](i).contains(value))
        {
            return true;
        }
    }

    return false;
}

}
// ************************************************************************* //
