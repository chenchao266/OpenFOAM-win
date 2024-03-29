﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
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
    Foam::SymmTensor2D

Description
    A templated (2 x 2) symmetric tensor of objects of \<T\>, effectively
    containing 3 elements, derived from VectorSpace.

See also
    Test-SymmTensor2D.C

SourceFiles
    SymmTensor2DI.H

\*---------------------------------------------------------------------------*/

#ifndef SymmTensor2D_H
#define SymmTensor2D_H

#include "contiguous.H"
#include "Vector2D.H"
#include "SphericalTensor2D.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                         Class SymmTensor2D Declaration
\*---------------------------------------------------------------------------*/

template<class Cmpt>
class SymmTensor2D
:
    public VectorSpace<SymmTensor2D<Cmpt>, Cmpt, 3>
{
public:

    // Typedefs

        //- Equivalent type of labels used for valid component indexing
        typedef SymmTensor2D<label> labelType;


    // Member Constants

        //- Rank of SymmTensor2D is 2
        static constexpr direction rank = 2;


    // Static Data Members

        static const SymmTensor2D I;


    //- Component labeling enumeration
    enum components { XX, XY, YY };


    // Generated Methods

        //- Default construct
        SymmTensor2D() = default;

        //- Copy construct
        SymmTensor2D(const SymmTensor2D&) = default;

        //- Copy assignment
        SymmTensor2D& operator=(const SymmTensor2D&) = default;


    // Constructors

        //- Construct initialized to zero
        inline SymmTensor2D(const zero);

        //- Construct given VectorSpace
        inline SymmTensor2D(const VectorSpace<SymmTensor2D<Cmpt>, Cmpt, 3>&);

        //- Construct given SphericalTensor
        inline SymmTensor2D(const SphericalTensor2D<Cmpt>&);

        //- Construct given the three components
        inline SymmTensor2D
        (
            const Cmpt txx, const Cmpt txy,
                            const Cmpt tyy
        );

        //- Construct from Istream
        inline explicit SymmTensor2D(Istream& is);


    // Member Functions

        // Access

            inline const Cmpt& xx() const;
            inline const Cmpt& xy() const;
            inline const Cmpt& yx() const;
            inline const Cmpt& yy() const;

            inline Cmpt& xx();
            inline Cmpt& xy();
            inline Cmpt& yx();
            inline Cmpt& yy();


        // Diagonal access and manipulation

            //- Extract the diagonal as a vector
            inline Vector2D<Cmpt> diag() const;

            //- Set values of the diagonal
            inline void diag(const Vector2D<Cmpt>& v);


        //- Return non-Hermitian transpose
        inline const SymmTensor2D<Cmpt>& T() const;


    // Member Operators

        //- Inherit VectorSpace assignment operators
        using VectorSpace<SymmTensor2D<Cmpt>, Cmpt, 3>::operator=;//SymmTensor2D::vsType

        //- Construct given SphericalTensor2D
        inline void operator=(const SphericalTensor2D<Cmpt>&);
};


// * * * * * * * * * * * * * * * * * Traits  * * * * * * * * * * * * * * * * //

//- Data are contiguous if component type is contiguous
template<class Cmpt>
struct is_contiguous<SymmTensor2D<Cmpt>> : is_contiguous<Cmpt> {};

//- Data are contiguous label if component type is label
template<class Cmpt>
struct is_contiguous_label<SymmTensor2D<Cmpt>> : is_contiguous_label<Cmpt> {};

//- Data are contiguous scalar if component type is scalar
template<class Cmpt>
struct is_contiguous_scalar<SymmTensor2D<Cmpt>> : is_contiguous_scalar<Cmpt> {};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "SymmTensor2DI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
