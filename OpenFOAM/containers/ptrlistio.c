﻿/*---------------------------------------------------------------------------*\
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

//#include "PtrList.H"
#include "SLList.H"
#include "_Istream.H"
#include "INew.H"

// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

 namespace Foam{
template<class T>

template<class INew>
void PtrList<T>::readIstream(Istream& is, const INew& inew)
{
    clear();  // Delete old pointers and reset the list size

    is.fatalCheck(FUNCTION_NAME);

    token tok(is);

    is.fatalCheck("PtrList::readIstream : reading first token");

    if (tok.isLabel())
    {
        // Label: could be int(..), int{...} or just a plain '0'

        // Read size of list
        const label len = tok.labelToken();

        // Set list length to that read
        resize(len);

        // Read beginning of contents
        const char delimiter = is.readBeginList("PtrList");

        if (len)
        {
            if (delimiter == token::BEGIN_LIST)
            {
                for (label i=0; i<len; ++i)
                {
                    T* p = inew(is).ptr();
                    set(i, p);

                    is.fatalCheck
                    (
                        "PtrList::readIstream : "
                        "reading entry"
                    );
                }
            }
            else  // Assumed to be BEGIN_BLOCK
            {
                T* p = inew(is).ptr();
                set(0, p);

                is.fatalCheck
                (
                    "PtrList::readIstream : "
                    "reading the single entry"
                );

                for (label i=1; i<len; ++i)
                {
                    set(i, p->clone());
                }
            }
        }

        // Read end of contents
        is.readEndList("PtrList");
    }
    else if (tok.isPunctuation(token::BEGIN_LIST))
    {
        // "(...)" : read as SLList and transfer contents
        // This would be more efficient (fewer allocations, lower overhead)
        // using a DynamicList, but then we have circular dependencies

        SLList<T*> slList;

        is >> tok;
        while (!tok.isPunctuation(token::END_LIST))
        {
            is.putBack(tok);

            if (is.eof())
            {
                FatalIOErrorInFunction(is)
                    << "Premature EOF after reading " << tok.info() << nl
                    << exit(FatalIOError);
            }

            slList.append(inew(is).ptr());
            is >> tok;
        }

        resize(slList.size());

        // A list of pointers - can simply shallow copy them
        label i = 0;
        for (T* ptr : slList)
        {
            set(i++, ptr);
        }
    }
    else
    {
        FatalIOErrorInFunction(is)
            << "incorrect first token, expected <int> or '(', found "
            << tok.info() << nl
            << exit(FatalIOError);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class T>
template<class INew>
PtrList<T>::PtrList(Istream& is, const INew& inew)
{
    this->readIstream(is, inew);
}


template<class T>
PtrList<T>::PtrList(Istream& is)
{
    this->readIstream(is, INew<T>());
}


// * * * * * * * * * * * * * * * Istream Operator  * * * * * * * * * * * * * //

template<class T>
Istream& operator>>(Istream& is, PtrList<T>& list)
{
    list.readIstream(is, INew<T>());
    return is;
}


// ************************************************************************* //

 } // End namespace Foam
