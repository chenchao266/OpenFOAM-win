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

\*---------------------------------------------------------------------------*/

#include "error.H"

// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //


 namespace Foam{
template<unsigned Width>
inline unsigned int PackedList<Width>::repeated_value(unsigned val)
{
    return BitOps::repeat_value<block_type,Width>(val);
}


template<unsigned Width>
inline unsigned int PackedList<Width>::readValue(Istream& is)
{
    const unsigned int val = readLabel(is);

    if (val > max_value)
    {
        FatalIOErrorInFunction(is)
            << "Out-of-range value " << val << " for PackedList<" << Width
            << ">. Maximum permitted value is " << max_value << "."
            << exit(FatalIOError);
    }

    return val;
}


template<unsigned Width>
inline void PackedList<Width>::setPair(Istream& is)
{
    is.readBegin("Tuple2<label,uint32>");

    const label ind = readLabel(is);
    const unsigned int val = readLabel(is);

    is.readEnd("Tuple2<label,uint32>");

    if (val > max_value)
    {
        FatalIOErrorInFunction(is)
            << "Out-of-range value " << val << " for PackedList<" << Width
            << "> at index " << ind
            << ". Maximum permitted value is " << max_value << "."
            << exit(FatalIOError);
    }

    set(ind, val);

    is.check(FUNCTION_NAME);
}


template<unsigned Width>
inline void PackedList<Width>::clear_trailing_bits()
{
    // Mask off any partial rubbish in final block
    const unsigned int blk = size() / elem_per_block;
    const unsigned int off = size() % elem_per_block;

    if (off)
    {
        blocks_[blk] &= mask_lower(off);
    }
}


template<unsigned Width>
inline bool PackedList<Width>::trim(label minpos)
{
    if (empty())
    {
        return false;  // Trivial case
    }

    const label orig = size();
    if (orig < minpos)
    {
        minpos = orig;   // Don't allow allow accidental growth!
    }

    for (label blocki = num_blocks(size())-1; blocki >= 0; --blocki)
    {
        // Truncate to the block begin
        size_ = blocki * elem_per_block;

        unsigned int blockval = blocks_[blocki];

        // Some bits were found in the block, increment size again
        if (blockval)
        {
            for (; blockval; ++size_)
            {
                blockval >>= Width;
            }
            break;
        }
        else if (size_ < minpos)
        {
            break;
        }
    }

    if (size_ < minpos)
    {
        size_ = minpos;
    }

    return (size() != orig);
}


template<unsigned Width>
inline void PackedList<Width>::copyAssign(const PackedList<Width>& rhs)
{
    // Self-assignment silently ignored
    blocks_ = rhs.blocks_;
    size_ = rhs.size_;
}


// * * * * * * * * * * * * * * * Specializations * * * * * * * * * * * * * * //


 } // End namespace Foam
namespace Foam
{

// constexpr noexcept
template<> inline unsigned int PackedList<1>::repeated_value(unsigned val)
{
    return (val ? ~0u : 0u);
}

template<> inline unsigned int PackedList<1>::readValue(Istream& is)
{
    return readBool(is);
}

template<> inline void PackedList<1>::setPair(Istream& is)
{
    set(readLabel(is), true);
}

} // End namespace Foam


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


 namespace Foam{
template<unsigned Width>
inline constexpr PackedList<Width>::PackedList() noexcept
:
    blocks_(),
    size_(0)
{}


template<unsigned Width>
inline PackedList<Width>::PackedList(const label numElem)
:
    blocks_(num_blocks(numElem), 0u),
    size_(numElem)
{}


template<unsigned Width>
inline PackedList<Width>::PackedList
(
    const label numElem,
    const unsigned int val
)
:
    blocks_(num_blocks(numElem), 0u),
    size_(numElem)
{
    if (val)
    {
        operator=(val);
    }
}


template<unsigned Width>
inline PackedList<Width>::PackedList(Istream& is)
:
    blocks_(),
    size_(0)
{
    readList(is);
}


template<unsigned Width>
inline PackedList<Width>::PackedList(const PackedList<Width>& list)
:
    blocks_(list.blocks_),
    size_(list.size_)
{}


template<unsigned Width>
inline PackedList<Width>::PackedList(PackedList<Width>&& list)
:
    blocks_(std::move(list.blocks_)),
    size_(list.size_)
{
    list.size_ = 0;
}


template<unsigned Width>
inline PackedList<Width>::PackedList(const labelUList& values)
:
    blocks_(num_blocks(values.size()), 0u),
    size_(values.size())
{
    const label len = values.size();

    // Could add more intelligent filling (blockwise),
    // but likely done fairly infrequently

    for (label i = 0; i < len; ++i)
    {
        const unsigned int val(values[i]);
        if (val) set(i, val);
    }
}


template<unsigned Width>
template<class Addr>
inline PackedList<Width>::PackedList
(
    const IndirectListBase<label, Addr>& values
)
:
    blocks_(num_blocks(values.size()), 0u),
    size_(values.size())
{
    const label len = values.size();

    // Could add more intelligent filling (blockwise),
    // but likely done fairly infrequently

    for (label i = 0; i < len; ++i)
    {
        const unsigned int val(values[i]);
        if (val) set(i, val);
    }
}


template<unsigned Width>
inline autoPtr<PackedList<Width>>
PackedList<Width>::clone() const
{
    return autoPtr<PackedList<Width>>::New(*this);
}


// * * * * * * * * * * * * * * * * References * * * * * * * * * * * * * * * * //

template<unsigned Width>
inline PackedList<Width>::reference::reference
(
    PackedList<Width>* parent,
    const label index
)
:
    ref_(parent->blocks_[index / elem_per_block]),
    shift_(Width * (index % elem_per_block))
{}


template<unsigned Width>
inline unsigned int PackedList<Width>::reference::get() const
{
    return ((ref_ >> shift_) & max_value);
}


template<unsigned Width>
inline bool PackedList<Width>::reference::set(const unsigned int val)
{
    const unsigned int mask = (max_value << shift_);
    const unsigned int prev = ref_;

    if (val >= max_value)
    {
        ref_ |= mask;  // Overflow is max_value, so fill entirely
    }
    else
    {
        ref_ &= ~mask;
        ref_ |= mask & (val << shift_);
    }

    return (prev != ref_);
}


template<unsigned Width>
inline void PackedList<Width>::reference::operator=
(
    const reference& other
)
{
    // Accepts self-assignment
    this->set(other.get());
}


template<unsigned Width>
inline void PackedList<Width>::reference::operator=
(
    const unsigned int val
)
{
    this->set(val);
}


template<unsigned Width>
inline PackedList<Width>::reference::operator unsigned int () const
{
    return this->get();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<unsigned Width>
inline void PackedList<Width>::checkIndex(const label i) const
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
            << "index " << i << " out of range [0," << size_ << ")"
            << abort(FatalError);
    }
}


template<unsigned Width>
inline label PackedList<Width>::size() const noexcept
{
    return size_;
}


template<unsigned Width>
inline bool PackedList<Width>::empty() const noexcept
{
    return !size_;
}


template<unsigned Width>
inline label PackedList<Width>::capacity() const noexcept
{
    return elem_per_block * blocks_.size();
}


template<unsigned Width>
inline void PackedList<Width>::resize_nocopy
(
    const label numElem
)
{
    this->resize(numElem);
}


template<unsigned Width>
inline void PackedList<Width>::resize
(
    const label newSize,
    const unsigned int val
)
{
    reserve(newSize);

    const label oldSize = size();
    size_ = newSize;

    if (oldSize < newSize)
    {
        // Fill new elements or newly exposed elements
        if (val)
        {
            // Fill value for complete blocks
            const unsigned int blockval = repeated_value(val);

            // Fill complete blocks
            const label oldLen = num_blocks(oldSize);
            const label newLen = num_blocks(size());
            for (label blocki = oldLen; blocki < newLen; ++blocki)
            {
                blocks_[blocki] = blockval;
            }

            // Finish previous partial block, preserve existing value
            {
                const unsigned int blk = oldSize / elem_per_block;
                const unsigned int off = oldSize % elem_per_block;
                if (off)
                {
                    const unsigned int mask = mask_lower(off);

                    blocks_[blk] &= mask;
                    blocks_[blk] |= ~mask & blockval;
                }
            }

            clear_trailing_bits();
        }
    }
    else if (newSize < oldSize)
    {
        // The list is now shorter than before, so we zero assign the unused
        // blocks and any trailing junk. This costs slightly here, but make
        // things much simpler elsewhere.

        // Clear complete blocks
        const label oldLen = num_blocks(oldSize);
        const label newLen = num_blocks(size());
        for (label blocki = newLen; blocki < oldLen; ++blocki)
        {
            blocks_[blocki] = 0u;
        }

        clear_trailing_bits();
    }
}


template<unsigned Width>
inline void PackedList<Width>::setCapacity(const label numElem)
{
    const label nblocks = num_blocks(numElem);

    blocks_.resize(nblocks, 0u);

    if (numElem < size())
    {
        size_ = numElem;
        clear_trailing_bits();
    }
}


template<unsigned Width>
inline void PackedList<Width>::reserve(const label numElem)
{
    const label oldLen = blocks_.size();
    const label newLen = num_blocks(numElem);

    // Allocate more capacity if necessary
    if (oldLen < newLen)
    {
        blocks_.resize
        (
            // SizeMin=16, allocation doubling
            max(16, max(newLen, 2*oldLen)),
            0u
        );
    }
}


template<unsigned Width>
inline void PackedList<Width>::reset()
{
    blocks_ = 0u;
}


template<unsigned Width>
inline void PackedList<Width>::clear()
{
    reset();
    size_ = 0;
}


template<unsigned Width>
inline void PackedList<Width>::clearStorage()
{
    blocks_.clear();
    size_ = 0;
}


template<unsigned Width>
inline void PackedList<Width>::shrink()
{
    // Any unneeded space allocated?
    const label nblocks = num_blocks(size());
    if (nblocks < blocks_.size())
    {
        blocks_.resize(nblocks);
    }
}


template<unsigned Width>
inline List<unsigned int>& PackedList<Width>::storage()
{
    return blocks_;
}


template<unsigned Width>
inline const List<unsigned int>& PackedList<Width>::storage() const
{
    return blocks_;
}


template<unsigned Width>
inline label PackedList<Width>::nBlocks() const
{
    return num_blocks(size());
}


template<unsigned Width>
inline const unsigned int* PackedList<Width>::cdata() const noexcept
{
    return blocks_.cdata();
}


template<unsigned Width>
inline unsigned int* PackedList<Width>::data() noexcept
{
    return blocks_.data();
}


template<unsigned Width>
inline const char* PackedList<Width>::cdata_bytes() const noexcept
{
    return blocks_.cdata_bytes();
}


template<unsigned Width>
inline char* PackedList<Width>::data_bytes() noexcept
{
    return blocks_.data_bytes();
}


template<unsigned Width>
inline std::streamsize PackedList<Width>::size_bytes() const noexcept
{
    return num_blocks(size()) * sizeof(block_type);
}


template<unsigned Width>
inline std::streamsize PackedList<Width>::byteSize() const noexcept
{
    return this->size_bytes();
}


template<unsigned Width>
inline void PackedList<Width>::swap(PackedList<Width>& rhs)
{
    if (this == &rhs)
    {
        return;  // Self-swap is a no-op
    }

    blocks_.swap(rhs.blocks_);
    std::swap(size_, rhs.size_);
}


template<unsigned Width>
inline void PackedList<Width>::transfer(PackedList<Width>& rhs)
{
    if (this == &rhs)
    {
        return;  // Self-assignment is a no-op
    }

    blocks_.transfer(rhs.blocks_);
    size_ = rhs.size_;
    rhs.size_ = 0;
}


template<unsigned Width>
inline unsigned int PackedList<Width>::get(const label i) const
{
    if (i < 0 || i >= size())
    {
        #ifdef FULLDEBUG
        if (i < 0)
        {
            WarningInFunction
                << "Ignoring attempt to get a negative index " << i
                << " range is [0," << size_ << ")"
                << endl;
        }
        #endif

        return 0u;        // Out-of-bounds (lazy): return 0 (false)
    }

    return reference(const_cast<PackedList<Width>*>(this), i).get();
}


template<unsigned Width>
inline bool PackedList<Width>::set
(
    const label i,
    const unsigned int val
)
{
    if (i < 0)
    {
        #ifdef FULLDEBUG
        WarningInFunction
            << "Ignoring attempt to set a negative index " << i
            << " range is [0," << size_ << ")"
            << endl;
        #endif

        return false;       // Out-of-bounds: ignore
    }
    else if (i >= size())
    {
        if (!val)           // Unset out-of-bounds: ignore
        {
            return false;
        }

        resize(i + 1);      // Lazy evaluation: adjust size for assign
    }

    return reference(this, i).set(val);
}


template<unsigned Width>
inline bool PackedList<Width>::unset(const label i)
{
    if (i < 0 || i >= size())
    {
        return false;       // Unset out-of-bounds: ignore
    }

    return reference(this, i).set(0u);
}


template<unsigned Width>
inline PackedList<Width>&
PackedList<Width>::append(const unsigned int val)
{
    const label idx = size();
    reserve(idx + 1);
    ++size_;

    reference(this, idx).set(val);
    return *this;
}


template<unsigned Width>
inline unsigned int PackedList<Width>::remove()
{
    // Location of last element and simultaneously the new size
    const label idx = size()-1;

    if (idx < 0)
    {
        FatalErrorInFunction
            << "List is empty" << abort(FatalError);
    }

    const unsigned int old = reference(this, idx).get();
    resize(idx);

    return old;
}


template<unsigned Width>
inline void PackedList<Width>::fill(const unsigned int val)
{
    if (empty())
    {
        return;  // Trivial case
    }

    const label nblocks = num_blocks(size());

    // Fill value for complete blocks
    const unsigned int blockval = (val ? repeated_value(val) : 0u);

    for (label blocki=0; blocki < nblocks; ++blocki)
    {
        blocks_[blocki] = blockval;
    }

    if (val)
    {
        clear_trailing_bits();
    }
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<unsigned Width>
inline unsigned int PackedList<Width>::operator[](const label i) const
{
    return get(i);
}


template<unsigned Width>
inline typename PackedList<Width>::reference
PackedList<Width>::operator[](const label i)
{
    #ifdef FULLDEBUG
    checkIndex(i);
    #endif
    return reference(this, i);
}


template<unsigned Width>
inline void PackedList<Width>::operator=(const PackedList<Width>& rhs)
{
    copyAssign(rhs);
}


template<unsigned Width>
inline void PackedList<Width>::operator=(PackedList<Width>&& rhs)
{
    transfer(rhs);
}


template<unsigned Width>
inline void PackedList<Width>::operator=(const unsigned int val)
{
    fill(val);
}


// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

template<unsigned Width>
inline bool operator==
(
    const PackedList<Width>& a,
    const PackedList<Width>& b
)
{
    return a.equal(b);
}


template<unsigned Width>
inline bool operator!=
(
    const PackedList<Width>& a,
    const PackedList<Width>& b
)
{
    return !a.equal(b);
}


// ************************************************************************* //

 } // End namespace Foam
