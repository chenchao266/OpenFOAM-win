﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2019 OpenCFD Ltd.
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

// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //


 namespace Foam{
template<class Cmpt>
void zip
(
    Field<SymmTensor<Cmpt>>& result,
    const UList<Cmpt>& xx, const UList<Cmpt>& xy, const UList<Cmpt>& xz,
    const UList<Cmpt>& yy, const UList<Cmpt>& yz,
    const UList<Cmpt>& zz
)
{
    typedef SymmTensor<Cmpt> value_type;

    const label len = result.size();

    #ifdef FULLDEBUG
    if
    (
        len != xx.size() || len != xy.size() || len != xz.size()
     || len != yy.size() || len != yz.size()
     || len != zz.size()
    )
    {
        FatalErrorInFunction
            << "Components sizes do not match: " << len << " ("
            << xx.size() << ' ' << xy.size() << ' ' << xz.size() << ' '
            << yy.size() << ' ' << yz.size() << ' '
            << zz.size() << ')'
            << nl
            << abort(FatalError);
    }
    #endif

    for (label i=0; i < len; ++i)
    {
        result[i] = value_type
        (
            xx[i], xy[i], xz[i],
            /*yx*/ yy[i], yz[i],
            /*zx   zy */  zz[i]
        );
    }
}


template<class Cmpt>
void unzip
(
    const UList<SymmTensor<Cmpt>>& input,
    Field<Cmpt>& xx, Field<Cmpt>& xy, Field<Cmpt>& xz,
    Field<Cmpt>& yy, Field<Cmpt>& yz,
    Field<Cmpt>& zz
)
{
    const label len = input.size();

    #ifdef FULLDEBUG
    if
    (
        len != xx.size() || len != xy.size() || len != xz.size()
     || len != yy.size() || len != yz.size()
     || len != zz.size()
    )
    {
        FatalErrorInFunction
            << "Components sizes do not match: " << len << " ("
            << xx.size() << ' ' << xy.size() << ' ' << xz.size() << ' '
            << yy.size() << ' ' << yz.size() << ' '
            << zz.size() << ')'
            << nl
            << abort(FatalError);
    }
    #endif

    for (label i=0; i < len; ++i)
    {
        xx[i] = input[i].xx(); xy[i] = input[i].xy(); xz[i] = input[i].xz();
        yy[i] = input[i].yy(); yz[i] = input[i].yz();
        zz[i] = input[i].zz();
    }
}


template<class Cmpt>
tmp<Field<SymmTensor<Cmpt>>>
zip
(
    const Field<Cmpt>& xx, const Field<Cmpt>& xy, const Field<Cmpt>& xz,
    const Field<Cmpt>& yy, const Field<Cmpt>& yz,
    const Field<Cmpt>& zz
)
{
    auto tresult = tmp<Field<SymmTensor<Cmpt>>>::New(xx.size());

    zip(tresult.ref(), xx, xy, xz, yy, yz, zz);

    return tresult;
}


template<class Cmpt>
void unzipDiag
(
    const UList<SymmTensor<Cmpt>>& input,
    Field<Vector<Cmpt>>& result
)
{
    const label len = input.size();

    #ifdef FULLDEBUG
    if (len != result.size())
    {
        FatalErrorInFunction
            << "Components sizes do not match: " << len << " ("
            << result.size() << ')'
            << nl
            << abort(FatalError);
    }
    #endif

    for (label i=0; i < len; ++i)
    {
        result[i] = input[i].diag();
    }
}


template<class Cmpt>
tmp<Field<Vector<Cmpt>>>
unzipDiag
(
    const Field<SymmTensor<Cmpt>>& input
)
{
    auto tresult = tmp<Field<Vector<Cmpt>>>::New(input.size());

    unzipDiag(input, tresult.ref());

    return tresult;
}


// ************************************************************************* //

 } // End namespace Foam
