﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
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

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


 namespace Foam{
template<class Type, class GeoMesh>
inline SubDimensionedField<Type, GeoMesh>::SubDimensionedField
(
    const SubField<Type>& sfield
)
:
    SubField<Type>(sfield)
{}


template<class Type, class GeoMesh>
inline SubDimensionedField<Type, GeoMesh>::SubDimensionedField
(
    const UList<Type>& list,
    const label subSize
)
:
    SubField<Type>(list, subSize)
{}


template<class Type, class GeoMesh>
inline SubDimensionedField<Type, GeoMesh>::SubDimensionedField
(
    const UList<Type>& list,
    const label subSize,
    const label startIndex
)
:
    SubField<Type>(list, subSize, startIndex)
{}


template<class Type, class GeoMesh>
inline SubDimensionedField<Type, GeoMesh>::SubDimensionedField
(
    const UList<Type>& list,
    const labelRange& range
)
:
    SubField<Type>(list, range)
{}


template<class Type, class GeoMesh>
inline SubDimensionedField<Type, GeoMesh>::SubDimensionedField
(
    const labelRange& range,
    const UList<Type>& list
)
:
    SubField<Type>(range, list)
{}


template<class Type, class GeoMesh>
inline SubDimensionedField<Type, GeoMesh>::SubDimensionedField
(
    const SubDimensionedField<Type, GeoMesh>& sfield
)
:
    refCount(),
    SubField<Type>(sfield)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type, class GeoMesh>
inline const SubDimensionedField<Type, GeoMesh>&
SubDimensionedField<Type, GeoMesh>::null()
{
    return NullObjectRef<SubDimensionedField<Type, GeoMesh>>();
}


template<class Type, class GeoMesh>
inline
tmp
<
    Field<typename SubDimensionedField<Type, GeoMesh>::cmptType>
>
SubDimensionedField<Type, GeoMesh>::component
(
    const direction d
) const
{
    return
    (
        reinterpret_cast<const DimensionedField<Type, GeoMesh>&>(*this)
    ).component(d);
}


template<class Type, class GeoMesh>
inline tmp<DimensionedField<Type, GeoMesh>>
SubDimensionedField<Type, GeoMesh>::T() const
{
    return
    (
        reinterpret_cast<const DimensionedField<Type, GeoMesh>&>(*this)
    ).T();
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Type, class GeoMesh>
inline void SubDimensionedField<Type, GeoMesh>::operator=
(
    const SubDimensionedField<Type, GeoMesh>& rhs
)
{
    dimensions() = rhs.dimensions();
    SubField<Type>::operator=(rhs);
}


template<class Type, class GeoMesh>
inline SubDimensionedField<Type, GeoMesh>::operator
const DimensionedField<Type, GeoMesh>&() const
{
    return *(reinterpret_cast<const DimensionedField<Type, GeoMesh>*>(this));
}


// ************************************************************************* //

 } // End namespace Foam
