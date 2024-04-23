/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2018-2021 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM, distributed under GPL-3.0-or-later.

Description
    Create a fvMesh (specified region or defaultRegion) with
    additional handling of -dry-run and -dry-run-write options.

Required Variables
    - args [argList]
    - runTime [Time]

Provided Variables
    - regionName [word]
    - mesh [fvMesh]
    - meshPtr [autoPtr<fvMesh>]

\*---------------------------------------------------------------------------*/



autoPtr<fvMesh> meshPtr(nullptr);
word regionName(polyMesh::defaultRegion);

if (args.dryRun() || args.found("dry-run-write"))
{
    Info
        << "Operating in 'dry-run' mode: case will run for 1 time step.  "
        << "All checks assumed OK on a clean exit" << endl;

    // Allow region in combination with dry-run
    args.readIfPresent("region", regionName);

    FieldBase::allowConstructFromLargerSize = true;

    // Create a simplified 1D mesh and attempt to re-create boundary conditions
    meshPtr.reset
    (
        new simplifiedMeshes::columnFvMesh(runTime, regionName)
    );

    // Stop after 1 iteration of the simplified mesh

    if (args.found("dry-run-write"))
    {
        // Using saWriteNow triggers function objects execute(), write()
        runTime.stopAt(Time::saWriteNow);

        // Make sure mesh gets output to the current time (since instance
        // no longer constant)
        meshPtr().setInstance(runTime.timeName());
    }
    else
    {
        // Using saNoWriteNow triggers function objects execute(),
        // but not write()
        runTime.stopAt(Time::saNoWriteNow);
    }

    functionObject::outputPrefix = "postProcessing-dry-run";
}
else
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

    meshPtr.reset
    (
        new fvMesh
        (
            IOobject
            (
                regionName,
                runTime.timeName(),
                runTime,
                IOobject::MUST_READ
            ),
            false
        )
    );
    meshPtr().init(true);   // initialise all (lower levels and current)

    Info << endl;
}

fvMesh& mesh = meshPtr();


// ************************************************************************* //


