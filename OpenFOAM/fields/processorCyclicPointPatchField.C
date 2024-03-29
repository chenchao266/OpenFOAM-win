﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
    Copyright (C) 2020-2021 OpenCFD Ltd.
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

#include "processorCyclicPointPatchField.H"
#include "transformField.H"
#include "processorPolyPatch.H"


// * * * * * * * * * * * * * * * * Constructors * * * * * * * * * * * * * * //


 namespace Foam{
template<class Type>
processorCyclicPointPatchField<Type>::processorCyclicPointPatchField
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF
)
:
    coupledPointPatchField<Type>(p, iF),
    procPatch_(refCast<const processorCyclicPointPatch>(p)),
    receiveBuf_(0)
{}


template<class Type>
processorCyclicPointPatchField<Type>::processorCyclicPointPatchField
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const dictionary& dict
)
:
    coupledPointPatchField<Type>(p, iF, dict),
    procPatch_(refCast<const processorCyclicPointPatch>(p, dict)),
    receiveBuf_(0)
{}


template<class Type>
processorCyclicPointPatchField<Type>::processorCyclicPointPatchField
(
    const processorCyclicPointPatchField<Type>& ptf,
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const pointPatchFieldMapper& mapper
)
:
    coupledPointPatchField<Type>(ptf, p, iF, mapper),
    procPatch_(refCast<const processorCyclicPointPatch>(ptf.patch())),
    receiveBuf_(0)
{}


template<class Type>
processorCyclicPointPatchField<Type>::processorCyclicPointPatchField
(
    const processorCyclicPointPatchField<Type>& ptf,
    const DimensionedField<Type, pointMesh>& iF
)
:
    coupledPointPatchField<Type>(ptf, iF),
    procPatch_(refCast<const processorCyclicPointPatch>(ptf.patch())),
    receiveBuf_(0)
{}


// * * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * //

template<class Type>
processorCyclicPointPatchField<Type>::~processorCyclicPointPatchField()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void processorCyclicPointPatchField<Type>::initSwapAddSeparated
(
    const Pstream::commsTypes commsType,
    Field<Type>& pField
) const
{
    if (Pstream::parRun())
    {
        // Get internal field into correct order for opposite side
        Field<Type> pf
        (
            this->patchInternalField
            (
                pField,
                procPatch_.reverseMeshPoints()
            )
        );

        if (commsType == Pstream::commsTypes::nonBlocking)
        {
            receiveBuf_.setSize(pf.size());
            IPstream::read
            (
                commsType,
                procPatch_.neighbProcNo(),
                receiveBuf_.data_bytes(),
                receiveBuf_.size_bytes(),
                procPatch_.tag(),
                procPatch_.comm()
            );
        }
        OPstream::write
        (
            commsType,
            procPatch_.neighbProcNo(),
            pf.cdata_bytes(),
            pf.size_bytes(),
            procPatch_.tag(),
            procPatch_.comm()
        );
    }
}


template<class Type>
void processorCyclicPointPatchField<Type>::swapAddSeparated
(
    const Pstream::commsTypes commsType,
    Field<Type>& pField
) const
{
    if (Pstream::parRun())
    {
        // If nonblocking data has already been received into receiveBuf_
        if (commsType != Pstream::commsTypes::nonBlocking)
        {
            receiveBuf_.setSize(this->size());
            IPstream::read
            (
                commsType,
                procPatch_.neighbProcNo(),
                receiveBuf_.data_bytes(),
                receiveBuf_.size_bytes(),
                procPatch_.tag(),
                procPatch_.comm()
            );
        }

        if (doTransform())
        {
            const processorCyclicPolyPatch& ppp =
                procPatch_.procCyclicPolyPatch();
            const tensor& forwardT = ppp.forwardT()[0];

            transform(receiveBuf_, forwardT, receiveBuf_);
        }

        // All points are separated
        this->addToInternalField(pField, receiveBuf_);
    }
}


// ************************************************************************* //

 } // End namespace Foam
