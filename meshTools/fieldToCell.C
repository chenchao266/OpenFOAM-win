/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2017 OpenFOAM Foundation
     \\/     M anipulation  |
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

#include "fieldToCell.H"
#include "polyMesh.H"
#include "cellSet.H"
#include "Time.T.H"
#include "IFstream.H"
#include "fieldDictionary.H"

#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(fieldToCell, 0);
    addToRunTimeSelectionTable(topoSetSource, fieldToCell, word);
    addToRunTimeSelectionTable(topoSetSource, fieldToCell, istream);
}


Foam::topoSetSource::addToUsageTable Foam::fieldToCell::usage_
(
    fieldToCell::typeName,
    "\n    Usage: fieldToCell field min max\n\n"
    "    Select all cells with field value >= min and <= max\n\n"
);


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::fieldToCell::applyToSet
(
    const topoSetSource::setAction action,
    const scalarField& field,
    topoSet& set
) const
{
    Info<< "    Field min:" << min(field)
        << " max:" << max(field) << endl;

    if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
    {
        Info<< "    Adding all cells with value of field " << fieldName_
            << " within range " << min_ << ".." << max_ << endl;

        forAll(field, celli)
        {
            if (field[celli] >= min_ && field[celli] <= max_)
            {
                set.insert(celli);
            }
        }
    }
    else if (action == topoSetSource::DELETE)
    {
        Info<< "    Removing all cells with value of field " << fieldName_
            << " within range " << min_ << ".." << max_ << endl;

        forAll(field, celli)
        {
            if (field[celli] >= min_ && field[celli] <= max_)
            {
                set.erase(celli);
            }
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
Foam::fieldToCell::fieldToCell
(
    const polyMesh& mesh,
    const word& fieldName,
    const scalar min,
    const scalar max
)
:
    topoSetSource(mesh),
    fieldName_(fieldName),
    min_(min),
    max_(max)
{}


// Construct from dictionary
Foam::fieldToCell::fieldToCell
(
    const polyMesh& mesh,
    const dictionary& dict
)
:
    topoSetSource(mesh),
    fieldName_(dict.lookup("field")),
    min_(readScalar(dict.lookup("min"))),
    max_(readScalar(dict.lookup("max")))
{}


// Construct from Istream
Foam::fieldToCell::fieldToCell
(
    const polyMesh& mesh,
    Istream& is
)
:
    topoSetSource(mesh),
    fieldName_(checkIs(is)),
    min_(readScalar(checkIs(is))),
    max_(readScalar(checkIs(is)))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::fieldToCell::~fieldToCell()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::fieldToCell::applyToSet
(
    const topoSetSource::setAction action,
    topoSet& set
) const
{

//    // Construct temporary fvMesh from polyMesh
//    fvMesh fMesh
//    (
//        mesh(), // IOobject
//        mesh().points(),
//        mesh().faces(),
//        mesh().cells()
//    );
//
//    const polyBoundaryMesh& patches = mesh().boundaryMesh();
//
//    List<polyPatch*> newPatches(patches.size());
//    forAll(patches, patchi)
//    {
//        const polyPatch& pp = patches[patchi];
//
//        newPatches[patchi] =
//            patches[patchi].clone
//            (
//                fMesh.boundaryMesh(),
//                patchi,
//                pp.size(),
//                pp.start()
//            ).ptr();
//    }
//    fMesh.addFvPatches(newPatches);

    // Try to load field
    IOobject fieldObject
    (
        fieldName_,
        mesh().time().timeName(),
        mesh(),
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE,
        false
    );

    // Note: should check for volScalarField but that introduces depencendy
    //       on volMesh so just use another type with processor-local scope
    if (!fieldObject.typeHeaderOk<labelIOList>(false))
    {
        WarningInFunction
            << "Cannot read field " << fieldName_
            << " from time " << mesh().time().timeName() << endl;
    }
    else if (fieldObject.headerClassName() == "volScalarField")
    {
        IFstream str(typeFilePath<labelIOList>(fieldObject));

        // Read dictionary
        fieldDictionary fieldDict(fieldObject, fieldObject.headerClassName());

        scalarField internalVals("internalField", fieldDict, mesh().nCells());

        applyToSet(action, internalVals, set);
    }
    else if (fieldObject.headerClassName() == "volVectorField")
    {
        IFstream str(typeFilePath<labelIOList>(fieldObject));

        // Read dictionary
        fieldDictionary fieldDict(fieldObject, fieldObject.headerClassName());

        vectorField internalVals("internalField", fieldDict, mesh().nCells());

        applyToSet(action, mag(internalVals), set);
    }
    else
    {
        WarningInFunction
            << "Cannot handle fields of type " << fieldObject.headerClassName()
            << endl;
    }
}


// ************************************************************************* //
