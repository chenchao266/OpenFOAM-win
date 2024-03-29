﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011 OpenFOAM Foundation
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

//#include "LPtrList.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


 namespace Foam{
template<class LListBase, class T>
LPtrList<LListBase, T>::LPtrList(const LPtrList<LListBase, T>& lst)
:
    LList<LListBase, T*>()
{
    for (auto iter = lst.cbegin(); iter != lst.cend(); ++iter)
    {
        this->append((*iter).clone().ptr());
    }
}


template<class LListBase, class T>
LPtrList<LListBase, T>::LPtrList(LPtrList<LListBase, T>&& lst)
:
    LList<LListBase, T*>()
{
    LList<LListBase, T*>::transfer(lst);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class LListBase, class T>
LPtrList<LListBase, T>::~LPtrList()
{
    this->clear();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class LListBase, class T>
bool LPtrList<LListBase, T>::eraseHead()
{
    T* p = this->removeHead();

    if (p)
    {
        delete p;
        return true;
    }

    return false;
}


template<class LListBase, class T>
void LPtrList<LListBase, T>::clear()
{
    const label len = this->size();
    for (label i=0; i<len; ++i)
    {
        eraseHead();
    }

    LList<LListBase, T*>::clear();
}


template<class LListBase, class T>
void LPtrList<LListBase, T>::transfer(LPtrList<LListBase, T>& lst)
{
    clear();
    LList<LListBase, T*>::transfer(lst);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class LListBase, class T>
void LPtrList<LListBase, T>::operator=(const LPtrList<LListBase, T>& lst)
{
    clear();

    for (auto iter = lst.cbegin(); iter != lst.cend(); ++iter)
    {
        this->append((*iter).clone().ptr());
    }
}


template<class LListBase, class T>
void LPtrList<LListBase, T>::operator=(LPtrList<LListBase, T>&& lst)
{
    transfer(lst);
}


// ************************************************************************* //

 } // End namespace Foam
