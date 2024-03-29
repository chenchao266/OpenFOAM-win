﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2018-2020 OpenCFD Ltd.
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

// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //


 namespace Foam{
inline predicates::scalars::unary predicates::scalars::operation
(
    const word& opName,
    const scalar opVal,
    const scalar tol
)
{
    return operation(opNames[opName], opVal, tol);
}


inline predicates::scalars::unary predicates::scalars::operation
(
    const Tuple2<word, scalar>& op,
    const scalar tol
)
{
    return operation(opNames[op.first()], op.second(), tol);
}


inline predicates::scalars::unary predicates::scalars::operation
(
    const std::pair<word, scalar>& op,
    const scalar tol
)
{
    return operation(opNames[op.first], op.second, tol);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline bool predicates::scalars::found
(
    const scalar value,
    label pos
) const
{
    return (this->find(value, pos) >= 0);
}


inline bool predicates::scalars::match(const scalar& value) const
{
    return this->matchAny(value);
}


inline bool predicates::scalars::matchAny(const scalar& value) const
{
    for (const unary& test : *this)
    {
        if (test(value))
        {
            return true;
        }
    }

    return false;
}


inline bool predicates::scalars::matchAll(const scalar& value) const
{
    for (const unary& test : *this)
    {
        if (!test(value))
        {
            return false;
        }
    }

    return (!this->empty());
}


inline labelList predicates::scalars::matching
(
    const scalar& value
) const
{
    labelList indices(this->size());

    label i = 0, count = 0;
    for (const unary& test : *this)
    {
        if (test(value))
        {
            indices[count] = i;
            ++count;
        }
        ++i;
    }
    indices.resize(count);

    return indices;
}


inline labelList predicates::scalars::matching
(
    const UList<scalar>& input,
    const bool invert
) const
{
    const label len = input.size();

    labelList indices(len);

    label count = 0;
    for (label i=0; i < len; ++i)
    {
        if (match(input[i]) ? !invert : invert)
        {
            indices[count] = i;
            ++count;
        }
    }
    indices.resize(count);

    return indices;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline bool predicates::scalars::operator()(const scalar& value) const
{
    return this->found(value);
}


// ************************************************************************* //

 } // End namespace Foam
