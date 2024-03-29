﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
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

// * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * //


 namespace Foam{
template<class T>
inline constexpr Detail::PtrListDetail<T>::PtrListDetail() noexcept
:
    List<T*>()
{}


template<class T>
inline Detail::PtrListDetail<T>::PtrListDetail(const label len)
:
    List<T*>(len, reinterpret_cast<T*>(0))
{}


template<class T>
inline Detail::PtrListDetail<T>::PtrListDetail
(
    const UList<T*>& list
)
:
    List<T*>(list)
{}


template<class T>
inline Detail::PtrListDetail<T>::PtrListDetail
(
    const PtrListDetail<T>& list
)
:
    List<T*>(list)
{}


template<class T>
inline Detail::PtrListDetail<T>::PtrListDetail
(
    PtrListDetail<T>&& list
)
:
    List<T*>(std::move(list))
{}


template<class T>
inline Detail::PtrListDetail<T>::PtrListDetail
(
    PtrListDetail<T>& list,
    bool reuse
)
:
    List<T*>(list, reuse)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T>
inline void Detail::PtrListDetail<T>::setAddressableSize
(
    const label n
) noexcept
{
    List<T*>::setAddressableSize(n);
}


template<class T>
inline void Detail::PtrListDetail<T>::resize(const label newLen)
{
    if (newLen <= 0)
    {
        List<T*>::clear();
    }
    else if (newLen != List<T*>::size())
    {
        // Truncate or extend. Any new elements are initialized to nullptr.
        List<T*>::resize(newLen, reinterpret_cast<T*>(0));
    }
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class T>
inline void Detail::PtrListDetail<T>::operator=
(
    const PtrListDetail<T>& list
)
{
    List<T*>::operator=(list); // shallow copy
}


template<class T>
inline void Detail::PtrListDetail<T>::operator=
(
    PtrListDetail<T>&& list
)
{
    List<T*>::transfer(list);
}


// ************************************************************************* //

 } // End namespace Foam
