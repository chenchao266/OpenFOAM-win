﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2017 OpenFOAM Foundation
    Copyright (C) 2020 OpenCFD Ltd.
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
    Foam::linearEqn

Description
    Container to encapsulate various operations for
    linear equation of the forms with real coefficients:

    \f[
        a*x + b = 0
          x + B = 0
    \f]

See also
    Test-linearEqn.C

SourceFiles
    linearEqnI.H

\*---------------------------------------------------------------------------*/

#ifndef linearEqn_H
#define linearEqn_H

#include "Roots.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                         Class linearEqn Declaration
\*---------------------------------------------------------------------------*/

class linearEqn
:
    public VectorSpace<linearEqn, scalar, 2>
{
public:

    //- Component labeling enumeration
    enum components { A, B };


    // Constructors

        //- Construct null
        inline linearEqn();

        //- Construct initialized to zero
        inline linearEqn(const zero);

        //- Construct from components
        inline linearEqn(const scalar a, const scalar b);


    // Member Functions

    // Access

        inline scalar a() const;
        inline scalar b() const;

        inline scalar& a();
        inline scalar& b();

    // Evaluate

        //- Evaluate the linear equation at x
        inline scalar value(const scalar x) const;

        //- Evaluate the derivative of the linear equation at x
        inline scalar derivative(const scalar x) const;

        //- Estimate the error of evaluation of the linear equation at x
        inline scalar error(const scalar x) const;

        //- Return the real root of the linear equation
        inline Roots<1> roots() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "linearEqnI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
