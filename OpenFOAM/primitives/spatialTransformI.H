/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2016 OpenFOAM Foundation
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

#include "transform.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //
namespace Foam {
    tensor spatialTransform::Erx() const
    {
        return E_ & *r_;
    }


    // * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

    inline spatialTransform::spatialTransform() : E_(tensor::I),
        r_(Zero)
    {}


    inline spatialTransform::spatialTransform
    (
        const tensor& E,
        const vector& r
    ) : E_(E),
        r_(r)
    {}


    inline spatialTransform::spatialTransform(Istream& is) : E_(is),
        r_(is)
    {}


    inline spatialTransform::transpose::transpose(const spatialTransform& X) : X_(X)
    {}


    inline spatialTransform::dual::dual(const spatialTransform& X) : X_(X)
    {}


    // * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

    inline const tensor& spatialTransform::E() const
    {
        return E_;
    }

    inline tensor& spatialTransform::E()
    {
        return E_;
    }

    inline const vector& spatialTransform::r() const
    {
        return r_;
    }

    inline vector& spatialTransform::r()
    {
        return r_;
    }


    inline spatialTransform::transpose spatialTransform::T() const
    {
        return transpose(*this);
    }


    inline spatialTransform spatialTransform::inv() const
    {
        return spatialTransform(E_.T(), -(E_ & r_));
    }


    // * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

    inline spatialTransform::dual spatialTransform::operator*() const
    {
        return dual(*this);
    }


    inline spatialTransform::operator spatialTensor() const
    {
        return spatialTensor
        (
            E_, Zero,
            -Erx(), E_
        );
    }


    inline void spatialTransform::operator&=(const spatialTransform& X)
    {
        E_ &= X.E_;
        r_ = X.r_ + (r_ & X.E_);
    }


    inline spatialTransform spatialTransform::operator&
        (
            const spatialTransform& X
            ) const
    {
        return spatialTransform(E_ & X.E_, X.r_ + (r_ & X.E_));
    }


    inline spatialVector spatialTransform::operator&
        (
            const spatialVector& v
            ) const
    {
        return spatialVector
        (
            E_ & v.w(),
            E_ & (v.l() - (r_ ^ v.w()))
        );
    }


    inline spatialVector spatialTransform::operator&&
        (
            const spatialVector& v
            ) const
    {
        return spatialVector
        (
            E_ & v.w(),
            E_ & (v.l() - r_)
        );
    }


    inline vector spatialTransform::transformPoint
    (
        const vector& p
    ) const
    {
        return E_ & (p - r_);
    }


    inline spatialTransform::transpose::operator spatialTensor() const
    {
        return spatialTensor
        (
            X_.E().T(), -X_.Erx().T(),
            Zero, X_.E().T()
        );
    }


    inline spatialVector spatialTransform::transpose::operator&
        (
            const spatialVector& f
            ) const
    {
        vector ETfl(X_.E().T() & f.l());

        return spatialVector
        (
            (X_.E().T() & f.w()) + (X_.r() ^ ETfl),
            ETfl
        );
    }


    inline spatialTransform::dual::operator spatialTensor() const
    {
        return spatialTensor
        (
            X_.E(), -X_.Erx(),
            Zero, X_.E()
        );
    }


    inline spatialVector spatialTransform::dual::operator&
        (
            const spatialVector& f
            ) const
    {
        return spatialVector
        (
            X_.E() & (f.w() - (X_.r() ^ f.l())),
            X_.E() & f.l()
        );
    }


    // * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * * //

    inline Istream& operator>>(Istream& is, spatialTransform& X)
    {
        is >> X.E() >> X.r();
        return is;
    }


    inline Ostream& operator<<
        (
            Ostream& os,
            const spatialTransform& X
            )
    {
        os << X.E() << token::SPACE << X.r();
        return os;
    }

}
// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//- Rotational spatial transformation tensor about the x-axis by omega radians
inline spatialTransform Xrx(const scalar& omega)
{
    return spatialTransform(Rx(omega), Zero);
}

//- Rotational spatial transformation tensor about the x-axis by omega radians
inline spatialTransform Xry(const scalar& omega)
{
    return spatialTransform(Ry(omega), Zero);
}

//- Rotational spatial transformation tensor about the z-axis by omega radians
inline spatialTransform Xrz(const scalar& omega)
{
    return spatialTransform(Rz(omega), Zero);
}

//- Rotational spatial transformation tensor about axis a by omega radians
inline spatialTransform Xr(const vector& a, const scalar omega)
{
    return spatialTransform(Ra(a, omega), Zero);
}

//- Translational spatial transformation tensor for translation r
inline spatialTransform Xt(const vector& r)
{
    return spatialTransform(tensor::I, r);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
