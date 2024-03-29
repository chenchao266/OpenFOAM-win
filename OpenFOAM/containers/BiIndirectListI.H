﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2019 OpenCFD Ltd.
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

// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //


 namespace Foam{
template<class T>
inline label BiIndirectList<T>::posIndex(const label i)
{
    return i;
}


template<class T>
inline label BiIndirectList<T>::negIndex(const label i)
{
    return -i-1;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class T>
inline BiIndirectList<T>::BiIndirectList
(
    const UList<T>& posList,
    const UList<T>& negList,
    const labelUList& addr
)
:
    posList_(const_cast<UList<T>&>(posList)),
    negList_(const_cast<UList<T>&>(negList)),
    addressing_(addr)
{}


template<class T>
inline BiIndirectList<T>::BiIndirectList
(
    const UList<T>& posList,
    const UList<T>& negList,
    labelList&& addr
)
:
    posList_(const_cast<UList<T>&>(posList)),
    negList_(const_cast<UList<T>&>(negList)),
    addressing_(std::move(addr))
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T>
inline label BiIndirectList<T>::size() const noexcept
{
    return addressing_.size();
}


template<class T>
inline bool BiIndirectList<T>::empty() const noexcept
{
    return addressing_.empty();
}


template<class T>
inline const UList<T>& BiIndirectList<T>::posList() const noexcept
{
    return posList_;
}


template<class T>
inline const UList<T>& BiIndirectList<T>::negList() const noexcept
{
    return negList_;
}


template<class T>
inline const labelList&
BiIndirectList<T>::addressing() const noexcept
{
    return addressing_;
}


template<class T>
inline void BiIndirectList<T>::resetAddressing
(
    const labelUList& addr
)
{
    addressing_ = addr;
}


template<class T>
inline void BiIndirectList<T>::resetAddressing
(
    labelList&& addr
)
{
    addressing_.transfer(addr);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class T>
inline List<T> BiIndirectList<T>::operator()() const
{
    List<T> result(size());

    forAll(*this, i)
    {
        result[i] = operator[](i);
    }

    return result;
}


template<class T>
inline T& BiIndirectList<T>::operator[](const label i)
{
    const label index = addressing_[i];

    if (index >= 0)
    {
        return posList_[index];
    }
    else
    {
        return negList_[-index-1];
    }
}


template<class T>
inline const T& BiIndirectList<T>::operator[](const label i) const
{
    const label index = addressing_[i];

    if (index >= 0)
    {
        return posList_[index];
    }
    else
    {
        return negList_[-index-1];
    }
}


template<class T>
inline void BiIndirectList<T>::operator=(const UList<T>& ae)
{
    if (addressing_.size() != ae.size())
    {
        FatalErrorInFunction
            << "Addressing and list of addressed elements "
               "have different sizes: "
            << addressing_.size() << " " << ae.size()
            << abort(FatalError);
    }

    forAll(addressing_, i)
    {
        operator[](i) = ae[i];
    }
}


template<class T>
inline void BiIndirectList<T>::operator=(const T& val)
{
    forAll(addressing_, i)
    {
        operator[](i) = val;
    }
}


// ************************************************************************* //

 } // End namespace Foam
