﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2013-2016 OpenFOAM Foundation
    Copyright (C) 2019-2020 OpenCFD Ltd.
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

#include "SquareMatrix.H"
#include "RectangularMatrix.H"
#include "labelList.H"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //
namespace Foam {
template<class Type>
template<class CompOp>
List<label> SquareMatrix<Type>::sortPermutation
(
    CompOp& compare
) const
{
    List<label> p(this->m());
    std::iota(p.begin(), p.end(), 0);
    std::sort
    (
        p.begin(),
        p.end(),
        [&](label i, label j){ return compare((*this)(i,i), (*this)(j,j)); }
    );

    return p;
}


template<class Type>
void SquareMatrix<Type>::applyPermutation(const List<label>& p)
{
    #ifdef FULLDEBUG
    if (this->m() != p.size())
    {
        FatalErrorInFunction
            << "Attempt to column-reorder according to an uneven list: " << nl
            << "SquareMatrix diagonal size = " << this->m() << nl
            << "Permutation list size = " << p.size() << nl
            << abort(FatalError);
    }
    #endif

    SquareMatrix<Type> reordered(this->sizes());

    label j = 0;
    for (const label i : p)
    {
        reordered.subColumn(j) = this->subColumn(i);
        ++j;
    }

    this->transfer(reordered);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Type>
template<class AnyType>
void SquareMatrix<Type>::operator=(const Identity<AnyType>)
{
    Matrix<SquareMatrix<Type>, Type>::operator=(Zero);

    for (label i = 0; i < this->n(); ++i)
    {
        this->operator()(i, i) = pTraits<Type>::one;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


 } // End namespace Foam
namespace Foam
{

// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

//- Return the determinant of the LU decomposed SquareMatrix
template<class Type>
scalar detDecomposed
(
    const SquareMatrix<Type>& matrix,
    const label sign
)
{
    Type diagProduct = pTraits<Type>::one;

    for (label i = 0; i < matrix.m(); ++i)
    {
        diagProduct *= matrix(i, i);
    }

    return sign*diagProduct;
}


//- Return the determinant of SquareMatrix
template<class Type>
scalar det(const SquareMatrix<Type>& matrix)
{
    SquareMatrix<Type> matrixTmp = matrix;

    labelList pivotIndices(matrix.m());
    label sign;
    LUDecompose(matrixTmp, pivotIndices, sign);

    return detDecomposed(matrixTmp, sign);
}


//- Return the SquareMatrix det and the LU decomposition in the original matrix
template<class Type>
scalar det(SquareMatrix<Type>& matrix)
{
    labelList pivotIndices(matrix.m());
    label sign;
    LUDecompose(matrix, pivotIndices, sign);

    return detDecomposed(matrix, sign);
}


//- Return Matrix column-reordered according to
//- a given permutation labelList
template<class Type>
SquareMatrix<Type> applyPermutation
(
    const SquareMatrix<Type>& mat,
    const List<label>& p
)
{
    #ifdef FULLDEBUG
    if (mat.m() != p.size())
    {
        FatalErrorInFunction
            << "Attempt to column-reorder according to an uneven list: " << nl
            << "SquareMatrix diagonal size = " << mat.m() << nl
            << "Permutation list size = " << p.size() << nl
            << abort(FatalError);
    }
    #endif

    SquareMatrix<Type> reordered(mat.sizes());

    label j = 0;
    for (const label i : p)
    {
        reordered.subColumn(j) = mat.subColumn(i);
        ++j;
    }

    return reordered;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
class typeOfInnerProduct<Type, SquareMatrix<Type>, SquareMatrix<Type>>
{
public:

    typedef SquareMatrix<Type> type;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
