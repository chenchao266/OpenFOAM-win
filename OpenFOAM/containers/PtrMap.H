﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2017-2018 OpenCFD Ltd.
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

Class
    Foam::PtrMap

Description
    A HashTable of pointers to objects of type \<T\> with a label key.

See also
    Map, HashPtrTable

\*---------------------------------------------------------------------------*/

#ifndef PtrMap_H
#define PtrMap_H

#include "HashPtrTable.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class PtrMap Declaration
\*---------------------------------------------------------------------------*/

template<class T>
class PtrMap
:
    public HashPtrTable<T, label, Hash<label>>
{
public:

    //- The template instance used for this PtrMap
    typedef PtrMap<T> this_type;

    //- The template instance used for the parent HashTable
    typedef HashPtrTable<T, label, Hash<label>> parent_type;


    // Constructors

        //- Default construct with default table capacity
        PtrMap()
        :
            parent_type()
        {}

        //- Construct with given initial table capacity
        explicit PtrMap(const label size)
        :
            parent_type(size)
        {}

        //- Construct from Istream
        PtrMap(Istream& is)
        :
            parent_type(is)
        {}

        //- Copy construct
        PtrMap(const this_type& map)
        :
            parent_type(map)
        {}

        //- Move construct
        PtrMap(this_type&& map)
        :
            parent_type(std::move(map))
        {}


    // Member Operators

        //- Copy assignment
        void operator=(const this_type& rhs)
        {
            parent_type::operator=(rhs);
        }

        //- Move assignment
        void operator=(this_type&& rhs)
        {
            parent_type::operator=(std::move(rhs));
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
