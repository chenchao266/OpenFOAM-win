/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2019-2021 OpenCFD Ltd.
------------------------------------------------------------------------------
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

#include "faOptions.H"
#include "faMesh.H"
#include "Time1.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    namespace fa
    {
        defineTypeNameAndDebug(options, 0);
    }
}


// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

Foam::IOobject Foam::fa::options::createIOobject
(
    const fvMesh& mesh
) const
{
    IOobject io
    (
        typeName,
        mesh.time().constant(),
        mesh,
        IOobject::MUST_READ,
        IOobject::NO_WRITE
    );

    if (io.typeHeaderOk<IOdictionary>(true))
    {
        Info<< "Creating finite area options from "
            << io.instance()/io.name() << nl
            << endl;

        io.readOpt(IOobject::MUST_READ_IF_MODIFIED);
    }
    else
    {
        // Check if the faOptions file is in system
        io.instance() = mesh.time().system();

        if (io.typeHeaderOk<IOdictionary>(true))
        {
            Info<< "Creating finite area options from "
                << io.instance()/io.name() << nl
                << endl;

            io.readOpt(IOobject::MUST_READ_IF_MODIFIED);
        }
        else
        {
            io.readOpt(IOobject::NO_READ);
        }
    }

    return io;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::fa::options::options
(
    const fvPatch& p
)
:
    IOdictionary(createIOobject(p.boundaryMesh().mesh())),
    optionList(p, *this)
{}


Foam::fa::options& Foam::fa::options::New(const fvPatch& p)
{
    const fvMesh& mesh = p.boundaryMesh().mesh();

    if (mesh.thisDb().foundObject<options>(typeName))
    {
        return const_cast<options&>
        (
            mesh.lookupObject<options>(typeName)
        );
    }
    else
    {
        if (debug)
        {
            InfoInFunction
                << "Constructing " << typeName
                << " for region " << mesh.name() << endl;
        }

        options* objectPtr = new options(p);
        regIOobject::store(objectPtr);
        return *objectPtr;
    }
}


bool Foam::fa::options::read()
{
    if (IOdictionary::regIOobject::read())
    {
        optionList::read(*this);
        return true;
    }
    else
    {
        return false;
    }
}


// ************************************************************************* //
