/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011 OpenFOAM Foundation
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

#include "setUpdater.H"
#include "polyMesh.H"
#include "Time1.H"
#include "mapPolyMesh.H"
#include "IOobjectList.H"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void Foam::setUpdater::updateSets(const mapPolyMesh& morphMap) const
{
    //
    // Update all sets in memory.
    //

    HashTable<const Type*> memSets =
        morphMap.mesh().objectRegistry::lookupClass<Type>();

    forAllIters(memSets, iter)
    {
        Type& set = const_cast<Type&>(*iter());

        DebugPout
            << "Set:" << set.name() << " size:" << set.size()
            << " updated in memory" << endl;

        set.updateMesh(morphMap);

        // Write or not? Debatable.
        set.write();
    }


    //
    // Update all sets on disk
    //

    // Get last valid mesh (discard points-only change)
    IOobjectList objs
    (
        morphMap.mesh().time(),
        morphMap.mesh().facesInstance(),
        "polyMesh/sets"
    );

    IOobjectList fileSets(objs.lookupClass<Type>());

    forAllConstIters(fileSets, iter)
    {
        if (!memSets.found(iter.key()))
        {
            // Not in memory. Load it.
            Type set(*iter());

            if (debug)
            {
                Pout<< "Set:" << set.name() << " size:" << set.size()
                    << " updated on disk" << endl;
            }

            set.updateMesh(morphMap);

            set.write();
        }
        else
        {
            DebugPout
                << "Set:" << iter.key() << " already updated from memory"
                << endl;
        }
    }
}


// ************************************************************************* //
