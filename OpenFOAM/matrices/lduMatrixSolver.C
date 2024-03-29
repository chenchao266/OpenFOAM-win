﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
    Copyright (C) 2019-2021 OpenCFD Ltd.
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

//#include "LduMatrix.H"
#include "DiagonalSolver.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


 namespace Foam{
template<class Type, class DType, class LUType>
autoPtr<typename LduMatrix<Type, DType, LUType>::solver>
LduMatrix<Type, DType, LUType>::solver::New
(
    const word& fieldName,
    const LduMatrix<Type, DType, LUType>& matrix,
    const dictionary& solverDict
)
{
    const word solverName(solverDict.get<word>("solver"));

    if (matrix.diagonal())
    {
        return autoPtr<typename LduMatrix<Type, DType, LUType>::solver>
        (
            new DiagonalSolver<Type, DType, LUType>
            (
                fieldName,
                matrix,
                solverDict
            )
        );
    }
    else if (matrix.symmetric())
    {
        auto* ctorPtr = symMatrixConstructorTable(solverName);

        if (!ctorPtr)
        {
            FatalIOErrorInLookup
            (
                solverDict,
                "symmetric matrix solver",
                solverName,
                *symMatrixConstructorTablePtr_
            ) << exit(FatalIOError);
        }

        return autoPtr<typename LduMatrix<Type, DType, LUType>::solver>
        (
            ctorPtr
            (
                fieldName,
                matrix,
                solverDict
            )
        );
    }
    else if (matrix.asymmetric())
    {
        auto* ctorPtr = asymMatrixConstructorTable(solverName);

        if (!ctorPtr)
        {
            FatalIOErrorInLookup
            (
                solverDict,
                "asymmetric matrix solver",
                solverName,
                *asymMatrixConstructorTablePtr_
            ) << exit(FatalIOError);
        }

        return autoPtr<typename LduMatrix<Type, DType, LUType>::solver>
        (
            ctorPtr
            (
                fieldName,
                matrix,
                solverDict
            )
        );
    }

    FatalIOErrorInFunction(solverDict)
        << "cannot solve incomplete matrix, "
           "no diagonal or off-diagonal coefficient"
        << exit(FatalIOError);

    return nullptr;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type, class DType, class LUType>
LduMatrix<Type, DType, LUType>::solver::solver
(
    const word& fieldName,
    const LduMatrix<Type, DType, LUType>& matrix,
    const dictionary& solverDict
)
:
    fieldName_(fieldName),
    matrix_(matrix),

    controlDict_(solverDict),

    log_(1),
    minIter_(0),
    maxIter_(defaultMaxIter_),
    tolerance_(1e-6*pTraits<Type>::one_),
    relTol_(Zero)
{
    readControls();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type, class DType, class LUType>
void LduMatrix<Type, DType, LUType>::solver::readControls()
{
    controlDict_.readIfPresent("log", log_);
    controlDict_.readIfPresent("minIter", minIter_);
    controlDict_.readIfPresent("maxIter", maxIter_);
    controlDict_.readIfPresent("tolerance", tolerance_);
    controlDict_.readIfPresent("relTol", relTol_);
}


template<class Type, class DType, class LUType>
void LduMatrix<Type, DType, LUType>::solver::read
(
    const dictionary& solverDict
)
{
    controlDict_ = solverDict;
    readControls();
}


template<class Type, class DType, class LUType>
Type LduMatrix<Type, DType, LUType>::solver::normFactor
(
    const Field<Type>& psi,
    const Field<Type>& Apsi,
    Field<Type>& tmpField
) const
{
    // --- Calculate A dot reference value of psi
    matrix_.sumA(tmpField);
    cmptMultiply(tmpField, tmpField, gAverage(psi));

    return stabilise
    (
        gSum(cmptMag(Apsi - tmpField) + cmptMag(matrix_.source() - tmpField)),
        SolverPerformance<Type>::small_
    );

    // At convergence this simpler method is equivalent to the above
    // return stabilise(2*gSumCmptMag(matrix_.source()), matrix_.small_);
}


// ************************************************************************* //

 } // End namespace Foam
