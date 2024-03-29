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
    Field<Tensor<Cmpt>>& result,
    const UList<Cmpt>& xx, const UList<Cmpt>& xy, const UList<Cmpt>& xz,
    const UList<Cmpt>& yx, const UList<Cmpt>& yy, const UList<Cmpt>& yz,
    const UList<Cmpt>& zx, const UList<Cmpt>& zy, const UList<Cmpt>& zz
)
{
    typedef Tensor<Cmpt> value_type;

    const label len = result.size();

    #ifdef FULLDEBUG
    if
    (
        len != xx.size() || len != xy.size() || len != xz.size()
     || len != yx.size() || len != yy.size() || len != yz.size()
     || len != zx.size() || len != zy.size() || len != zz.size()
    )
    {
        FatalErrorInFunction
            << "Components sizes do not match: " << len << " ("
            << xx.size() << ' ' << xy.size() << ' ' << xz.size() << ' '
            << yx.size() << ' ' << yy.size() << ' ' << yz.size() << ' '
            << zx.size() << ' ' << zy.size() << ' ' << zz.size() << ')'
            << nl
            << abort(FatalError);
    }
    #endif

    for (label i=0; i < len; ++i)
    {
        result[i] = value_type
        (
            xx[i], xy[i], xz[i],
            yx[i], yy[i], yz[i],
            zx[i], zy[i], zz[i]
        );
    }
}


template<class Cmpt>
void unzip
(
    const UList<Tensor<Cmpt>>& input,
    Field<Cmpt>& xx, Field<Cmpt>& xy, Field<Cmpt>& xz,
    Field<Cmpt>& yx, Field<Cmpt>& yy, Field<Cmpt>& yz,
    Field<Cmpt>& zx, Field<Cmpt>& zy, Field<Cmpt>& zz
)
{
    const label len = input.size();

    #ifdef FULLDEBUG
    if
    (
        len != xx.size() || len != xy.size() || len != xz.size()
     || len != yx.size() || len != yy.size() || len != yz.size()
     || len != zx.size() || len != zy.size() || len != zz.size()
    )
    {
        FatalErrorInFunction
            << "Components sizes do not match: " << len << " ("
            << xx.size() << ' ' << xy.size() << ' ' << xz.size() << ' '
            << yx.size() << ' ' << yy.size() << ' ' << yz.size() << ' '
            << zx.size() << ' ' << zy.size() << ' ' << zz.size() << ')'
            << nl
            << abort(FatalError);
    }
    #endif

    for (label i=0; i < len; ++i)
    {
        xx[i] = input[i].xx(); xy[i] = input[i].xy(); xz[i] = input[i].xz();
        yx[i] = input[i].yx(); yy[i] = input[i].yy(); yz[i] = input[i].yz();
        zx[i] = input[i].zx(); zy[i] = input[i].zy(); zz[i] = input[i].zz();
    }
}


template<class Cmpt>
tmp<Field<Tensor<Cmpt>>>
zip
(
    const Field<Cmpt>& xx, const Field<Cmpt>& xy, const Field<Cmpt>& xz,
    const Field<Cmpt>& yx, const Field<Cmpt>& yy, const Field<Cmpt>& yz,
    const Field<Cmpt>& zx, const Field<Cmpt>& zy, const Field<Cmpt>& zz
)
{
    auto tresult = tmp<Field<Tensor<Cmpt>>>::New(xx.size());

    zip(tresult.ref(), xx, xy, xz, yx, yy, yz, zx, zy, zz);

    return tresult;
}


template<class Cmpt>
void zipRows
(
    Field<Tensor<Cmpt>>& result,
    const UList<Vector<Cmpt>>& x,
    const UList<Vector<Cmpt>>& y,
    const UList<Vector<Cmpt>>& z
)
{
    const label len = result.size();

    #ifdef FULLDEBUG
    if (len != x.size() || len != y.size() || len != z.size())
    {
        FatalErrorInFunction
            << "Components sizes do not match: " << len << " ("
            << x.size() << ' ' << y.size() << ' ' << z.size() << ')'
            << nl
            << abort(FatalError);
    }
    #endif

    for (label i=0; i < len; ++i)
    {
        result[i].rows(x[i], y[i], z[i]);
    }
}


template<class Cmpt>
void zipCols
(
    Field<Tensor<Cmpt>>& result,
    const UList<Vector<Cmpt>>& x,
    const UList<Vector<Cmpt>>& y,
    const UList<Vector<Cmpt>>& z
)
{
    const label len = result.size();

    #ifdef FULLDEBUG
    if (len != x.size() || len != y.size() || len != z.size())
    {
        FatalErrorInFunction
            << "Components sizes do not match: " << len << " ("
            << x.size() << ' ' << y.size() << ' ' << z.size() << ')'
            << nl
            << abort(FatalError);
    }
    #endif

    for (label i=0; i < len; ++i)
    {
        result[i].cols(x[i], y[i], z[i]);
    }
}


template<class Cmpt>
void unzipRows
(
    const UList<Tensor<Cmpt>>& input,
    Field<Vector<Cmpt>>& x,
    Field<Vector<Cmpt>>& y,
    Field<Vector<Cmpt>>& z
)
{
    const label len = input.size();

    #ifdef FULLDEBUG
    if (len != x.size() || len != y.size() || len != z.size())
    {
        FatalErrorInFunction
            << "Components sizes do not match: " << len << " ("
            << x.size() << ' ' << y.size() << ' ' << z.size() << ')'
            << nl
            << abort(FatalError);
    }
    #endif

    for (label i=0; i < len; ++i)
    {
        x[i] = input[i].x();
        y[i] = input[i].y();
        z[i] = input[i].z();
    }
}


template<class Cmpt>
void unzipCols
(
    const UList<Tensor<Cmpt>>& input,
    Field<Vector<Cmpt>>& x,
    Field<Vector<Cmpt>>& y,
    Field<Vector<Cmpt>>& z
)
{
    const label len = input.size();

    #ifdef FULLDEBUG
    if (len != x.size() || len != y.size() || len != z.size())
    {
        FatalErrorInFunction
            << "Components sizes do not match: " << len << " ("
            << x.size() << ' ' << y.size() << ' ' << z.size() << ')'
            << nl
            << abort(FatalError);
    }
    #endif

    for (label i=0; i < len; ++i)
    {
        x[i] = input[i].cx();
        y[i] = input[i].cy();
        z[i] = input[i].cz();
    }
}


template<class Cmpt>
void unzipRow
(
    const UList<Tensor<Cmpt>>& input,
    const vector::components cmpt,
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

    switch (cmpt)
    {
        case vector::components::X :
        {
            for (label i=0; i < len; ++i)
            {
                result[i] = input[i].x();
            }
        }
        break;

        case vector::components::Y :
        {
            for (label i=0; i < len; ++i)
            {
                result[i] = input[i].y();
            }
        }
        break;

        case vector::components::Z :
        {
            for (label i=0; i < len; ++i)
            {
                result[i] = input[i].z();
            }
        }
        break;
    }
}


template<class Cmpt>
void unzipCol
(
    const UList<Tensor<Cmpt>>& input,
    const vector::components cmpt,
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

    switch (cmpt)
    {
        case vector::components::X :
        {
            for (label i=0; i < len; ++i)
            {
                result[i] = input[i].cx();
            }
        }
        break;

        case vector::components::Y :
        {
            for (label i=0; i < len; ++i)
            {
                result[i] = input[i].cy();
            }
        }
        break;

        case vector::components::Z :
        {
            for (label i=0; i < len; ++i)
            {
                result[i] = input[i].cz();
            }
        }
        break;
    }
}


template<class Cmpt>
void unzipDiag
(
    const UList<Tensor<Cmpt>>& input,
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
unzipRow
(
    const Field<Tensor<Cmpt>>& input,
    const vector::components cmpt
)
{
    auto tresult = tmp<Field<Vector<Cmpt>>>::New(input.size());

    unzipRow(input, cmpt, tresult.ref());

    return tresult;
}


template<class Cmpt>
tmp<Field<Vector<Cmpt>>>
unzipCol
(
    const Field<Tensor<Cmpt>>& input,
    const vector::components cmpt
)
{
    auto tresult = tmp<Field<Vector<Cmpt>>>::New(input.size());

    unzipCol(input, cmpt, tresult.ref());

    return tresult;
}


template<class Cmpt>
tmp<Field<Vector<Cmpt>>>
unzipDiag
(
    const Field<Tensor<Cmpt>>& input
)
{
    auto tresult = tmp<Field<Vector<Cmpt>>>::New(input.size());

    unzipDiag(input, tresult.ref());

    return tresult;
}


// ************************************************************************* //

 } // End namespace Foam
