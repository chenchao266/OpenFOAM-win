﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2017 OpenFOAM Foundation
    Copyright (C) 2021 OpenCFD Ltd.
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

#include "OneConstant.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


 namespace Foam{
template<class Type>
Function1Types::OneConstant<Type>::OneConstant
(
    const word& entryName,
    const objectRegistry* obrPtr
)
:
    Function1<Type>(entryName, obrPtr)
{}


template<class Type>
Function1Types::OneConstant<Type>::OneConstant
(
    const word& entryName,
    const dictionary& dict,
    const objectRegistry* obrPtr
)
:
    Function1<Type>(entryName, dict, obrPtr)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
tmp<Field<Type>> Function1Types::OneConstant<Type>::value
(
    const scalarField& x
) const
{
    return tmp<Field<Type>>::New(x.size(), pTraits<Type>::one_);
}


template<class Type>
tmp<Field<Type>> Function1Types::OneConstant<Type>::integrate
(
    const scalarField& x1,
    const scalarField& x2
) const
{
    return (x2 - x1)*pTraits<Type>::one_;
}


template<class Type>
void Function1Types::OneConstant<Type>::writeData(Ostream& os) const
{
    Function1<Type>::writeData(os);
    os.endEntry();
}


// ************************************************************************* //

 } // End namespace Foam
