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

#include "fv.H"
//#include "limitedSnGrad.H"
#include "volFields.H"
#include "surfaceFields.H"
#include "localMax.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fv
{

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
limitedSnGrad<Type>::~limitedSnGrad()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
tmp<surfaceFieldType<Type>>
limitedSnGrad<Type>::correction
(
    const volFieldType<Type>& vf
) const
{
    const surfaceFieldType<Type> corr
    (
        correctedScheme_().correction(vf)
    );

    const surfaceScalarField limiter
    (
        min
        (
            limitCoeff_
           *mag(snGradScheme<Type>::snGrad(vf, deltaCoeffs(vf), "SndGrad"))
           /(
                (1 - limitCoeff_)*mag(corr)
              + dimensionedScalar("small", corr.dimensions(), SMALL)
            ),
            dimensionedScalar("one", dimless, 1.0)
        )
    );

    if (fv::debug)
    {
        InfoInFunction
            << "limiter min: " << min(limiter.primitiveField())
            << " max: "<< max(limiter.primitiveField())
            << " avg: " << average(limiter.primitiveField()) << endl;
    }

    return limiter*corr;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
