﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2015 OpenFOAM Foundation
    Copyright (C) 2018 OpenCFD Ltd.
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

//#include "Random.H"
#include "Swap.H"
#include "Pstream.H"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


 namespace Foam{
template<class Type>
Type Random::sample01()
{
    Type value;
    for (direction cmpt=0; cmpt<pTraits<Type>::nComponents; ++cmpt)
    {
        value.component(cmpt) = scalar01();
    }

    return value;
}


template<class Type>
Type Random::GaussNormal()
{
    Type value;
    for (direction cmpt=0; cmpt<pTraits<Type>::nComponents; ++cmpt)
    {
        value.component(cmpt) = GaussNormal<scalar>();
    }

    return value;
}


template<class Type>
Type Random::position(const Type& start, const Type& end)
{
    Type value(start);
    for (direction cmpt=0; cmpt<pTraits<Type>::nComponents; ++cmpt)
    {
        value.component(cmpt) +=
            scalar01()*(end.component(cmpt) - start.component(cmpt));
    }

    return value;
}


template<class Type>
void Random::randomise01(Type& value)
{
    value = sample01<Type>();
}


template<class Type>
void Random::shuffle(UList<Type>& values)
{
    for (label posi = values.size()-1; posi > 0; --posi)
    {
        const label i = position<label>(0, posi);
        Swap(values[i], values[posi]);
    }
}


template<class Type>
Type Random::globalSample01()
{
    Type value = -GREAT*pTraits<Type>::one_;

    if (Pstream::master())
    {
        value = sample01<Type>();
    }

    Pstream::scatter(value);

    return value;
}


template<class Type>
Type Random::globalGaussNormal()
{
    Type value = -GREAT*pTraits<Type>::one_;

    if (Pstream::master())
    {
        value = GaussNormal<Type>();
    }

    Pstream::scatter(value);

    return value;
}


template<class Type>
Type Random::globalPosition(const Type& start, const Type& end)
{
    Type value = -GREAT*pTraits<Type>::one_;

    if (Pstream::master())
    {
        value = position<Type>(start, end);
    }

    Pstream::scatter(value);

    return value;
}


template<class Type>
void Random::globalRandomise01(Type& value)
{
    value = -GREAT*pTraits<Type>::one_;

    if (Pstream::master())
    {
        value = sample01<Type>();
    }

    Pstream::scatter(value);
}


// ************************************************************************* //

 } // End namespace Foam
