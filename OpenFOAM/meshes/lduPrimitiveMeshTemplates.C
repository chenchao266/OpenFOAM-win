﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2013 OpenFOAM Foundation
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

#include "lduPrimitiveMesh.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


 namespace Foam{
template<class ProcPatch>
lduSchedule lduPrimitiveMesh::nonBlockingSchedule
(
    const lduInterfacePtrsList& interfaces
)
{
    lduSchedule schedule(2*interfaces.size());
    label slotI = 0;

    forAll(interfaces, i)
    {
        if (interfaces.set(i) && !isA<ProcPatch>(interfaces[i]))
        {
            schedule[slotI].patch = i;
            schedule[slotI].init = true;
            slotI++;
            schedule[slotI].patch = i;
            schedule[slotI].init = false;
            slotI++;
        }
    }

    forAll(interfaces, i)
    {
        if (interfaces.set(i) && isA<ProcPatch>(interfaces[i]))
        {
            schedule[slotI].patch = i;
            schedule[slotI].init = true;
            slotI++;
        }
    }

    forAll(interfaces, i)
    {
        if (interfaces.set(i) && isA<ProcPatch>(interfaces[i]))
        {
            schedule[slotI].patch = i;
            schedule[slotI].init = false;
            slotI++;
        }
    }

    return schedule;
}


// ************************************************************************* //

 } // End namespace Foam
