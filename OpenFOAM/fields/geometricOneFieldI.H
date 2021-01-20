/*---------------------------------------------------------------------------*\
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

#include "geometricOneField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
namespace Foam {
inline const dimensionSet& geometricOneField::dimensions() const
{
    return dimless;
}


inline one geometricOneField::operator[](const label) const
{
    return one();
}


inline oneField geometricOneField::field() const
{
    return oneField();
}


inline oneField geometricOneField::oldTime() const
{
    return oneField();
}


inline geometricOneField::Internal
geometricOneField::operator()() const
{
    return Internal();
}


inline geometricOneField::Internal
geometricOneField::v() const
{
    return Internal();
}


inline typename geometricOneField::Internal::FieldType
geometricOneField::primitiveField() const
{
    return typename Internal::FieldType();
}


inline geometricOneField::Boundary
geometricOneField::boundaryField() const
{
    return Boundary();
}


inline const geometricOneField& operator*
(
    const geometricOneField& gof,
    const geometricOneField&
)
{
    return gof;
}


inline const geometricOneField::Internal& operator*
(
    const geometricOneField::Internal& of,
    const geometricOneField&
)
{
    return of;
}


inline const geometricOneField::Internal& operator*
(
    const geometricOneField&,
    const geometricOneField::Internal& of
)
{
    return of;
}


inline const geometricOneField& operator/
(
    const geometricOneField& gof,
    const geometricOneField&
)
{
    return gof;
}


inline const geometricOneField::Internal& operator/
(
    const geometricOneField::Internal& of,
    const geometricOneField&
)
{
    return of;
}


inline const geometricOneField::Internal& operator/
(
    const geometricOneField&,
    const geometricOneField::Internal& of
)
{
    return of;
}

}
// ************************************************************************* //
