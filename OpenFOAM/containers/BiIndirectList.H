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

Class
    Foam::BiIndirectList

Description
    Indexes into negList (negative index) or posList (zero or positive index).

SourceFiles
    BiIndirectListI.H

\*---------------------------------------------------------------------------*/

#ifndef BiIndirectList_H
#define BiIndirectList_H

#include "List.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                       Class BiIndirectList Declaration
\*---------------------------------------------------------------------------*/

template<class T>
class BiIndirectList
{
    // Private Data

        UList<T>& posList_;
        UList<T>& negList_;

        labelList addressing_;


public:

    // Constructors

        //- Construct given the complete lists and the addressing array
        inline BiIndirectList
        (
            const UList<T>& posList,
            const UList<T>& negList,
            const labelUList& addr
        );

        //- Construct given the complete list and by transferring addressing
        inline BiIndirectList
        (
            const UList<T>& posList,
            const UList<T>& negList,
            labelList&& addr
        );


    // Member Functions

        // Access

            //- The number of elements in the list
            inline label size() const noexcept;

            //- True if the list is empty (ie, size() is zero).
            inline bool empty() const noexcept;

            inline const UList<T>& posList() const noexcept;
            inline const UList<T>& negList() const noexcept;

            //- Return the list addressing
            inline const labelList& addressing() const noexcept;

            //- Calculate index given whether index is into posList or negList
            inline static label posIndex(const label i);
            inline static label negIndex(const label i);


        // Edit

            //- Copy reset addressing
            inline void resetAddressing(const labelUList& addr);

            //- Move reset addressing
            inline void resetAddressing(labelList&& addr);


        // Member Operators

            //- Return the addressed elements as a List
            inline List<T> operator()() const;

            //- Return non-const access to an element
            inline T& operator[](const label i);

            //- Return const access to an element
            inline const T& operator[](const label i) const;

            //- Assignment to UList of addressed elements
            inline void operator=(const UList<T>& ae);

            //- Assignment of all entries to the given value
            inline void operator=(const T& val);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "BiIndirectListI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
