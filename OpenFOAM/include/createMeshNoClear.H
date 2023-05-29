/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011 OpenFOAM Foundation
    Copyright (C) 2021 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM, distributed under GPL-3.0-or-later.

Description
    Create a fvMesh (defaultRegion)

Required Variables
    - runTime [Time]

Provided Variables
    - mesh [fvMesh]

Notes
    Currently identical to non-dry-run version of createMesh.H

\*---------------------------------------------------------------------------*/


 namespace Foam{
Info
    << "Create mesh, no clear-out for time = "
    << runTime.timeName() << nl << endl;

fvMesh mesh
(
    IOobject
    (
        polyMesh::defaultRegion,
        runTime.timeName(),
        runTime,
        IOobject::MUST_READ
    )
);


// ************************************************************************* //

 } // End namespace Foam
