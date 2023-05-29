/*---------------------------------------------------------------------------*\
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

// * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * * //


 namespace Foam{
template<class T>
inline const T SVD::sign(const T& a, const T& b)
{
    //return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a);
    return b >= 0 ? a : -a;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const scalarRectangularMatrix& SVD::U() const
{
    return U_;
}


inline const scalarRectangularMatrix& SVD::V() const
{
    return V_;
}


inline const scalarDiagonalMatrix& SVD::S() const
{
    return S_;
}


inline bool SVD::converged() const
{
    return converged_;
}


inline label SVD::nZeros() const
{
    return nZeros_;
}


inline scalar SVD::minNonZeroS() const
{
    scalar minS = S_[0];
    for (label i=1; i<S_.size(); i++)
    {
        scalar s = S_[i];
        if (s > VSMALL && s < minS) minS = s;
    }
    return minS;
}


// ************************************************************************* //

 } // End namespace Foam
