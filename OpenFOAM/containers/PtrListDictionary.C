﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2015 OpenFOAM Foundation
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

//#include "PtrListDictionary.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


 namespace Foam{
template<class T>
PtrListDictionary<T>::PtrListDictionary(const label size)
:
    DictionaryBase<PtrList<T>, T>(2*size)
{
    PtrList<T>::resize(size);
}


template<class T>
PtrListDictionary<T>::PtrListDictionary(const PtrListDictionary& dict)
:
    DictionaryBase<PtrList<T>, T>(dict)
{}


template<class T>
template<class INew>
PtrListDictionary<T>::PtrListDictionary(Istream& is, const INew& iNew)
:
    DictionaryBase<PtrList<T>, T>(is, iNew)
{}


template<class T>
PtrListDictionary<T>::PtrListDictionary(Istream& is)
:
    DictionaryBase<PtrList<T>, T>(is)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T>
inline autoPtr<T> PtrListDictionary<T>::set
(
    const label i,
    const word& key,
    T* ptr
)
{
    if (!DictionaryBase<PtrList<T>, T>::hashedTs_.insert(key, ptr))
    {
        FatalErrorInFunction
            << "Cannot insert with key '" << key << "' into hash-table"
            << abort(FatalError);
    }
    return PtrList<T>::set(i, ptr);
}


template<class T>
inline autoPtr<T> PtrListDictionary<T>::set
(
    const label i,
    const word& key,
    autoPtr<T>& aptr
)
{
    T* ptr = aptr.ptr();
    if (!DictionaryBase<PtrList<T>, T>::hashedTs_.insert(key, ptr))
    {
        FatalErrorInFunction
            << "Cannot insert with key '" << key << "' into hash-table"
            << abort(FatalError);
    }
    return PtrList<T>::set(i, ptr);
}


template<class T>
inline autoPtr<T> PtrListDictionary<T>::set
(
    const label i,
    const word& key,
    tmp<T>& t
)
{
    T* ptr = t.ptr();
    if (!DictionaryBase<PtrList<T>, T>::hashedTs_.insert(key, ptr))
    {
        FatalErrorInFunction
            << "Cannot insert with key '" << key << "' into hash-table"
            << abort(FatalError);
    }
    return PtrList<T>::set(i, ptr);
}


// ************************************************************************* //

 } // End namespace Foam
