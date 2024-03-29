﻿/*---------------------------------------------------------------------------*\
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

Class
    Foam::IndirectListAddressing

Description
    A class for storing list addressing (labels, slices etc), which are
    normally to used by IndirectList.
    Private inheritance is often used by any inheriting classes.

\*---------------------------------------------------------------------------*/

#ifndef IndirectListAddressing_H
#define IndirectListAddressing_H

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                   Class IndirectListAddressing Declaration
\*---------------------------------------------------------------------------*/

template<class Addr>
class IndirectListAddressing
{
    // Private Data

        //- Storage for addressing
        Addr storedAddr_;


public:

    // Constructors

        //- Copy construct from addressing information
        IndirectListAddressing(const Addr& addr)
        :
            storedAddr_(addr)
        {}

        //- Move construct from addressing information
        IndirectListAddressing(Addr&& addr)
        :
            storedAddr_(std::move(addr))
        {}


    // Member Functions

        //- Const access to the addressing
        const Addr& addressing() const noexcept
        {
            return storedAddr_;
        }

        //- Non-const access to the addressing
        Addr& addressing() noexcept
        {
            return storedAddr_;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
