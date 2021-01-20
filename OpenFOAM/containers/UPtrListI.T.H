/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2016 OpenFOAM Foundation
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

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //
namespace Foam {
    template<class T>
    inline label UPtrList<T>::size() const
    {
        return ptrs_.size();
    }


    template<class T>
    inline bool UPtrList<T>::empty() const
    {
        return ptrs_.empty();
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
        return this->operator[](this->size() - 1);
    }


    template<class T>
    inline const T& UPtrList<T>::last() const
    {
        return this->operator[](this->size() - 1);
    }


    template<class T>
    inline void UPtrList<T>::resize(const label newSize)
    {
        this->setSize(newSize);
    }


    template<class T>
    inline bool UPtrList<T>::set(const label i) const
    {
        return ptrs_[i] != nullptr;
    }


    template<class T>
    inline T* UPtrList<T>::set(const label i, T* ptr)
    {
        T* old = ptrs_[i];
        ptrs_[i] = ptr;
        return old;
    }


    template<class T>
    inline Xfer<UPtrList<T>> UPtrList<T>::xfer()
    {
        return xferMove(*this);
    }


    // * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

    template<class T>
    inline const T& UPtrList<T>::operator[](const label i) const
    {
        if (!ptrs_[i])
        {
            FatalErrorInFunction
                << "hanging pointer at index " << i
                << " (size " << size()
                << "), cannot dereference"
                << abort(FatalError);
        }

        return *(ptrs_[i]);
    }


    template<class T>
    inline T& UPtrList<T>::operator[](const label i)
    {
        if (!ptrs_[i])
        {
            FatalErrorInFunction
                << "hanging pointer at index " << i
                << " (size " << size()
                << "), cannot dereference"
                << abort(FatalError);
        }

        return *(ptrs_[i]);
    }


    template<class T>
    inline const T* UPtrList<T>::operator()(const label i) const
    {
        return ptrs_[i];
    }


    // * * * * * * * * * * * * * * * * STL iterator  * * * * * * * * * * * * * * //

    template<class T>
    inline UPtrList<T>::iterator::iterator(T** ptr)
        :
        ptr_(ptr)
    {}

    template<class T>
    inline bool UPtrList<T>::iterator::operator==(const iterator& iter) const
    {
        return ptr_ == iter.ptr_;
    }

    template<class T>
    inline bool UPtrList<T>::iterator::operator!=(const iterator& iter) const
    {
        return ptr_ != iter.ptr_;
    }

    template<class T>
    inline T& UPtrList<T>::iterator::operator*()
    {
        return **ptr_;
    }

    template<class T>
    inline T& UPtrList<T>::iterator::operator()()
    {
        return operator*();
    }

    template<class T>
    inline typename UPtrList<T>::iterator
        UPtrList<T>::iterator::operator++()
    {
        ++ptr_;
        return *this;
    }

    template<class T>
    inline typename UPtrList<T>::iterator
        UPtrList<T>::iterator::operator++(int)
    {
        iterator tmp = *this;
        ++ptr_;
        return tmp;
    }

    template<class T>
    inline typename UPtrList<T>::iterator
        UPtrList<T>::iterator::operator--()
    {
        --ptr_;
        return *this;
    }

    template<class T>
    inline typename UPtrList<T>::iterator
        UPtrList<T>::iterator::operator--(int)
    {
        iterator tmp = *this;
        --ptr_;
        return tmp;
    }

    template<class T>
    inline typename UPtrList<T>::iterator
        UPtrList<T>::iterator::operator+=(label n)
    {
        ptr_ += n;
        return *this;
    }

    template<class T>
    inline typename UPtrList<T>::iterator
        operator+(const typename UPtrList<T>::iterator& iter, label n)
    {
        typename UPtrList<T>::iterator tmp = iter;
        return tmp += n;
    }

    template<class T>
    inline typename UPtrList<T>::iterator
        operator+(label n, const typename UPtrList<T>::iterator& iter)
    {
        typename UPtrList<T>::iterator tmp = iter;
        return tmp += n;
    }

    template<class T>
    inline typename UPtrList<T>::iterator
        UPtrList<T>::iterator::operator-=(label n)
    {
        ptr_ -= n;
        return *this;
    }

    template<class T>
    inline typename UPtrList<T>::iterator
        operator-(const typename UPtrList<T>::iterator& iter, label n)
    {
        typename UPtrList<T>::iterator tmp = iter;
        return tmp -= n;
    }

    template<class T>
    inline label operator-
        (
            const typename UPtrList<T>::iterator& iter1,
            const typename UPtrList<T>::iterator& iter2
            )
    {
        return (iter1.ptr_ - iter2.ptr_) / sizeof(T*);
    }

    template<class T>
    inline T& UPtrList<T>::iterator::operator[](label n)
    {
        return *(*this + n);
    }

    template<class T>
    inline bool UPtrList<T>::iterator::operator<(const iterator& iter) const
    {
        return ptr_ < iter.ptr_;
    }

    template<class T>
    inline bool UPtrList<T>::iterator::operator>(const iterator& iter) const
    {
        return ptr_ > iter.ptr_;
    }

    template<class T>
    inline bool UPtrList<T>::iterator::operator<=(const iterator& iter) const
    {
        return ptr_ <= iter.ptr_;
    }

    template<class T>
    inline bool UPtrList<T>::iterator::operator>=(const iterator& iter) const
    {
        return ptr_ >= iter.ptr_;
    }

    template<class T>
    inline typename UPtrList<T>::iterator
        UPtrList<T>::begin()
    {
        return ptrs_.begin();
    }

    template<class T>
    inline typename UPtrList<T>::iterator
        UPtrList<T>::end()
    {
        return ptrs_.end();
    }


    // * * * * * * * * * * * * * * * STL const_iterator  * * * * * * * * * * * * //

    template<class T>
    inline UPtrList<T>::const_iterator::const_iterator(const T* const* ptr)
        :
        ptr_(ptr)
    {}


    template<class T>
    inline UPtrList<T>::const_iterator::const_iterator(const iterator& iter)
        :
        ptr_(iter.ptr_)
    {}


    template<class T>
    inline bool UPtrList<T>::const_iterator::operator==
        (
            const const_iterator& iter
            ) const
    {
        return ptr_ == iter.ptr_;
    }


    template<class T>
    inline bool UPtrList<T>::const_iterator::operator!=
        (
            const const_iterator& iter
            ) const
    {
        return ptr_ != iter.ptr_;
    }


    template<class T>
    inline const T& UPtrList<T>::const_iterator::operator*()
    {
        return **ptr_;
    }


    template<class T>
    inline const T& UPtrList<T>::const_iterator::operator()()
    {
        return operator*();
    }


    template<class T>
    inline typename UPtrList<T>::const_iterator
        UPtrList<T>::const_iterator::operator++()
    {
        ++ptr_;
        return *this;
    }


    template<class T>
    inline typename UPtrList<T>::const_iterator
        UPtrList<T>::const_iterator::operator++(int)
    {
        const_iterator tmp = *this;
        ++ptr_;
        return tmp;
    }


    template<class T>
    inline typename UPtrList<T>::const_iterator
        UPtrList<T>::const_iterator::operator--()
    {
        --ptr_;
        return *this;
    }


    template<class T>
    inline typename UPtrList<T>::const_iterator
        UPtrList<T>::const_iterator::operator--(int)
    {
        const_iterator tmp = *this;
        --ptr_;
        return tmp;
    }


    template<class T>
    inline typename UPtrList<T>::const_iterator
        UPtrList<T>::const_iterator::operator+=(label n)
    {
        ptr_ += n;
        return *this;
    }


    template<class T>
    inline typename UPtrList<T>::const_iterator
        operator+(const typename UPtrList<T>::const_iterator& iter, label n)
    {
        typename UPtrList<T>::const_iterator tmp = iter;
        return tmp += n;
    }


    template<class T>
    inline typename UPtrList<T>::const_iterator
        operator+(label n, const typename UPtrList<T>::const_iterator& iter)
    {
        typename UPtrList<T>::const_iterator tmp = iter;
        return tmp += n;
    }


    template<class T>
    inline typename UPtrList<T>::const_iterator
        UPtrList<T>::const_iterator::operator-=(label n)
    {
        ptr_ -= n;
        return *this;
    }


    template<class T>
    inline typename UPtrList<T>::const_iterator
        operator-(const typename UPtrList<T>::const_iterator& iter, label n)
    {
        typename UPtrList<T>::const_iterator tmp = iter;
        return tmp -= n;
    }


    template<class T>
    inline label operator-
        (
            const typename UPtrList<T>::const_iterator& iter1,
            const typename UPtrList<T>::const_iterator& iter2
            )
    {
        return (iter1.ptr_ - iter2.ptr_) / sizeof(T*);
    }


    template<class T>
    inline const T& UPtrList<T>::const_iterator::operator[](label n)
    {
        return *(*this + n);
    }


    template<class T>
    inline bool UPtrList<T>::const_iterator::operator<
        (
            const const_iterator& iter
            ) const
    {
        return ptr_ < iter.ptr_;
    }


    template<class T>
    inline bool UPtrList<T>::const_iterator::operator>
        (
            const const_iterator& iter
            ) const
    {
        return ptr_ > iter.ptr_;
    }


    template<class T>
    inline bool UPtrList<T>::const_iterator::operator<=
        (
            const const_iterator& iter
            ) const
    {
        return ptr_ <= iter.ptr_;
    }


    template<class T>
    inline bool UPtrList<T>::const_iterator::operator>=
        (
            const const_iterator& iter
            ) const
    {
        return ptr_ >= iter.ptr_;
    }


    template<class T>
    inline typename UPtrList<T>::const_iterator
        UPtrList<T>::begin() const
    {
        return ptrs_.begin();
    }


    template<class T>
    inline typename UPtrList<T>::const_iterator
        UPtrList<T>::end() const
    {
        return ptrs_.end();
    }


    template<class T>
    inline typename UPtrList<T>::const_iterator
        UPtrList<T>::cbegin() const
    {
        return ptrs_.begin();
    }


    template<class T>
    inline typename UPtrList<T>::const_iterator
        UPtrList<T>::cend() const
    {
        return ptrs_.end();
    }

}
// ************************************************************************* //
