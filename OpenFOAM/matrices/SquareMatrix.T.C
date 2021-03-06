﻿/*---------------------------------------------------------------------------*\
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

//#include "SquareMatrix.T.H"
#include "labelList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
namespace Foam {
    template<class Type>
    scalar detDecomposed
    (
        const SquareMatrix<Type>& matrix,
        const label sign
    )
    {
        Type diagProduct = pTraits<Type>::one;

        for (label i = 0; i < matrix.m(); i++)
        {
            diagProduct *= matrix(i, i);
        }

        return sign * diagProduct;
    }


    template<class Type>
    scalar det(const SquareMatrix<Type>& matrix)
    {
        SquareMatrix<Type> matrixTmp = matrix;

        labelList pivotIndices(matrix.m());
        label sign;
        LUDecompose(matrixTmp, pivotIndices, sign);

        return detDecomposed(matrixTmp, sign);
    }


    template<class Type>
    scalar det(SquareMatrix<Type>& matrix)
    {
        labelList pivotIndices(matrix.m());
        label sign;
        LUDecompose(matrix, pivotIndices, sign);

        return detDecomposed(matrix, sign);
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
