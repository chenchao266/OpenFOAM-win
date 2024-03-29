﻿/*---------------------------------------------------------------------------*\
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
#include "diagTensor2.H"

// * * * * * * * * * * * * * * * global functions  * * * * * * * * * * * * * //


 namespace Foam{
void transform
(
    vectorField& rtf,
    const quaternion& q,
    const vectorField& tf
)
{
    tensor rot = q.R();
    TFOR_ALL_F_OP_FUNC_S_F(vector, rtf, =, transform, tensor, rot, vector, tf)
}


tmp<vectorField> transform
(
    const quaternion& q,
    const vectorField& fld
)
{
    auto tresult = tmp<vectorField>::New(fld.size());
    transform(tresult.ref(), q, fld);
    return tresult;
}


tmp<vectorField> transform
(
    const quaternion& q,
    const tmp<vectorField>& tfld
)
{
    tmp<vectorField> tresult = New(tfld);
    transform(tresult.ref(), q, tfld());
    tfld.clear();
    return tresult;
}


void transformPoints
(
    vectorField& result,
    const septernion& tr,
    const vectorField& fld
)
{
    vector trans = tr.t();

    // Check if any translation
    if (mag(trans) > VSMALL)
    {
        TFOR_ALL_F_OP_F_OP_S(vector, result, =, vector, fld, -, vector, trans);
    }
    else
    {
        result = fld;
    }

    // Check if any rotation
    if (mag(tr.r().R() - I) > SMALL)
    {
        transform(result, tr.r(), result);
    }
}


tmp<vectorField> transformPoints
(
    const septernion& tr,
    const vectorField& fld
)
{
    auto tresult = tmp<vectorField>::New(fld.size());
    transformPoints(tresult.ref(), tr, fld);
    return tresult;
}


tmp<vectorField> transformPoints
(
    const septernion& tr,
    const tmp<vectorField>& tfld
)
{
    tmp<vectorField> tresult = New(tfld);
    transformPoints(tresult.ref(), tr, tfld());
    tfld.clear();
    return tresult;
}


// ************************************************************************* //

 } // End namespace Foam
