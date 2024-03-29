﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2017 OpenFOAM Foundation
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

Class
    Foam::BarycentricTensor

Description
    Templated 4x3 tensor derived from VectorSpace. Has 12 components. Can
    represent a barycentric transformation as a matrix-barycentric inner-
    product. Can alternatively represent an inverse barycentric transformation
    as a vector-matrix inner-product.

SourceFiles
    BarycentricTensorI.H

\*---------------------------------------------------------------------------*/

#ifndef BarycentricTensor_H
#define BarycentricTensor_H

#include "Barycentric.H"
#include "Tensor.H"
#include "Vector.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                         Class BarycentricTensor Declaration
\*---------------------------------------------------------------------------*/

template<class Cmpt>
class BarycentricTensor
:
    public MatrixSpace<BarycentricTensor<Cmpt>, Cmpt, 4, 3>
{
public:

    // Typedefs

        //- Equivalent type of labels used for valid component indexing
        typedef Tensor<label> labelType;


    // Member Constants

        //- Rank of BarycentricTensor is 2
        static constexpr direction rank = 2;


    //- Component labeling enumeration
    enum components { XA, XB, XC, XD, YA, YB, YC, YD, ZA, ZB, ZC, ZD };


    // Generated Methods

        //- Default construct
        BarycentricTensor() = default;


    // Constructors

        //- Construct initialised to zero
        inline BarycentricTensor(const zero);

        //- Construct given three barycentric components (rows)
        inline BarycentricTensor
        (
            const Barycentric<Cmpt>& x,
            const Barycentric<Cmpt>& y,
            const Barycentric<Cmpt>& z
        );

        //- Construct given four vector components (columns)
        inline BarycentricTensor
        (
            const Vector<Cmpt>& a,
            const Vector<Cmpt>& b,
            const Vector<Cmpt>& c,
            const Vector<Cmpt>& d
        );


    // Member Functions

        // Row-barycentric access

            inline Barycentric<Cmpt> x() const;
            inline Barycentric<Cmpt> y() const;
            inline Barycentric<Cmpt> z() const;

        // Column-vector access

            inline Vector<Cmpt> a() const;
            inline Vector<Cmpt> b() const;
            inline Vector<Cmpt> c() const;
            inline Vector<Cmpt> d() const;
};


// * * * * * * * * * * * * * * * * * Traits  * * * * * * * * * * * * * * * * //

//- Data are contiguous if component type is contiguous
template<class Cmpt>
struct is_contiguous<BarycentricTensor<Cmpt>> : is_contiguous<Cmpt> {};

//- Data are contiguous label if component type is label
template<class Cmpt>
struct is_contiguous_label<BarycentricTensor<Cmpt>>
:
    is_contiguous_label<Cmpt>
{};

//- Data are contiguous scalar if component type is scalar
template<class Cmpt>
struct is_contiguous_scalar<BarycentricTensor<Cmpt>>
:
    is_contiguous_scalar<Cmpt>
{};


template<class Cmpt>
class typeOfTranspose<Cmpt, BarycentricTensor<Cmpt>>
{
public:

    typedef void type;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "BarycentricTensorI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
