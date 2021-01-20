﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2013-2017 OpenFOAM Foundation
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

//#include "cyclicACMIFvPatchField.H"
#include "transformField.H"
#include "GeometricField.T.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //
namespace Foam {
    template<class Type>
    cyclicACMIFvPatchField<Type>::cyclicACMIFvPatchField
    (
        const fvPatch& p,
        const DimensionedField<Type, volMesh>& iF
    )
        :
        cyclicACMILduInterfaceField(),
        coupledFvPatchField<Type>(p, iF),
        cyclicACMIPatch_(refCast<const cyclicACMIFvPatch>(p))
    {}


    template<class Type>
    cyclicACMIFvPatchField<Type>::cyclicACMIFvPatchField
    (
        const fvPatch& p,
        const DimensionedField<Type, volMesh>& iF,
        const dictionary& dict
    )
        :
        cyclicACMILduInterfaceField(),
        coupledFvPatchField<Type>(p, iF, dict, dict.found("value")),
        cyclicACMIPatch_(refCast<const cyclicACMIFvPatch>(p))
    {
        if (!isA<cyclicACMIFvPatch>(p))
        {
            FatalIOErrorInFunction
            (
                dict
            ) << "    patch type '" << p.type()
                << "' not constraint type '" << typeName << "'"
                << "\n    for patch " << p.name()
                << " of field " << this->internalField().name()
                << " in file " << this->internalField().objectPath()
                << exit(FatalIOError);
        }

        if (!dict.found("value") && this->coupled())
        {
            this->evaluate(Pstream::commsTypes::blocking);
        }
    }


    template<class Type>
    cyclicACMIFvPatchField<Type>::cyclicACMIFvPatchField
    (
        const cyclicACMIFvPatchField<Type>& ptf,
        const fvPatch& p,
        const DimensionedField<Type, volMesh>& iF,
        const fvPatchFieldMapper& mapper
    )
        :
        cyclicACMILduInterfaceField(),
        coupledFvPatchField<Type>(ptf, p, iF, mapper),
        cyclicACMIPatch_(refCast<const cyclicACMIFvPatch>(p))
    {
        if (!isA<cyclicACMIFvPatch>(this->patch()))
        {
            FatalErrorInFunction
                << "' not constraint type '" << typeName << "'"
                << "\n    for patch " << p.name()
                << " of field " << this->internalField().name()
                << " in file " << this->internalField().objectPath()
                << exit(FatalIOError);
        }
    }



    template<class Type>
    cyclicACMIFvPatchField<Type>::cyclicACMIFvPatchField
    (
        const cyclicACMIFvPatchField<Type>& ptf
    )
        :
        cyclicACMILduInterfaceField(),
        coupledFvPatchField<Type>(ptf),
        cyclicACMIPatch_(ptf.cyclicACMIPatch_)
    {}


    template<class Type>
    cyclicACMIFvPatchField<Type>::cyclicACMIFvPatchField
    (
        const cyclicACMIFvPatchField<Type>& ptf,
        const DimensionedField<Type, volMesh>& iF
    )
        :
        cyclicACMILduInterfaceField(),
        coupledFvPatchField<Type>(ptf, iF),
        cyclicACMIPatch_(ptf.cyclicACMIPatch_)
    {}


    // * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

    template<class Type>
    bool cyclicACMIFvPatchField<Type>::coupled() const
    {
        return cyclicACMIPatch_.coupled();
    }


    template<class Type>
    tmp<Field<Type>>
        cyclicACMIFvPatchField<Type>::patchNeighbourField() const
    {
        const Field<Type>& iField = this->primitiveField();
        const cyclicACMIPolyPatch& cpp = cyclicACMIPatch_.cyclicACMIPatch();
        tmp<Field<Type>> tpnf
        (
            cyclicACMIPatch_.interpolate
            (
                Field<Type>
                (
                    iField,
                    cpp.neighbPatch().faceCells()
                    )
            )
        );

        if (doTransform())
        {
            tpnf.ref() = transform(forwardT(), tpnf());
        }

        return tpnf;
    }


    template<class Type>
    const cyclicACMIFvPatchField<Type>&
        cyclicACMIFvPatchField<Type>::neighbourPatchField() const
    {
        const volFieldType<Type>& fld =
            static_cast<const volFieldType<Type>&>
            (
                this->primitiveField()
                );

        return refCast<const cyclicACMIFvPatchField<Type>>
            (
                fld.boundaryField()[cyclicACMIPatch_.neighbPatchID()]
                );
    }


    template<class Type>
    const fvPatchField<Type>&
        cyclicACMIFvPatchField<Type>::nonOverlapPatchField() const
    {
        const volFieldType<Type>& fld =
            static_cast<const volFieldType<Type>&>
            (
                this->primitiveField()
                );

        return fld.boundaryField()[cyclicACMIPatch_.nonOverlapPatchID()];
    }


    template<class Type>
    void cyclicACMIFvPatchField<Type>::updateInterfaceMatrix
    (
        scalarField& result,
        const scalarField& psiInternal,
        const scalarField& coeffs,
        const direction cmpt,
        const Pstream::commsTypes
    ) const
    {
        const cyclicACMIPolyPatch& cpp = cyclicACMIPatch_.cyclicACMIPatch();

        // note: only applying coupled contribution

        const labelUList& nbrFaceCellsCoupled =
            cpp.neighbPatch().faceCells();

        scalarField pnf(psiInternal, nbrFaceCellsCoupled);

        // Transform according to the transformation tensors
        transformCoupleField(pnf, cmpt);

        const labelUList& faceCells = cyclicACMIPatch_.faceCells();

        pnf = cyclicACMIPatch_.interpolate(pnf);

        forAll(faceCells, elemI)
        {
            result[faceCells[elemI]] -= coeffs[elemI] * pnf[elemI];
        }
    }


    template<class Type>
    void cyclicACMIFvPatchField<Type>::updateInterfaceMatrix
    (
        Field<Type>& result,
        const Field<Type>& psiInternal,
        const scalarField& coeffs,
        const Pstream::commsTypes
    ) const
    {
        const cyclicACMIPolyPatch& cpp = cyclicACMIPatch_.cyclicACMIPatch();

        // note: only applying coupled contribution

        const labelUList& nbrFaceCellsCoupled = cpp.neighbPatch().faceCells();

        Field<Type> pnf(psiInternal, nbrFaceCellsCoupled);

        // Transform according to the transformation tensors
        transformCoupleField(pnf);

        const labelUList& faceCells = cyclicACMIPatch_.faceCells();

        pnf = cyclicACMIPatch_.interpolate(pnf);

        forAll(faceCells, elemI)
        {
            result[faceCells[elemI]] -= coeffs[elemI] * pnf[elemI];
        }
    }


    template<class Type>
    void cyclicACMIFvPatchField<Type>::manipulateMatrix
    (
        fvMatrix<Type>& matrix
    )
    {
        const scalarField& mask = cyclicACMIPatch_.cyclicACMIPatch().mask();

        // nothing to be done by the AMI, but re-direct to non-overlap patch
        // with non-overlap patch weights
        const fvPatchField<Type>& npf = nonOverlapPatchField();

        const_cast<fvPatchField<Type>&>(npf).manipulateMatrix(matrix, 1.0 - mask);
    }


    template<class Type>
    void cyclicACMIFvPatchField<Type>::updateCoeffs()
    {
        // Update non-overlap patch - some will implement updateCoeffs, and
        // others will implement evaluate

        // Pass in (1 - mask) to give non-overlap patch the chance to do
        // manipulation of non-face based data

        const scalarField& mask = cyclicACMIPatch_.cyclicACMIPatch().mask();
        const fvPatchField<Type>& npf = nonOverlapPatchField();
        const_cast<fvPatchField<Type>&>(npf).updateWeightedCoeffs(1.0 - mask);
    }


    template<class Type>
    void cyclicACMIFvPatchField<Type>::write(Ostream& os) const
    {
        fvPatchField<Type>::write(os);
        this->writeEntry("value", os);
    }

}
// ************************************************************************* //
