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
    Create a fvMesh for a specified region, or the defaultRegion

Required Variables
    - args [argList]
    - runTime [Time]

Provided Variables
    - regionName [word]
    - mesh [fvMesh]

\*---------------------------------------------------------------------------*/


 
word regionName(polyMesh::defaultRegion);

{
    if (args.readIfPresent("region", regionName))
    {
        Info << "Create mesh " << regionName;
    }
    else
    {
        Info << "Create mesh";
    }
    Info << " for time = " << runTime.timeName() << nl;
}


fvMesh mesh
(
    IOobject
    (
        regionName,
        runTime.timeName(),
        runTime,
        IOobject::MUST_READ
    ),
    false
);

mesh.init(true);   // Initialise all (lower levels and current)

Info << endl;


// ************************************************************************* //

 
