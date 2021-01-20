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

//#include "DynamicField.Ty.H"

// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //
namespace Foam {
    template<class Ty, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
    DynamicField<Ty, SizeInc, SizeMult, SizeDiv>::DynamicField(Istream& is)
        :
        Field<Ty>(is),
        capacity_(Field<Ty>::size())
    {}


    template<class Ty, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
    tmp<DynamicField<Ty, SizeInc, SizeMult, SizeDiv>>
        DynamicField<Ty, SizeInc, SizeMult, SizeDiv>::clone() const
    {
        return tmp<DynamicField<Ty, SizeInc, SizeMult, SizeDiv>>
            (
                new DynamicField<Ty, SizeInc, SizeMult, SizeDiv>(*this)
                );
    }


    // * * * * * * * * * * * * * * * IOstream Operator * * * * * * * * * * * * * //

    template<class Ty, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
    Ostream& operator<<
        (
            Ostream& os,
            const DynamicField<Ty, SizeInc, SizeMult, SizeDiv>& lst
            )
    {
        os << static_cast<const Field<Ty>&>(lst);
        return os;
    }


    template<class Ty, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
    Istream& operator>>
        (
            Istream& is,
            DynamicField<Ty, SizeInc, SizeMult, SizeDiv>& lst
            )
    {
        is >> static_cast<Field<Ty>&>(lst);
        lst.capacity_ = lst.Field<Ty>::size();

        return is;
    }

}
// ************************************************************************* //
