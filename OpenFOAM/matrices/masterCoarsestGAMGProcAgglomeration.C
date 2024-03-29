﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2013-2014 OpenFOAM Foundation
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

#include "masterCoarsestGAMGProcAgglomeration.H"
#include "addToRunTimeSelectionTable.H"
#include "GAMGAgglomeration.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(masterCoarsestGAMGProcAgglomeration, 0);

    addToRunTimeSelectionTable
    (
        GAMGProcAgglomeration,
        masterCoarsestGAMGProcAgglomeration,
        GAMGAgglomeration
    );



    // * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

    masterCoarsestGAMGProcAgglomeration::masterCoarsestGAMGProcAgglomeration
    (
        GAMGAgglomeration& agglom,
        const dictionary& controlDict
    )
        :
        GAMGProcAgglomeration(agglom, controlDict)
    {}


    // * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

    masterCoarsestGAMGProcAgglomeration::
        ~masterCoarsestGAMGProcAgglomeration()
    {
        forAllReverse(comms_, i)
        {
            if (comms_[i] != -1)
            {
                UPstream::freeCommunicator(comms_[i]);
            }
        }
    }


    // * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

    bool masterCoarsestGAMGProcAgglomeration::agglomerate()
    {
        if (debug)
        {
            Pout << nl << "Starting mesh overview" << endl;
            printStats(Pout, agglom_);
        }

        if (agglom_.size() >= 1)
        {
            // Agglomerate one but last level (since also agglomerating
            // restrictAddressing)
            label fineLevelIndex = agglom_.size() - 1;

            if (agglom_.hasMeshLevel(fineLevelIndex))
            {
                // Get the fine mesh
                const lduMesh& levelMesh = agglom_.meshLevel(fineLevelIndex);
                label levelComm = levelMesh.comm();
                label nProcs = UPstream::nProcs(levelComm);

                if (nProcs > 1)
                {
                    // Processor restriction map: per processor the coarse processor
                    labelList procAgglomMap(nProcs, Zero);

                    // Master processor
                    labelList masterProcs;
                    // Local processors that agglomerate. agglomProcIDs[0] is in
                    // masterProc.
                    List<label> agglomProcIDs;
                    GAMGAgglomeration::calculateRegionMaster
                    (
                        levelComm,
                        procAgglomMap,
                        masterProcs,
                        agglomProcIDs
                    );

                    // Allocate a communicator for the processor-agglomerated matrix
                    comms_.append
                    (
                        UPstream::allocateCommunicator
                        (
                            levelComm,
                            masterProcs
                        )
                    );

                    // Use processor agglomeration maps to do the actual collecting.
                    if (Pstream::myProcNo(levelComm) != -1)
                    {
                        GAMGProcAgglomeration::agglomerate
                        (
                            fineLevelIndex,
                            procAgglomMap,
                            masterProcs,
                            agglomProcIDs,
                            comms_.last()
                        );
                    }
                }
            }
        }

        // Print a bit
        if (debug)
        {
            Pout << nl << "Agglomerated mesh overview" << endl;
            printStats(Pout, agglom_);
        }

        return true;
    }

}
// ************************************************************************* //
