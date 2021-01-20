/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2013-2016 OpenFOAM Foundation
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

//#include "fvcCellReduce.H"
#include "fvMesh.H"
#include "volFields.H"
#include "surfaceFields.H"
#include "extrapolatedCalculatedFvPatchFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
namespace Foam {
    template<class Type, class CombineOp>
    tmp<volFieldType<Type>>
        fvc::cellReduce
        (
            const surfaceFieldType<Type>& ssf,
            const CombineOp& cop,
            const Type& nullValue
        )
    {
        typedef volFieldType<Type> VolFieldType;

        const fvMesh& mesh = ssf.mesh();

        tmp<VolFieldType> tresult
        (
            new VolFieldType
            (
                IOobject
                (
                    "cellReduce(" + ssf.name() + ')',
                    ssf.instance(),
                    mesh,
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                mesh,
                dimensioned<Type>("initialValue", ssf.dimensions(), nullValue),
                extrapolatedCalculatedFvPatchField<Type>::typeName
            )
        );

        volFieldType& result = tresult.ref();

        const labelUList& own = mesh.owner();
        const labelUList& nbr = mesh.neighbour();

        forAll(own, i)
        {
            label celli = own[i];
            cop(result[celli], ssf[i]);
        }
        forAll(nbr, i)
        {
            label celli = nbr[i];
            cop(result[celli], ssf[i]);
        }

        result.correctBoundaryConditions();

        return tresult;
    }


    template<class Type, class CombineOp>
    tmp<volFieldType<Type>>
        fvc::cellReduce
        (
            const tmp<surfaceFieldType<Type>>& tssf,
            const CombineOp& cop,
            const Type& nullValue
        )
    {
        tmp<volFieldType<Type>> tvf
        (
            cellReduce
            (
                tssf,
                cop,
                nullValue
            )
        );

        tssf.clear();

        return tvf;
    }

}
// ************************************************************************* //
