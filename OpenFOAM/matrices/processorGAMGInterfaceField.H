﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2014 OpenFOAM Foundation
    Copyright (C) 2019 OpenCFD Ltd.
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

Class
    Foam::processorGAMGInterfaceField

Description
    GAMG agglomerated processor interface field.

SourceFiles
    processorGAMGInterfaceField.C

\*---------------------------------------------------------------------------*/

#ifndef processorGAMGInterfaceField_H
#define processorGAMGInterfaceField_H

#include "GAMGInterfaceField.H"
#include "processorGAMGInterface.H"
#include "processorLduInterfaceField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                  Class processorGAMGInterfaceField Declaration
\*---------------------------------------------------------------------------*/

class processorGAMGInterfaceField
:
    public GAMGInterfaceField,
    public processorLduInterfaceField
{
    // Private data

        //- Local reference cast into the processor interface
        const processorGAMGInterface& procInterface_;

        //- Is the transform required
        bool doTransform_;

        //- Rank of component for transformation
        int rank_;


        // Sending and receiving

            //- Outstanding request
            mutable label outstandingSendRequest_;

            //- Outstanding request
            mutable label outstandingRecvRequest_;

            //- Scalar send buffer
            mutable solveScalarField scalarSendBuf_;

            //- Scalar receive buffer
            mutable solveScalarField scalarReceiveBuf_;



    // Private Member Functions

        //- No copy construct
        processorGAMGInterfaceField
        (
            const processorGAMGInterfaceField&
        ) = delete;

        //- No copy assignment
        void operator=(const processorGAMGInterfaceField&) = delete;


public:

    //- Runtime type information
    TypeName("processor");


    // Constructors

        //- Construct from GAMG interface and fine level interface field
        processorGAMGInterfaceField
        (
            const GAMGInterface& GAMGCp,
            const lduInterfaceField& fineInterface
        );

        //- Construct from GAMG interface and fine level interface field
        processorGAMGInterfaceField
        (
            const GAMGInterface& GAMGCp,
            const bool doTransform,
            const int rank
        );


    //- Destructor
    virtual ~processorGAMGInterfaceField() = default;


    // Member Functions

        // Access

            //- Return size
            label size() const
            {
                return procInterface_.size();
            }


        // Interface matrix update

            //- Initialise neighbour matrix update
            virtual void initInterfaceMatrixUpdate
            (
                solveScalarField& result,
                const bool add,
                const lduAddressing& lduAddr,
                const label patchId,
                const solveScalarField& psiInternal,
                const scalarField& coeffs,
                const direction cmpt,
                const Pstream::commsTypes commsType
            ) const;

            //- Update result field based on interface functionality
            virtual void updateInterfaceMatrix
            (
                solveScalarField& result,
                const bool add,
                const lduAddressing& lduAddr,
                const label patchId,
                const solveScalarField& psiInternal,
                const scalarField& coeffs,
                const direction cmpt,
                const Pstream::commsTypes commsType
            ) const;


        //- Processor interface functions

            //- Return communicator used for comms
            virtual label comm() const
            {
                return procInterface_.comm();
            }

            //- Return processor number
            virtual int myProcNo() const
            {
                return procInterface_.myProcNo();
            }

            //- Return neighbour processor number
            virtual int neighbProcNo() const
            {
                return procInterface_.neighbProcNo();
            }

            //- Does the interface field perform the transformation
            virtual bool doTransform() const
            {
                return doTransform_;
            }

            //- Return face transformation tensor
            virtual const tensorField& forwardT() const
            {
                return procInterface_.forwardT();
            }

            //- Return rank of component for transform
            virtual int rank() const
            {
                return rank_;
            }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
