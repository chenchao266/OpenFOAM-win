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

#include "ListOps.H"
#include "labelRange.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


 namespace Foam{
inline globalIndex::globalIndex
(
    const labelUList& listOffsets
)
{
    if (listOffsets.size() > 1)
    {
        offsets_ = listOffsets;
    }
}


inline globalIndex::globalIndex
(
    labelList&& listOffsets
)
{
    if (listOffsets.size() > 1)
    {
        offsets_.transfer(listOffsets);
    }
    else
    {
        listOffsets.clear();
    }
}


inline globalIndex::globalIndex
(
    const labelUList& offsetsOrSizes,
    enum globalIndex::accessType accType
)
{
    if (accType == accessType::SIZES)
    {
        reset(offsetsOrSizes);
    }
    else if (offsetsOrSizes.size() > 1)
    {
        // accessType::OFFSETS
        offsets_ = offsetsOrSizes;
    }
}


inline globalIndex::globalIndex(const label localSize)
{
    reset(localSize);
}


inline globalIndex::globalIndex
(
    const label localSize,
    const int tag,
    const label comm,
    const bool parallel
)
{
    reset(localSize, tag, comm, parallel);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline bool globalIndex::empty() const
{
    return offsets_.empty() || offsets_.last() == 0;
}


inline label globalIndex::nProcs() const noexcept
{
    const label len = (offsets_.size() - 1);
    return (len < 1) ? static_cast<label>(0) : len;
}


inline labelRange globalIndex::allProcs() const noexcept
{
    // Proc 0 -> nProcs
    const label len = (offsets_.size() - 1);
    return (len < 1) ? labelRange() : labelRange(0, len);
}


inline labelRange globalIndex::subProcs() const noexcept
{
    // Proc 1 -> nProcs
    const label len = (offsets_.size() - 2);
    return (len < 1) ? labelRange() : labelRange(1, len);
}


inline const labelList& globalIndex::offsets() const noexcept
{
    return offsets_;
}


inline labelList& globalIndex::offsets() noexcept
{
    return offsets_;
}


inline const labelUList globalIndex::localStarts() const
{
    const label len = (offsets_.size() - 1);

    if (len < 1) return labelUList::null();

    return labelList::subList(offsets_, len);
}


inline label globalIndex::size() const
{
    return offsets_.empty() ? static_cast<label>(0) : offsets_.last();
}


inline void globalIndex::reset(const label localSize)
{
    reset(localSize, Pstream::msgType(), UPstream::worldComm, true);
}


inline label globalIndex::offset(const label proci) const
{
    return offsets_[proci];
}


inline label globalIndex::localStart(const label proci) const
{
    return offsets_[proci];
}


inline label globalIndex::localStart() const
{
    return localStart(Pstream::myProcNo());
}


inline label globalIndex::localSize(const label proci) const
{
    return offsets_[proci+1] - offsets_[proci];
}


inline label globalIndex::localSize() const
{
    return localSize(Pstream::myProcNo());
}


inline label globalIndex::maxSize() const
{
    // Use out-of-range proci to avoid excluding any processor
    return maxNonLocalSize(-1);
}


inline label globalIndex::maxNonLocalSize() const
{
    return maxNonLocalSize(Pstream::myProcNo());
}


inline labelRange globalIndex::range(const label proci) const
{
    return labelRange(offsets_[proci], offsets_[proci+1] - offsets_[proci]);
}


inline labelRange globalIndex::range() const
{
    return range(Pstream::myProcNo());
}


inline bool globalIndex::isLocal(const label proci, const label i) const
{
    return i >= offsets_[proci] && i < offsets_[proci+1];
}


inline bool globalIndex::isLocal(const label i) const
{
    return isLocal(Pstream::myProcNo(), i);
}


inline label globalIndex::toGlobal
(
    const label proci,
    const label i
) const
{
    return i + offsets_[proci];
}


inline label globalIndex::toGlobal(const label i) const
{
    return toGlobal(Pstream::myProcNo(), i);
}


inline labelList globalIndex::toGlobal
(
    const label proci,
    const labelUList& labels
) const
{
    labelList result(labels);
    inplaceToGlobal(proci, result);

    return result;
}


inline labelList globalIndex::toGlobal
(
    const labelUList& labels
) const
{
    return toGlobal(Pstream::myProcNo(), labels);
}


inline void globalIndex::inplaceToGlobal
(
    const label proci,
    labelList& labels
) const
{
    const label off = offsets_[proci];

    for (label& val : labels)
    {
        val += off;
    }
}


inline void globalIndex::inplaceToGlobal(labelList& labels) const
{
    inplaceToGlobal(Pstream::myProcNo(), labels);
}


inline label
globalIndex::toLocal(const label proci, const label i) const
{
    const label locali = i - offsets_[proci];

    if (locali < 0 || i >= offsets_[proci+1])
    {
        FatalErrorInFunction
            << "Global " << i << " does not belong on processor "
            << proci << nl << "Offsets:" << offsets_
            << abort(FatalError);
    }
    return locali;
}


inline label globalIndex::toLocal(const label i) const
{
    return toLocal(Pstream::myProcNo(), i);
}


inline label globalIndex::whichProcID(const label i) const
{
    if (i < 0 || i >= size())
    {
        FatalErrorInFunction
            << "Global " << i << " does not belong on any processor."
            << " Offsets:" << offsets_
            << abort(FatalError);
    }

    return findLower(offsets_, i+1);
}


// * * * * * * * * * * * * * * * * Iterators * * * * * * * * * * * * * * * * //

inline globalIndex::const_iterator::
const_iterator
(
    const globalIndex* globalIdx,
    const label i
) noexcept
:
    parent_(globalIdx),
    proci_(i)
{}


inline label globalIndex::const_iterator::
proci() const noexcept
{
    return proci_;
}


inline label globalIndex::const_iterator::
start() const
{
    return (*parent_).localStart(proci_);
}


inline label globalIndex::const_iterator::
size() const
{
    return (*parent_).localSize(proci_);
}


inline labelRange globalIndex::const_iterator::
range() const
{
    return (*parent_).range(proci_);
}


inline labelRange globalIndex::const_iterator::
operator*() const
{
    return this->range();
}


inline globalIndex::const_iterator&
globalIndex::const_iterator::
operator++()
{
    ++proci_;
    return *this;
}


inline globalIndex::const_iterator
globalIndex::const_iterator::
operator++(int)
{
    const_iterator old(*this);
    ++proci_;
    return old;
}


inline globalIndex::const_iterator&
globalIndex::const_iterator::
operator--()
{
    --proci_;
    return *this;
}


inline globalIndex::const_iterator
globalIndex::const_iterator::
operator--(int)
{
    const_iterator old(*this);
    --proci_;
    return old;
}


inline bool
globalIndex::const_iterator::
operator==
(
    const const_iterator& iter
) const noexcept
{
    return (proci_ == iter.proci_);
}


inline bool
globalIndex::const_iterator::
operator!=
(
    const const_iterator& iter
) const noexcept
{
    return (proci_ != iter.proci_);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline globalIndex::const_iterator
globalIndex::cbegin() const noexcept
{
    return const_iterator(this);
}


inline const globalIndex::const_iterator
globalIndex::cend() const noexcept
{
    return const_iterator(this, this->nProcs());
}


inline globalIndex::const_iterator
globalIndex::begin() const noexcept
{
    return const_iterator(this);
}


inline const globalIndex::const_iterator
globalIndex::end() const noexcept
{
    return const_iterator(this, this->nProcs());
}


// ************************************************************************* //

 } // End namespace Foam
