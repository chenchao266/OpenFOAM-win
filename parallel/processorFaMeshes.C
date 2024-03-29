/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016-2017 Wikki Ltd
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

#include "processorFaMeshes.H"
#include "Time1.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::processorFaMeshes::read()
{
    forAll(fvMeshes_, procI)
    {
        meshes_.set
        (
            procI,
            new faMesh(fvMeshes_[procI])
        );

        pointProcAddressing_.set
        (
            procI,
            new labelIOList
            (
                IOobject
                (
                    "pointProcAddressing",
                    meshes_[procI].time().findInstance
                    (
                        meshes_[procI].meshDir(),
                        "pointProcAddressing"
                    ),
                    meshes_[procI].meshSubDir,
                    fvMeshes_[procI],
                    IOobject::MUST_READ,
                    IOobject::NO_WRITE
                )
            )
        );

        edgeProcAddressing_.set
        (
            procI,
            new labelIOList
            (
                IOobject
                (
                    "edgeProcAddressing",
                    meshes_[procI].time().findInstance
                    (
                        meshes_[procI].meshDir(),
                        "edgeProcAddressing"
                    ),
                    meshes_[procI].meshSubDir,
                    fvMeshes_[procI],
                    IOobject::MUST_READ,
                    IOobject::NO_WRITE
                )
            )
        );

        faceProcAddressing_.set
        (
            procI,
            new labelIOList
            (
                IOobject
                (
                    "faceProcAddressing",
                    meshes_[procI].time().findInstance
                    (
                        meshes_[procI].meshDir(),
                        "faceProcAddressing"
                    ),
                    meshes_[procI].meshSubDir,
                    fvMeshes_[procI],
                    IOobject::MUST_READ,
                    IOobject::NO_WRITE
                )
            )
        );

        boundaryProcAddressing_.set
        (
            procI,
            new labelIOList
            (
                IOobject
                (
                    "boundaryProcAddressing",
                    meshes_[procI].time().findInstance
                    (
                        meshes_[procI].meshDir(),
                        "faceProcAddressing"
                    ),
                    meshes_[procI].meshSubDir,
                    fvMeshes_[procI],
                    IOobject::MUST_READ,
                    IOobject::NO_WRITE
                )
            )
        );
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::processorFaMeshes::processorFaMeshes
(
    const UPtrList<fvMesh>& processorFvMeshes
)
:
    fvMeshes_(processorFvMeshes),
    meshes_(processorFvMeshes.size()),
    pointProcAddressing_(meshes_.size()),
    edgeProcAddressing_(meshes_.size()),
    faceProcAddressing_(meshes_.size()),
    boundaryProcAddressing_(meshes_.size())
{
    read();
}


// ************************************************************************* //
