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

#include "transformList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


 namespace Foam{
template<class T>
List<T> transform
(
    const tensor& rotTensor,
    const UList<T>& field
)
{
    List<T> result(field.size());

    forAll(field, i)
    {
        result[i] = transform(rotTensor, field[i]);
    }

    return result;
}


template<class T>
void transformList(const tensor& rotTensor, UList<T>& field)
{
    forAll(field, i)
    {
        field[i] = transform(rotTensor, field[i]);
    }
}


template<class T>
void transformList(const tensorField& rotTensor, UList<T>& field)
{
    if (rotTensor.size() == 1)
    {
        transformList(rotTensor[0], field);
    }
    else if (rotTensor.size() == field.size())
    {
        forAll(field, i)
        {
            field[i] = transform(rotTensor[i], field[i]);
        }
    }
    else
    {
        FatalErrorInFunction
            << "Sizes of field and transformation not equal. field:"
            << field.size() << " transformation:" << rotTensor.size()
            << abort(FatalError);
    }
}


template<class T>
void transformList(const tensor& rotTensor, Map<T>& field)
{
    forAllIters(field, iter)
    {
        T& value = iter.val();
        value = transform(rotTensor, value);
    }
}


template<class T>
void transformList(const tensorField& rotTensor, Map<T>& field)
{
    if (rotTensor.size() == 1)
    {
        transformList(rotTensor[0], field);
    }
    else
    {
        FatalErrorInFunction
            << "Multiple transformation tensors not supported. field:"
            << field.size() << " transformation:" << rotTensor.size()
            << abort(FatalError);
    }
}


template<class T>
void transformList(const tensor& rotTensor, EdgeMap<T>& field)
{
    forAllIters(field, iter)
    {
        T& value = iter.val();
        value = transform(rotTensor, value);
    }
}


template<class T>
void transformList(const tensorField& rotTensor, EdgeMap<T>& field)
{
    if (rotTensor.size() == 1)
    {
        transformList(rotTensor[0], field);
    }
    else
    {
        FatalErrorInFunction
            << "Multiple transformation tensors not supported. field:"
            << field.size() << " transformation:" << rotTensor.size()
            << abort(FatalError);
    }
}


// ************************************************************************* //

 } // End namespace Foam
