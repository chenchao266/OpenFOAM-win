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

#include "vectorTensorTransform.H"
#include "IOstreams.H"
#include "OStringStream.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //
namespace Foam {
    const char* const vectorTensorTransform::typeName =
        "vectorTensorTransform";

    const vectorTensorTransform vectorTensorTransform::_zero
    (
        Zero,
        Zero,
        false
    );


    const vectorTensorTransform vectorTensorTransform::I
    (
        Zero,
        sphericalTensor::I,
        false
    );


    // * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

    vectorTensorTransform::vectorTensorTransform(Istream& is)
    {
        is >> *this;
    }


    // * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

    word name(const vectorTensorTransform& s)
    {
        OStringStream buf;

        buf << '(' << s.t() << ',' << s.R() << ')';

        return buf.str();
    }


    template<>
    tmp<Field<bool>> vectorTensorTransform::transform
    (
        const Field<bool>& fld
    ) const
    {
        return fld;
    }
    template<>
    tmp<Field<label>> vectorTensorTransform::transform
    (
        const Field<label>& fld
    ) const
    {
        return fld;
    }
    template<>
    tmp<Field<scalar>> vectorTensorTransform::transform
    (
        const Field<scalar>& fld
    ) const
    {
        return fld;
    }


    // * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

    Istream& operator>>(Istream& is, vectorTensorTransform& tr)
    {
        // Read beginning of vectorTensorTransform
        is.readBegin("vectorTensorTransform");

        is >> tr.t_ >> tr.R_ >> tr.hasR_;

        // Read end of vectorTensorTransform
        is.readEnd("vectorTensorTransform");

        // Check state of Istream
        is.check("operator>>(Istream&, vectorTensorTransform&)");

        return is;
    }


    Ostream& operator<<(Ostream& os, const vectorTensorTransform& tr)
    {
        os << token::BEGIN_LIST
            << tr.t() << token::SPACE << tr.R() << token::SPACE << tr.hasR()
            << token::END_LIST;

        return os;
    }

}
// ************************************************************************* //
