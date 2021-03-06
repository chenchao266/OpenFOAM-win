﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2016 OpenFOAM Foundation
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

//#include "fvcDdt.H"
#include "fvMesh.H"
#include "ddtScheme.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fvc
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
tmp<volFieldType<Type>>
ddt
(
    const dimensioned<Type> dt,
    const fvMesh& mesh
)
{
    return fv::ddtScheme<Type>::New
    (
        mesh,
        mesh.ddtScheme("ddt(" + dt.name() + ')')
    ).ref().fvcDdt(dt);
}


template<class Type>
tmp<volFieldType<Type>>
ddt
(
    const volFieldType<Type>& vf
)
{
    return fv::ddtScheme<Type>::New
    (
        vf.mesh(),
        vf.mesh().ddtScheme("ddt(" + vf.name() + ')')
    ).ref().fvcDdt(vf);
}


template<class Type>
tmp<volFieldType<Type>>
ddt
(
    const dimensionedScalar& rho,
    const volFieldType<Type>& vf
)
{
    return fv::ddtScheme<Type>::New
    (
        vf.mesh(),
        vf.mesh().ddtScheme("ddt(" + rho.name() + ',' + vf.name() + ')')
    ).ref().fvcDdt(rho, vf);
}


template<class Type>
tmp<volFieldType<Type>>
ddt
(
    const volScalarField& rho,
    const volFieldType<Type>& vf
)
{
    return fv::ddtScheme<Type>::New
    (
        vf.mesh(),
        vf.mesh().ddtScheme("ddt(" + rho.name() + ',' + vf.name() + ')')
    ).ref().fvcDdt(rho, vf);
}


template<class Type>
tmp<volFieldType<Type>>
ddt
(
    const volScalarField& alpha,
    const volScalarField& rho,
    const volFieldType<Type>& vf
)
{
    return fv::ddtScheme<Type>::New
    (
        vf.mesh(),
        vf.mesh().ddtScheme
        (
            "ddt("
          + alpha.name() + ','
          + rho.name() + ','
          + vf.name() + ')'
        )
    ).ref().fvcDdt(alpha, rho, vf);
}


template<class Type>
tmp<surfaceFieldType<Type>>
ddt
(
    const surfaceFieldType<Type>& sf
)
{
    return fv::ddtScheme<Type>::New
    (
        sf.mesh(),
        sf.mesh().ddtScheme("ddt(" + sf.name() + ')')
    ).ref().fvcDdt(sf);
}


template<class Type>
tmp<volFieldType<Type>>
ddt
(
    const one&,
    const volFieldType<Type>& vf
)
{
    return ddt(vf);
}


template<class Type>
tmp<volFieldType<Type>>
ddt
(
    const volFieldType<Type>& vf,
    const one&
)
{
    return ddt(vf);
}


template<class Type>
tmp<GeometricField<typename flux<Type>::type, fvsPatchField, surfaceMesh>>
ddtCorr
(
    const volFieldType<Type>& U,
    const surfaceFieldType<Type>& Uf
)
{
    return fv::ddtScheme<Type>::New
    (
        U.mesh(),
        U.mesh().ddtScheme("ddt(" + U.name() + ')')
    ).ref().fvcDdtUfCorr(U, Uf);
}


template<class Type>
tmp<GeometricField<typename flux<Type>::type, fvsPatchField, surfaceMesh>>
ddtCorr
(
    const volFieldType<Type>& U,
    const GeometricField
    <
        typename flux<Type>::type,
        fvsPatchField,
        surfaceMesh
    >& phi
)
{
    return fv::ddtScheme<Type>::New
    (
        U.mesh(),
        U.mesh().ddtScheme("ddt(" + U.name() + ')')
    ).ref().fvcDdtPhiCorr(U, phi);
}


template<class Type>
tmp<GeometricField<typename flux<Type>::type, fvsPatchField, surfaceMesh>>
ddtCorr
(
    const volScalarField& rho,
    const volFieldType<Type>& U,
    const surfaceFieldType<Type>& Uf
)
{
    return fv::ddtScheme<Type>::New
    (
        U.mesh(),
        U.mesh().ddtScheme("ddt(" + U.name() + ')')
    ).ref().fvcDdtUfCorr(rho, U, Uf);
}


template<class Type>
tmp<GeometricField<typename flux<Type>::type, fvsPatchField, surfaceMesh>>
ddtCorr
(
    const volScalarField& rho,
    const volFieldType<Type>& U,
    const GeometricField
    <
        typename flux<Type>::type,
        fvsPatchField,
        surfaceMesh
    >& phi
)
{
    return fv::ddtScheme<Type>::New
    (
        U.mesh(),
        U.mesh().ddtScheme("ddt(" + rho.name() + ',' + U.name() + ')')
    ).ref().fvcDdtPhiCorr(rho, U, phi);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fvc

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
