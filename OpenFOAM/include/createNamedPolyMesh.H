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
    Create a polyMesh for a specified region, or the defaultRegion

Required Variables
    - runTime [Time]

Provided Variables
    - regionName [word]
    - mesh [polyMesh]

\*---------------------------------------------------------------------------*/


 
word regionName(polyMesh::defaultRegion);

{
    if (args.readIfPresent("region", regionName))
    {
        Info << "Create polyMesh " << regionName;
    }
    else
    {
        Info << "Create polyMesh";
    }
    Info << " for time = " << runTime.timeName() << nl;
}


polyMesh mesh
(
    IOobject
    (
        regionName,
        runTime.timeName(),
        runTime,
        IOobject::MUST_READ
    )
);

Info << endl;

// ************************************************************************* //

 
