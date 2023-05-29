/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2017-2019 OpenCFD Ltd.
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
#include "nullObject.H"

// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //


 namespace Foam{
template<class IteratorType>
inline const IteratorType& SLListBase::iterator_end()
{
    return *reinterpret_cast<const IteratorType*>(nullObjectPtr);
}


template<class IteratorType>
inline IteratorType SLListBase::iterator_first() const
{
    SLListBase* list = const_cast<SLListBase*>(this);

    if (size())
    {
        return IteratorType(list, const_cast<SLListBase::link*>(last_->next_));
    }

    // Return an end iterator
    return IteratorType(list, nullptr);
}


template<class IteratorType>
inline IteratorType SLListBase::iterator_last() const
{
    SLListBase* list = const_cast<SLListBase*>(this);

    if (size())
    {
        return IteratorType(list, const_cast<SLListBase::link*>(last_));
    }

    // Return an end iterator
    return IteratorType(list, nullptr);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline label SLListBase::size() const noexcept
{
    return size_;
}


inline bool SLListBase::empty() const noexcept
{
    return !size_;
}


inline SLListBase::link*
SLListBase::first()
{
    if (!size_)
    {
        FatalErrorInFunction
            << "list is empty"
            << abort(FatalError);
    }
    return last_->next_;
}


inline const SLListBase::link*
SLListBase::first() const
{
    if (!size_)
    {
        FatalErrorInFunction
            << "list is empty"
            << abort(FatalError);
    }
    return last_->next_;
}


inline SLListBase::link*
SLListBase::last()
{
    if (!size_)
    {
        FatalErrorInFunction
            << "list is empty"
            << abort(FatalError);
    }
    return last_;
}


inline const SLListBase::link*
SLListBase::last() const
{
    if (!size_)
    {
        FatalErrorInFunction
            << "list is empty"
            << abort(FatalError);
    }
    return last_;
}


inline void SLListBase::clear()
{
    last_ = nullptr;
    size_ = 0;
}


inline void SLListBase::swap(SLListBase& lst)
{
    if (this == &lst)
    {
        return;  // Self-swap is a no-op
    }

    std::swap(last_, lst.last_);
    std::swap(size_, lst.size_);
}


inline void SLListBase::transfer(SLListBase& lst)
{
    if (this == &lst)
    {
        return;  // Self-assignment is a no-op
    }

    last_ = lst.last_;
    size_ = lst.size_;

    lst.clear();
}


inline SLListBase::link* SLListBase::remove
(
    SLListBase::iterator& iter
)
{
    return remove(iter.node_);
}


// * * * * * * * * * * * * * * * STL iterator  * * * * * * * * * * * * * * * //

inline SLListBase::iterator::iterator
(
    SLListBase* list,
    SLListBase::link* item
)
:
    node_(item),
    list_(list),
    copy_()
{
    if (node_ != nullptr)
    {
        copy_ = *node_;
    }
}


inline SLListBase::link*
SLListBase::iterator::get_node() const
{
    return node_;
}


inline bool SLListBase::iterator::good() const
{
    return (node_ != nullptr);
}


inline void SLListBase::iterator::next()
{
    if (list_)
    {
        if (node_ == list_->last_ || list_->last_ == nullptr)
        {
            node_ = nullptr;
        }
        else
        {
            node_ = copy_.next_;
            copy_ = *node_;
        }
    }
}


inline void SLListBase::iterator::operator=(const iterator& iter)
{
    node_ = iter.node_;
    list_ = iter.list_;
    copy_ = iter.copy_;
}


inline bool SLListBase::iterator::operator==(const iterator& iter) const
{
    return node_ == iter.node_;
}


inline bool SLListBase::iterator::operator!=(const iterator& iter) const
{
    return node_ != iter.node_;
}


inline SLListBase::iterator
SLListBase::begin()
{
    if (size())
    {
        return iterator_first<iterator>();
    }

    return end();
}


inline const SLListBase::iterator&
SLListBase::end()
{
    return iterator_end<SLListBase::iterator>();
}


inline const SLListBase::const_iterator&
SLListBase::cend() const
{
    return iterator_end<SLListBase::const_iterator>();
}


// * * * * * * * * * * * * * * STL const_iterator  * * * * * * * * * * * * * //

inline SLListBase::const_iterator::const_iterator
(
    const SLListBase* list,
    const SLListBase::link* item
)
:
    node_(item),
    list_(list)
{}


inline SLListBase::const_iterator::const_iterator
(
    const SLListBase::iterator& iter
)
:
    node_(iter.node_),
    list_(iter.list_)
{}


inline const SLListBase::link*
SLListBase::const_iterator::get_node() const
{
    return node_;
}


inline bool SLListBase::const_iterator::good() const
{
    return (node_ != nullptr);
}


inline void SLListBase::const_iterator::next()
{
    if (list_)
    {
        if (node_ == list_->last_)
        {
            node_ = nullptr;
        }
        else
        {
            node_ = node_->next_;
        }
    }
}


inline bool SLListBase::const_iterator::operator==
(
    const const_iterator& iter
) const
{
    return node_ == iter.node_;
}


inline bool SLListBase::const_iterator::operator!=
(
    const const_iterator& iter
) const
{
    return node_ != iter.node_;
}


inline SLListBase::const_iterator
SLListBase::cbegin() const
{
    if (size())
    {
        return iterator_first<const_iterator>();
    }

    return cend();
}


// ************************************************************************* //

 } // End namespace Foam
