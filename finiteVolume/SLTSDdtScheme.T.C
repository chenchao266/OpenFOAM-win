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

//#include "SLTSDdtScheme.T.H"
#include "surfaceInterpolate.H"
#include "fvMatrices.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fv
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
void SLTSDdtScheme<Type>::relaxedDiag
(
    scalarField& rD,
    const surfaceScalarField& phi
) const
{
    const labelUList& owner = mesh().owner();
    const labelUList& neighbour = mesh().neighbour();
    scalarField diag(rD.size(), 0.0);

    forAll(owner, facei)
    {
        if (phi[facei] > 0.0)
        {
            diag[owner[facei]] += phi[facei];
            rD[neighbour[facei]] += phi[facei];
        }
        else
        {
            diag[neighbour[facei]] -= phi[facei];
            rD[owner[facei]] -= phi[facei];
        }
    }

    forAll(phi.boundaryField(), patchi)
    {
        const fvsPatchScalarField& pphi = phi.boundaryField()[patchi];
        const labelUList& faceCells = pphi.patch().patch().faceCells();

        forAll(pphi, patchFacei)
        {
            if (pphi[patchFacei] > 0.0)
            {
                diag[faceCells[patchFacei]] += pphi[patchFacei];
            }
            else
            {
                rD[faceCells[patchFacei]] -= pphi[patchFacei];
            }
        }
    }

    rD += (1.0/alpha_ - 2.0)*diag;
}


template<class Type>
tmp<volScalarField> SLTSDdtScheme<Type>::SLrDeltaT() const
{
    const surfaceScalarField& phi =
        mesh().objectRegistry::template
            lookupObject<surfaceScalarField>(phiName_);

    const dimensionedScalar& deltaT = mesh().time().deltaT();

    tmp<volScalarField> trDeltaT
    (
        new volScalarField
        (
            IOobject
            (
                "rDeltaT",
                phi.instance(),
                mesh()
            ),
            mesh(),
            dimensionedScalar("rDeltaT", dimless/dimTime, 0.0),
            extrapolatedCalculatedFvPatchScalarField::typeName
        )
    );

    volScalarField& rDeltaT = trDeltaT.ref();

    relaxedDiag(rDeltaT, phi);

    if (phi.dimensions() == dimensionSet(0, 3, -1, 0, 0))
    {
        rDeltaT.primitiveFieldRef() = max
        (
            rDeltaT.primitiveField()/mesh().V(),
            scalar(1)/deltaT.value()
        );
    }
    else if (phi.dimensions() == dimensionSet(1, 0, -1, 0, 0))
    {
        const volScalarField& rho =
            mesh().objectRegistry::template lookupObject<volScalarField>
            (
                rhoName_
            ).oldTime();

        rDeltaT.primitiveFieldRef() = max
        (
            rDeltaT.primitiveField()/(rho.primitiveField()*mesh().V()),
            scalar(1)/deltaT.value()
        );
    }
    else
    {
        FatalErrorInFunction
            << "Incorrect dimensions of phi: " << phi.dimensions()
            << abort(FatalError);
    }

    rDeltaT.correctBoundaryConditions();

    return trDeltaT;
}


template<class Type>
tmp<volFieldType<Type>>
SLTSDdtScheme<Type>::fvcDdt
(
    const dimensioned<Type>& dt
)
{
    const volScalarField rDeltaT(SLrDeltaT());

    IOobject ddtIOobject
    (
        "ddt("+dt.name()+')',
        mesh().time().timeName(),
        mesh()
    );

    if (mesh().moving())
    {
        tmp<volFieldType<Type>> tdtdt
        (
            new volFieldType<Type>
            (
                ddtIOobject,
                mesh(),
                dimensioned<Type>
                (
                    "0",
                    dt.dimensions()/dimTime,
                    Zero
                )
            )
        );

        tdtdt.ref().primitiveFieldRef() =
            rDeltaT.primitiveField()*dt.value()*(1.0 - mesh().V0()/mesh().V());

        return tdtdt;
    }
    else
    {
        return tmp<volFieldType<Type>>
        (
            new volFieldType<Type>
            (
                ddtIOobject,
                mesh(),
                dimensioned<Type>
                (
                    "0",
                    dt.dimensions()/dimTime,
                    Zero
                ),
                calculatedFvPatchField<Type>::typeName
            )
        );
    }
}


template<class Type>
tmp<volFieldType<Type>>
SLTSDdtScheme<Type>::fvcDdt
(
    const volFieldType<Type>& vf
)
{
    const volScalarField rDeltaT(SLrDeltaT());

    IOobject ddtIOobject
    (
        "ddt("+vf.name()+')',
        mesh().time().timeName(),
        mesh()
    );

    if (mesh().moving())
    {
        return tmp<volFieldType<Type>>
        (
            new volFieldType<Type>
            (
                ddtIOobject,
                mesh(),
                rDeltaT.dimensions()*vf.dimensions(),
                rDeltaT.primitiveField()*
                (
                    vf.primitiveField()
                  - vf.oldTime().primitiveField()*mesh().V0()/mesh().V()
                ),
                rDeltaT.boundaryField()*
                (
                    vf.boundaryField() - vf.oldTime().boundaryField()
                )
            )
        );
    }
    else
    {
        return tmp<volFieldType<Type>>
        (
            new volFieldType<Type>
            (
                ddtIOobject,
                rDeltaT*(vf - vf.oldTime())
            )
        );
    }
}


template<class Type>
tmp<volFieldType<Type>>
SLTSDdtScheme<Type>::fvcDdt
(
    const dimensionedScalar& rho,
    const volFieldType<Type>& vf
)
{
    const volScalarField rDeltaT(SLrDeltaT());

    IOobject ddtIOobject
    (
        "ddt("+rho.name()+','+vf.name()+')',
        mesh().time().timeName(),
        mesh()
    );

    if (mesh().moving())
    {
        return tmp<volFieldType<Type>>
        (
            new volFieldType<Type>
            (
                ddtIOobject,
                mesh(),
                rDeltaT.dimensions()*rho.dimensions()*vf.dimensions(),
                rDeltaT.primitiveField()*rho.value()*
                (
                    vf.primitiveField()
                  - vf.oldTime().primitiveField()*mesh().V0()/mesh().V()
                ),
                rDeltaT.boundaryField()*rho.value()*
                (
                    vf.boundaryField() - vf.oldTime().boundaryField()
                )
            )
        );
    }
    else
    {
        return tmp<volFieldType<Type>>
        (
            new volFieldType<Type>
            (
                ddtIOobject,
                rDeltaT*rho*(vf - vf.oldTime())
            )
        );
    }
}


template<class Type>
tmp<volFieldType<Type>>
SLTSDdtScheme<Type>::fvcDdt
(
    const volScalarField& rho,
    const volFieldType<Type>& vf
)
{
    const volScalarField rDeltaT(SLrDeltaT());

    IOobject ddtIOobject
    (
        "ddt("+rho.name()+','+vf.name()+')',
        mesh().time().timeName(),
        mesh()
    );

    if (mesh().moving())
    {
        return tmp<volFieldType<Type>>
        (
            new volFieldType<Type>
            (
                ddtIOobject,
                mesh(),
                rDeltaT.dimensions()*rho.dimensions()*vf.dimensions(),
                rDeltaT.primitiveField()*
                (
                    rho.primitiveField()*vf.primitiveField()
                  - rho.oldTime().primitiveField()
                   *vf.oldTime().primitiveField()*mesh().V0()/mesh().V()
                ),
                rDeltaT.boundaryField()*
                (
                    rho.boundaryField()*vf.boundaryField()
                  - rho.oldTime().boundaryField()
                   *vf.oldTime().boundaryField()
                )
            )
        );
    }
    else
    {
        return tmp<volFieldType<Type>>
        (
            new volFieldType<Type>
            (
                ddtIOobject,
                rDeltaT*(rho*vf - rho.oldTime()*vf.oldTime())
            )
        );
    }
}


template<class Type>
tmp<volFieldType<Type>>
SLTSDdtScheme<Type>::fvcDdt
(
    const volScalarField& alpha,
    const volScalarField& rho,
    const volFieldType<Type>& vf
)
{
    const volScalarField rDeltaT(SLrDeltaT());

    IOobject ddtIOobject
    (
        "ddt("+alpha.name()+','+rho.name()+','+vf.name()+')',
        mesh().time().timeName(),
        mesh()
    );

    if (mesh().moving())
    {
        return tmp<volFieldType<Type>>
        (
            new volFieldType<Type>
            (
                ddtIOobject,
                mesh(),
                rDeltaT.dimensions()
               *alpha.dimensions()*rho.dimensions()*vf.dimensions(),
                rDeltaT.primitiveField()*
                (
                    alpha.primitiveField()
                   *rho.primitiveField()
                   *vf.primitiveField()

                  - alpha.oldTime().primitiveField()
                   *rho.oldTime().primitiveField()
                   *vf.oldTime().primitiveField()*mesh().Vsc0()/mesh().Vsc()
                ),
                rDeltaT.boundaryField()*
                (
                    alpha.boundaryField()
                   *rho.boundaryField()
                   *vf.boundaryField()

                  - alpha.oldTime().boundaryField()
                   *rho.oldTime().boundaryField()
                   *vf.oldTime().boundaryField()
                )
            )
        );
    }
    else
    {
        return tmp<volFieldType<Type>>
        (
            new volFieldType<Type>
            (
                ddtIOobject,
                rDeltaT
               *(
                   alpha*rho*vf
                 - alpha.oldTime()*rho.oldTime()*vf.oldTime()
                )
            )
        );
    }
}


template<class Type>
tmp<fvMatrix<Type>>
SLTSDdtScheme<Type>::fvmDdt
(
    const volFieldType<Type>& vf
)
{
    tmp<fvMatrix<Type>> tfvm
    (
        new fvMatrix<Type>
        (
            vf,
            vf.dimensions()*dimVol/dimTime
        )
    );

    fvMatrix<Type>& fvm = tfvm.ref();

    scalarField rDeltaT(SLrDeltaT()().primitiveField());

    fvm.diag() = rDeltaT*mesh().V();

    if (mesh().moving())
    {
        fvm.source() = rDeltaT*vf.oldTime().primitiveField()*mesh().V0();
    }
    else
    {
        fvm.source() = rDeltaT*vf.oldTime().primitiveField()*mesh().V();
    }

    return tfvm;
}


template<class Type>
tmp<fvMatrix<Type>>
SLTSDdtScheme<Type>::fvmDdt
(
    const dimensionedScalar& rho,
    const volFieldType<Type>& vf
)
{
    tmp<fvMatrix<Type>> tfvm
    (
        new fvMatrix<Type>
        (
            vf,
            rho.dimensions()*vf.dimensions()*dimVol/dimTime
        )
    );
    fvMatrix<Type>& fvm = tfvm.ref();

    scalarField rDeltaT(SLrDeltaT()().primitiveField());

    fvm.diag() = rDeltaT*rho.value()*mesh().V();

    if (mesh().moving())
    {
        fvm.source() = rDeltaT
            *rho.value()*vf.oldTime().primitiveField()*mesh().V0();
    }
    else
    {
        fvm.source() = rDeltaT
            *rho.value()*vf.oldTime().primitiveField()*mesh().V();
    }

    return tfvm;
}


template<class Type>
tmp<fvMatrix<Type>>
SLTSDdtScheme<Type>::fvmDdt
(
    const volScalarField& rho,
    const volFieldType<Type>& vf
)
{
    tmp<fvMatrix<Type>> tfvm
    (
        new fvMatrix<Type>
        (
            vf,
            rho.dimensions()*vf.dimensions()*dimVol/dimTime
        )
    );
    fvMatrix<Type>& fvm = tfvm.ref();

    scalarField rDeltaT(SLrDeltaT()().primitiveField());

    fvm.diag() = rDeltaT*rho.primitiveField()*mesh().V();

    if (mesh().moving())
    {
        fvm.source() = rDeltaT
            *rho.oldTime().primitiveField()
            *vf.oldTime().primitiveField()*mesh().V0();
    }
    else
    {
        fvm.source() = rDeltaT
            *rho.oldTime().primitiveField()
            *vf.oldTime().primitiveField()*mesh().V();
    }

    return tfvm;
}


template<class Type>
tmp<fvMatrix<Type>>
SLTSDdtScheme<Type>::fvmDdt
(
    const volScalarField& alpha,
    const volScalarField& rho,
    const volFieldType<Type>& vf
)
{
    tmp<fvMatrix<Type>> tfvm
    (
        new fvMatrix<Type>
        (
            vf,
            alpha.dimensions()*rho.dimensions()*vf.dimensions()*dimVol/dimTime
        )
    );
    fvMatrix<Type>& fvm = tfvm.ref();

    scalarField rDeltaT(SLrDeltaT()().primitiveField());

    fvm.diag() =
        rDeltaT*alpha.primitiveField()*rho.primitiveField()*mesh().Vsc();

    if (mesh().moving())
    {
        fvm.source() = rDeltaT
            *alpha.oldTime().primitiveField()
            *rho.oldTime().primitiveField()
            *vf.oldTime().primitiveField()*mesh().Vsc0();
    }
    else
    {
        fvm.source() = rDeltaT
            *alpha.oldTime().primitiveField()
            *rho.oldTime().primitiveField()
            *vf.oldTime().primitiveField()*mesh().Vsc();
    }

    return tfvm;
}


template<class Type>
tmp<typename SLTSDdtScheme<Type>::fluxFieldType>
SLTSDdtScheme<Type>::fvcDdtUfCorr
(
    const volFieldType<Type>& U,
    const surfaceFieldType<Type>& Uf
)
{
    const surfaceScalarField rDeltaT(fvc::interpolate(SLrDeltaT()));

    fluxFieldType phiUf0(mesh().Sf() & Uf.oldTime());
    fluxFieldType phiCorr
    (
        phiUf0 - fvc::dotInterpolate(mesh().Sf(), U.oldTime())
    );

    return tmp<fluxFieldType>
    (
        new fluxFieldType
        (
            IOobject
            (
                "ddtCorr(" + U.name() + ',' + Uf.name() + ')',
                mesh().time().timeName(),
                mesh()
            ),
            this->fvcDdtPhiCoeff(U.oldTime(), phiUf0, phiCorr)
           *rDeltaT*phiCorr
        )
    );
}


template<class Type>
tmp<typename SLTSDdtScheme<Type>::fluxFieldType>
SLTSDdtScheme<Type>::fvcDdtPhiCorr
(
    const volFieldType<Type>& U,
    const fluxFieldType& phi
)
{
    const surfaceScalarField rDeltaT(fvc::interpolate(SLrDeltaT()));

    fluxFieldType phiCorr
    (
        phi.oldTime() - fvc::dotInterpolate(mesh().Sf(), U.oldTime())
    );

    return tmp<fluxFieldType>
    (
        new fluxFieldType
        (
            IOobject
            (
                "ddtCorr(" + U.name() + ',' + phi.name() + ')',
                mesh().time().timeName(),
                mesh()
            ),
            this->fvcDdtPhiCoeff(U.oldTime(), phi.oldTime(), phiCorr)
           *rDeltaT*phiCorr
        )
    );
}


template<class Type>
tmp<typename SLTSDdtScheme<Type>::fluxFieldType>
SLTSDdtScheme<Type>::fvcDdtUfCorr
(
    const volScalarField& rho,
    const volFieldType<Type>& U,
    const surfaceFieldType<Type>& Uf
)
{
    if
    (
        U.dimensions() == dimVelocity
     && Uf.dimensions() == dimDensity*dimVelocity
    )
    {
        const surfaceScalarField rDeltaT(fvc::interpolate(SLrDeltaT()));

        volFieldType<Type> rhoU0
        (
            rho.oldTime()*U.oldTime()
        );

        fluxFieldType phiUf0(mesh().Sf() & Uf.oldTime());
        fluxFieldType phiCorr(phiUf0 - fvc::dotInterpolate(mesh().Sf(), rhoU0));

        return tmp<fluxFieldType>
        (
            new fluxFieldType
            (
                IOobject
                (
                    "ddtCorr("
                  + rho.name() + ',' + U.name() + ',' + Uf.name() + ')',
                    mesh().time().timeName(),
                    mesh()
                ),
                this->fvcDdtPhiCoeff(rhoU0, phiUf0, phiCorr)*rDeltaT*phiCorr
            )
        );
    }
    else if
    (
        U.dimensions() == dimDensity*dimVelocity
     && Uf.dimensions() == dimDensity*dimVelocity
    )
    {
        return fvcDdtUfCorr(U, Uf);
    }
    else
    {
        FatalErrorInFunction
            << "dimensions of Uf are not correct"
            << abort(FatalError);

        return fluxFieldType::null();
    }
}


template<class Type>
tmp<typename SLTSDdtScheme<Type>::fluxFieldType>
SLTSDdtScheme<Type>::fvcDdtPhiCorr
(
    const volScalarField& rho,
    const volFieldType<Type>& U,
    const fluxFieldType& phi
)
{
    if
    (
        U.dimensions() == dimVelocity
     && phi.dimensions() == rho.dimensions()*dimVelocity*dimArea
    )
    {
        dimensionedScalar rDeltaT = 1.0/mesh().time().deltaT();

        volFieldType<Type> rhoU0
        (
            rho.oldTime()*U.oldTime()
        );

        fluxFieldType phiCorr
        (
            phi.oldTime() - fvc::dotInterpolate(mesh().Sf(), rhoU0)
        );

        return tmp<fluxFieldType>
        (
            new fluxFieldType
            (
                IOobject
                (
                    "ddtCorr("
                  + rho.name() + ',' + U.name() + ',' + phi.name() + ')',
                    mesh().time().timeName(),
                    mesh()
                ),
                this->fvcDdtPhiCoeff(rhoU0, phi.oldTime(), phiCorr)
               *rDeltaT*phiCorr
            )
        );
    }
    else if
    (
        U.dimensions() == rho.dimensions()*dimVelocity
     && phi.dimensions() == rho.dimensions()*dimVelocity*dimArea
    )
    {
        return fvcDdtPhiCorr(U, phi);
    }
    else
    {
        FatalErrorInFunction
            << "dimensions of phi are not correct"
            << abort(FatalError);

        return fluxFieldType::null();
    }
}


template<class Type>
tmp<surfaceScalarField> SLTSDdtScheme<Type>::meshPhi
(
    const volFieldType<Type>&
)
{
    return tmp<surfaceScalarField>
    (
        new surfaceScalarField
        (
            IOobject
            (
                "meshPhi",
                mesh().time().timeName(),
                mesh(),
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            mesh(),
            dimensionedScalar("0", dimVolume/dimTime, 0.0)
        )
    );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
