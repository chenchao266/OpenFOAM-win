﻿/*---------------------------------------------------------------------------*\
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

//#include "fieldCoordinateSystemTransform.H"
#include "volFields.H"
#include "surfaceFields.H"
#include "transformGeometricField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class FieldType>
void Foam::functionObjects::fieldCoordinateSystemTransform::transformField
(
    const FieldType& field
)
{
    word transFieldName(transformFieldName(field.name()));

    store
    (
        transFieldName,
        Foam::transform(dimensionedTensor(coordSys_.R().R()), field)
    );
}


template<class Type>
void Foam::functionObjects::fieldCoordinateSystemTransform::transform
(
    const word& fieldName
)
{
    typedef volFieldType<Type> VolFieldType;
    typedef surfaceFieldType<Type> SurfaceFieldType;

    if (mesh_.foundObject<VolFieldType>(fieldName))
    {
        Foam_DebugInfo
            << type() << ": Field " << fieldName << " already in database"
            << endl;

        transformField<VolFieldType>
        (
            mesh_.lookupObject<VolFieldType>(fieldName)
        );
    }
    else if (mesh_.foundObject<SurfaceFieldType>(fieldName))
    {
        Foam_DebugInfo
            << type() << ": Field " << fieldName << " already in database"
            << endl;

        transformField<SurfaceFieldType>
        (
            mesh_.lookupObject<SurfaceFieldType>(fieldName)
        );
    }
    else
    {
        IOobject fieldHeader
        (
            fieldName,
            mesh_.time().timeName(),
            mesh_,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        );

        if
        (
            fieldHeader.typeHeaderOk<VolFieldType>(false)
         && fieldHeader.headerClassName() == VolFieldType::typeName
        )
        {
            Foam_DebugInfo
                << type() << ": Field " << fieldName << " read from file"
                << endl;

            transformField<VolFieldType>
            (
                mesh_.lookupObject<VolFieldType>(fieldName)
            );
        }
        else if
        (
            fieldHeader.typeHeaderOk<SurfaceFieldType>(false)
         && fieldHeader.headerClassName() == SurfaceFieldType::typeName
        )
        {
            Foam_DebugInfo
                << type() << ": Field " << fieldName << " read from file"
                << endl;

            transformField<SurfaceFieldType>
            (
                mesh_.lookupObject<SurfaceFieldType>(fieldName)
            );
        }
    }
}


// ************************************************************************* //
