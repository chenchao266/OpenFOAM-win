/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2015 OpenFOAM Foundation
    Copyright (C) 2017 OpenCFD Ltd.
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

#include "IOobject.H"
#include "dictionary2.H"
#include "fvMesh.H"
#include "surfaceMesh.H"
#include "fvPatchFieldMapper.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
Foam::fvsPatchField<Type>::fvsPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, surfaceMesh>& iF
)
:
    Field<Type>(p.size()),
    patch_(p),
    internalField_(iF)
{}


template<class Type>
Foam::fvsPatchField<Type>::fvsPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, surfaceMesh>& iF,
    const Field<Type>& f
)
:
    Field<Type>(f),
    patch_(p),
    internalField_(iF)
{}


template<class Type>
Foam::fvsPatchField<Type>::fvsPatchField
(
    const fvsPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, surfaceMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    Field<Type>(ptf, mapper),
    patch_(p),
    internalField_(iF)
{}


template<class Type>
Foam::fvsPatchField<Type>::fvsPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, surfaceMesh>& iF,
    const dictionary& dict
)
:
    Field<Type>(p.size()),
    patch_(p),
    internalField_(iF)
{
    if (dict.found("value"))
    {
        fvsPatchField<Type>::operator=
        (
            Field<Type>("value", dict, p.size())
        );
    }
    else
    {
        FatalIOErrorInFunction(dict)
            << "Essential entry 'value' missing on patch " << p.name() << nl
            << exit(FatalIOError);
    }
}


template<class Type>
Foam::fvsPatchField<Type>::fvsPatchField(const fvsPatchField<Type>& ptf)
:
    Field<Type>(ptf),
    patch_(ptf.patch_),
    internalField_(ptf.internalField_)
{}


template<class Type>
Foam::fvsPatchField<Type>::fvsPatchField
(
    const fvsPatchField<Type>& ptf,
    const DimensionedField<Type, surfaceMesh>& iF
)
:
    Field<Type>(ptf),
    patch_(ptf.patch_),
    internalField_(iF)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
const Foam::objectRegistry& Foam::fvsPatchField<Type>::db() const
{
    return patch_.boundaryMesh().mesh();
}


template<class Type>
void Foam::fvsPatchField<Type>::check(const fvsPatchField<Type>& ptf) const
{
    if (&patch_ != &(ptf.patch_))
    {
        FatalErrorInFunction
            << "different patches for fvsPatchField<Type>s"
            << abort(FatalError);
    }
}


template<class Type>
void Foam::fvsPatchField<Type>::autoMap(const fvPatchFieldMapper& m)
{
    const bool oriented = internalField_.oriented()();
    Field<Type>::autoMap(m, oriented);
}


template<class Type>
void Foam::fvsPatchField<Type>::rmap
(
    const fvsPatchField<Type>& ptf,
    const labelList& addr
)
{
    Field<Type>::rmap(ptf, addr);
}


template<class Type>
void Foam::fvsPatchField<Type>::write(Ostream& os) const
{
    os.writeEntry("type", type());
    this->writeEntry("value", os);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Type>
void Foam::fvsPatchField<Type>::operator=
(
    const UList<Type>& ul
)
{
    Field<Type>::operator=(ul);
}


template<class Type>
void Foam::fvsPatchField<Type>::operator=
(
    const fvsPatchField<Type>& ptf
)
{
    check(ptf);
    Field<Type>::operator=(ptf);
}


template<class Type>
void Foam::fvsPatchField<Type>::operator+=
(
    const fvsPatchField<Type>& ptf
)
{
    check(ptf);
    Field<Type>::operator+=(ptf);
}


template<class Type>
void Foam::fvsPatchField<Type>::operator-=
(
    const fvsPatchField<Type>& ptf
)
{
    check(ptf);
    Field<Type>::operator-=(ptf);
}


template<class Type>
void Foam::fvsPatchField<Type>::operator*=
(
    const fvsPatchField<scalar>& ptf
)
{
    if (&patch_ != &ptf.patch())
    {
        FatalErrorInFunction
            << "incompatible patches for patch fields"
            << abort(FatalError);
    }

    Field<Type>::operator*=(ptf);
}


template<class Type>
void Foam::fvsPatchField<Type>::operator/=
(
    const fvsPatchField<scalar>& ptf
)
{
    if (&patch_ != &ptf.patch())
    {
        FatalErrorInFunction
            << abort(FatalError);
    }

    Field<Type>::operator/=(ptf);
}


template<class Type>
void Foam::fvsPatchField<Type>::operator+=
(
    const Field<Type>& tf
)
{
    Field<Type>::operator+=(tf);
}


template<class Type>
void Foam::fvsPatchField<Type>::operator-=
(
    const Field<Type>& tf
)
{
    Field<Type>::operator-=(tf);
}


template<class Type>
void Foam::fvsPatchField<Type>::operator*=
(
    const scalarField& tf
)
{
    Field<Type>::operator*=(tf);
}


template<class Type>
void Foam::fvsPatchField<Type>::operator/=
(
    const scalarField& tf
)
{
    Field<Type>::operator/=(tf);
}


template<class Type>
void Foam::fvsPatchField<Type>::operator=
(
    const Type& t
)
{
    Field<Type>::operator=(t);
}


template<class Type>
void Foam::fvsPatchField<Type>::operator+=
(
    const Type& t
)
{
    Field<Type>::operator+=(t);
}


template<class Type>
void Foam::fvsPatchField<Type>::operator-=
(
    const Type& t
)
{
    Field<Type>::operator-=(t);
}


template<class Type>
void Foam::fvsPatchField<Type>::operator*=
(
    const scalar s
)
{
    Field<Type>::operator*=(s);
}


template<class Type>
void Foam::fvsPatchField<Type>::operator/=
(
    const scalar s
)
{
    Field<Type>::operator/=(s);
}


template<class Type>
void Foam::fvsPatchField<Type>::operator==
(
    const fvsPatchField<Type>& ptf
)
{
    Field<Type>::operator=(ptf);
}


template<class Type>
void Foam::fvsPatchField<Type>::operator==
(
    const Field<Type>& tf
)
{
    Field<Type>::operator=(tf);
}


template<class Type>
void Foam::fvsPatchField<Type>::operator==
(
    const Type& t
)
{
    Field<Type>::operator=(t);
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Type>
Foam::Ostream& Foam::operator<<(Ostream& os, const fvsPatchField<Type>& ptf)
{
    ptf.write(os);

    os.check(FUNCTION_NAME);

    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "fvsPatchFieldNew.C"

// ************************************************************************* //
