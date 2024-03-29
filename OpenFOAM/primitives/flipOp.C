﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2015-2016 OpenFOAM Foundation
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

#include "flipOp.H"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


 namespace Foam{
template<> scalar flipOp::operator()(const scalar& v) const
{
    return -v;
}


template<> vector flipOp::operator()(const vector& v) const
{
    return -v;
}


template<> sphericalTensor flipOp::operator()
(
    const sphericalTensor& v
) const
{
    return -v;
}


template<> symmTensor flipOp::operator()
(
    const symmTensor& v
) const
{
    return -v;
}


template<> tensor flipOp::operator()(const tensor& v) const
{
    return -v;
}


template<> triad flipOp::operator()
(
    const triad& v
) const
{
    return -v;
}


// ************************************************************************* //

 } // End namespace Foam
