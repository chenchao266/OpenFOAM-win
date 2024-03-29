﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2017-2021 OpenCFD Ltd.
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

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


 namespace Foam{
template<class Type, class GeoMesh>
inline const DimensionedField<Type, GeoMesh>&
DimensionedField<Type, GeoMesh>::null()
{
    return NullObjectRef<DimensionedField<Type, GeoMesh>>();
}


template<class Type, class GeoMesh>
inline const typename GeoMesh::Mesh&
DimensionedField<Type, GeoMesh>::mesh() const
{
    return mesh_;
}


template<class Type, class GeoMesh>
inline const dimensionSet&
DimensionedField<Type, GeoMesh>::dimensions() const
{
    return dimensions_;
}


template<class Type, class GeoMesh>
inline dimensionSet& DimensionedField<Type, GeoMesh>::dimensions()
{
    return dimensions_;
}


template<class Type, class GeoMesh>
inline const orientedType&
DimensionedField<Type, GeoMesh>::oriented() const noexcept
{
    return oriented_;
}


template<class Type, class GeoMesh>
inline orientedType&
DimensionedField<Type, GeoMesh>::oriented() noexcept
{
    return oriented_;
}


template<class Type, class GeoMesh>
inline void DimensionedField<Type, GeoMesh>::setOriented
(
    const bool oriented
) noexcept
{
    oriented_.setOriented(oriented);
}


template<class Type, class GeoMesh>
inline const Field<Type>&
DimensionedField<Type, GeoMesh>::field() const
{
    return *this;
}


template<class Type, class GeoMesh>
inline Field<Type>&
DimensionedField<Type, GeoMesh>::field()
{
    return *this;
}


// ************************************************************************* //

 } // End namespace Foam
