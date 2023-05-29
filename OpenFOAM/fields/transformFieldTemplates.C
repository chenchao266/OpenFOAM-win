/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
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

#include "transformField.H"
#include "FieldM.H"

// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //


 namespace Foam{
template<class Type>
void transform
(
    Field<Type>& result,
    const tensor& rot,
    const Field<Type>& fld
)
{
    TFOR_ALL_F_OP_FUNC_S_F
    (
        Type, result, =, transform, tensor, rot, Type, fld
    );
}


template<class Type>
void transform
(
    Field<Type>& result,
    const tensorField& rot,
    const Field<Type>& fld
)
{
    if (rot.size() == 1)
    {
        return transform(result, rot.first(), fld);
    }

    TFOR_ALL_F_OP_FUNC_F_F
    (
        Type, result, =, transform, tensor, rot, Type, fld
    );
}


template<class Type>
tmp<Field<Type>>
transform
(
    const tensorField& rot,
    const Field<Type>& fld
)
{
    auto tresult = tmp<Field<Type>>::New(fld.size());
    transform(tresult.ref(), rot, fld);
    return tresult;
}


template<class Type>
tmp<Field<Type>>
transform
(
    const tensorField& rot,
    const tmp<Field<Type>>& tfld
)
{
    tmp<Field<Type>> tresult = New(tfld);
    transform(tresult.ref(), rot, tfld());
    tfld.clear();
    return tresult;
}


template<class Type>
tmp<Field<Type>>
transform
(
    const tmp<tensorField>& trot,
    const Field<Type>& fld
)
{
    auto tresult = tmp<Field<Type>>::New(fld.size());
    transform(tresult.ref(), trot(), fld);
    trot.clear();
    return tresult;
}


template<class Type>
tmp<Field<Type>>
transform
(
    const tmp<tensorField>& trot,
    const tmp<Field<Type>>& tfld
)
{
    tmp<Field<Type>> tresult = New(tfld);
    transform(tresult.ref(), trot(), tfld());
    trot.clear();
    tfld.clear();
    return tresult;
}


template<class Type>
tmp<Field<Type>>
transform
(
    const tensor& rot,
    const Field<Type>& fld
)
{
    auto tresult = tmp<Field<Type>>::New(fld.size());
    transform(tresult.ref(), rot, fld);
    return tresult;
}


template<class Type>
tmp<Field<Type>>
transform
(
    const tensor& rot,
    const tmp<Field<Type>>& tfld
)
{
    tmp<Field<Type>> tresult = New(tfld);
    transform(tresult.ref(), rot, tfld());
    tfld.clear();
    return tresult;
}


template<class Type>
void invTransform
(
    Field<Type>& result,
    const tensor& rot,
    const Field<Type>& fld
)
{
    TFOR_ALL_F_OP_FUNC_S_F
    (
        Type, result, =, invTransform, tensor, rot, Type, fld
    );
}


template<class Type>
void invTransform
(
    Field<Type>& result,
    const tensorField& rot,
    const Field<Type>& fld
)
{
    if (rot.size() == 1)
    {
        return invTransform(result, rot.first(), fld);
    }

    TFOR_ALL_F_OP_FUNC_F_F
    (
        Type, result, =, invTransform, tensor, rot, Type, fld
    );
}


template<class Type>
tmp<Field<Type>>
invTransform
(
    const tensorField& rot,
    const Field<Type>& fld
)
{
    auto tresult = tmp<Field<Type>>::New(fld.size());
    invTransform(tresult.ref(), rot, fld);
    return tresult;
}


template<class Type>
tmp<Field<Type>>
invTransform
(
    const tensorField& rot,
    const tmp<Field<Type>>& tfld
)
{
    tmp<Field<Type>> tresult = New(tfld);
    invTransform(tresult.ref(), rot, tfld());
    tfld.clear();
    return tresult;
}


template<class Type>
tmp<Field<Type>>
invTransform
(
    const tmp<tensorField>& trot,
    const Field<Type>& fld
)
{
    auto tresult = tmp<Field<Type>>::New(fld.size());
    invTransform(tresult.ref(), trot(), fld);
    trot.clear();
    return tresult;
}


template<class Type>
tmp<Field<Type>>
invTransform
(
    const tmp<tensorField>& trot,
    const tmp<Field<Type>>& tfld
)
{
    tmp<Field<Type>> tresult = New(tfld);
    invTransform(tresult.ref(), trot(), tfld());
    trot.clear();
    tfld.clear();
    return tresult;
}


template<class Type>
tmp<Field<Type>>
invTransform
(
    const tensor& rot,
    const Field<Type>& fld
)
{
    auto tresult = tmp<Field<Type>>::New(fld.size());
    invTransform(tresult.ref(), rot, fld);
    return tresult;
}


template<class Type>
tmp<Field<Type>>
invTransform
(
    const tensor& rot,
    const tmp<Field<Type>>& tfld
)
{
    tmp<Field<Type>> tresult = New(tfld);
    invTransform(tresult.ref(), rot, tfld());
    tfld.clear();
    return tresult;
}


template<class Type1, class Type2>
tmp<Field<Type1>>
transformFieldMask(const Field<Type2>& fld)
{
    return fld;
}

template<class Type1, class Type2>
tmp<Field<Type1>>
transformFieldMask(const tmp<Field<Type2>>& tfld)
{
    return tmp<Field<Type1>>(tfld.ptr());
}


// ************************************************************************* //

 } // End namespace Foam
