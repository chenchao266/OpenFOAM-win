/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
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

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


 namespace Foam{
template<class EnumType>
inline bool Enum<EnumType>::empty() const noexcept
{
    return keys_.empty();
}


template<class EnumType>
inline label Enum<EnumType>::size() const noexcept
{
    return keys_.size();
}


template<class EnumType>
inline const List<word>&
Enum<EnumType>::names() const noexcept
{
    return keys_;
}


template<class EnumType>
inline const List<int>&
Enum<EnumType>::values() const noexcept
{
    return vals_;
}


template<class EnumType>
inline const List<word>&
Enum<EnumType>::toc() const noexcept
{
    return keys_;
}


template<class EnumType>
inline List<word>
Enum<EnumType>::sortedToc() const
{
    List<word> list(keys_);

    sort(list);

    return list;
}


template<class EnumType>
inline void Enum<EnumType>::clear()
{
    keys_.clear();
    vals_.clear();
}


template<class EnumType>
inline label Enum<EnumType>::find(const word& enumName) const
{
    return keys_.find(enumName);
}


template<class EnumType>
inline label Enum<EnumType>::find(const EnumType e) const
{
    return vals_.find(int(e));
}


template<class EnumType>
inline bool Enum<EnumType>::found(const word& enumName) const
{
    return keys_.found(enumName);
}


template<class EnumType>
inline bool Enum<EnumType>::found(const EnumType e) const
{
    return vals_.found(int(e));
}


template<class EnumType>
inline const word& Enum<EnumType>::get(const EnumType e) const
{
    const label idx = find(e);

    if (idx < 0)
    {
        return word::null;
    }

    return keys_[idx];
}


template<class EnumType>
inline bool Enum<EnumType>::readIfPresent
(
    const word& key,
    const dictionary& dict,
    EnumType& val
) const
{
    // Reading is non-mandatory
    return readEntry(key, dict, val, false);
}


template<class EnumType>
inline void Enum<EnumType>::write(const EnumType e, Ostream& os) const
{
    const label idx = find(e);

    if (idx >= 0)
    {
        os << keys_[idx];
    }
}


template<class EnumType>
template<class OS>
inline OS& Enum<EnumType>::writeList(OS& os, const label) const
{
    unsigned i = 0;

    os << '(';
    for (const word& k : keys_)
    {
        if (i++) os << ' ';
        os << k;
    }
    os << ')';

    return os;
}


// * * * * * * * * * * * * * * * * Iterators * * * * * * * * * * * * * * * * //

template<class EnumType>
inline Enum<EnumType>::const_iterator::const_iterator
(
    const Enum* eptr,
    const label idx
) noexcept
:
    ptr_(eptr),
    idx_(idx)
{}


template<class EnumType>
inline const word&
Enum<EnumType>::const_iterator::key() const
{
    return ptr_->names()[idx_];
}


template<class EnumType>
inline EnumType Enum<EnumType>::const_iterator::val() const
{
    return EnumType(ptr_->values()[idx_]);
}


template<class EnumType>
inline typename Enum<EnumType>::const_iterator&
Enum<EnumType>::const_iterator::operator++() noexcept
{
    ++idx_;
    return *this;
}


template<class EnumType>
inline bool Enum<EnumType>::const_iterator::operator==
(
    const const_iterator& iter
) const noexcept
{
    return idx_ == iter.idx_;
}


template<class EnumType>
inline bool Enum<EnumType>::const_iterator::operator!=
(
    const const_iterator& iter
) const noexcept
{
    return idx_ != iter.idx_;
}


template<class EnumType>
inline typename Enum<EnumType>::const_iterator
Enum<EnumType>::cbegin() const noexcept
{
    return typename Enum<EnumType>::const_iterator(this);
}


template<class EnumType>
inline typename Enum<EnumType>::const_iterator
Enum<EnumType>::cend() const noexcept
{
    return typename Enum<EnumType>::const_iterator(this, this->size());
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class EnumType>
inline EnumType Enum<EnumType>::operator[]
(
    const word& enumName
) const
{
    return get(enumName);
}


template<class EnumType>
inline const word& Enum<EnumType>::operator[]
(
    const EnumType e
) const
{
    return get(e);
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class EnumType>
inline Ostream& operator<<
(
    Ostream& os,
    const Enum<EnumType>& list
)
{
    return list.writeList(os);
}


template<class EnumType>
inline std::ostream& operator<<
(
    std::ostream& os,
    const Enum<EnumType>& list
)
{
    return list.writeList(os);
}


// ************************************************************************* //

 } // End namespace Foam
