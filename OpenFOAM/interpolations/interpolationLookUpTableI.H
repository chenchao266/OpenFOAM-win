/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011 OpenFOAM Foundation
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


 namespace Foam{
template<class Type>
inline label
interpolationLookUpTable<Type>::findFieldIndex
(
    const word& fieldName
) const
{
    return fieldIndices_[fieldName];
}


template<class Type>
inline const List<dictionary>&
interpolationLookUpTable<Type>::output() const
{
    return output_;
}


template<class Type>
inline const List<dictionary>&
interpolationLookUpTable<Type>::entries() const
{
    return entries_;
}


template<class Type>
inline const List<scalar>&
interpolationLookUpTable<Type>::min() const
{
    return min_;
}


template<class Type>
inline const List<label>&
interpolationLookUpTable<Type>::dim() const
{
    return dim_;
}


template<class Type>
inline const List<scalar>&
interpolationLookUpTable<Type>::delta() const
{
    return delta_;
}


template<class Type>
inline const List<scalar>&
interpolationLookUpTable<Type>::max() const
{
    return max_;
}


template<class Type>
inline word interpolationLookUpTable<Type>::tableName() const
{
    return fileName_.name();
}


// ************************************************************************* //

 } // End namespace Foam
