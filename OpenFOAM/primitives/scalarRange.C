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

#include "scalarRange.H"
#include "_string.H"
#include "Switch.H"
#include "MinMax.H"
#include "error.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


 namespace Foam{
const scalarRange scalarRange::always
(
    scalarRange::ALWAYS,
    -GREAT,
    GREAT
);


// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

bool scalarRange::parse(const std::string& str, scalarRange& range)
{
    range.clear();

    const auto colon = str.find(':');

    if (colon == std::string::npos)
    {
        // No colon

        // Use Switch to accept none/true/false.
        // Others like (f|n|t|y) and (on|off|yes|no) are not really
        // appropriate, but don't worry about that now.

        if (str.size() >= 4)
        {
            Switch sw = Switch::find(str);

            if (sw.good())
            {
                if (sw)
                {
                    range = scalarRange::always;
                }

                return true; // parsed ok
            }
        }

        // "VALUE"
        scalar val;
        if (readScalar(str, val))
        {
            range = scalarRange(val);
        }
    }
    else if (str[colon+1] == ':')
    {
        // A double colon ("::") is a syntax error
        return false;
    }
    else if (colon == 0)
    {
        // ":MAX"
        scalar val;
        if (readScalar(str.substr(1), val))
        {
            range = scalarRange::le(val);
        }
    }
    else if (colon == str.size()-1)
    {
        // "MIN:"
        scalar val;
        if (readScalar(str.substr(0, colon), val))
        {
            range = scalarRange::ge(val);
        }
    }
    else
    {
        // "MIN:MAX"
        scalar minVal, maxVal;
        if
        (
            readScalar(str.substr(0, colon), minVal)
         && readScalar(str.substr(colon+1), maxVal)
        )
        {
            range = scalarRange(minVal, maxVal);
        }
    }

    return range.valid();
}


scalarRange scalarRange::parse(const std::string& str)
{
    scalarRange range;

    if (!parse(str, range))
    {
        Info<< "Bad scalar-range while parsing: " << str << endl;
    }

    return range;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

scalarRange::scalarRange(const MinMax<label>& range) noexcept
:
    min_(range.min()),
    max_(range.max()),
    type_(max_ < min_ ? scalarRange::NONE : scalarRange::GE_LE)
{}


scalarRange::scalarRange(const MinMax<scalar>& range) noexcept
:
    min_(range.min()),
    max_(range.max()),
    type_(max_ < min_ ? scalarRange::NONE : scalarRange::GE_LE)
{}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

Ostream& operator<<(Ostream& os, const scalarRange& range)
{
    switch (range.type_)
    {
        case scalarRange::EQ:
            os << range.min();
            break;

        case scalarRange::GE:
        case scalarRange::GT:
            os << range.min() << ":Inf";
            break;

        case scalarRange::LE:
        case scalarRange::LT:
            os << "-Inf:" << range.max();
            break;

        case scalarRange::GE_LE:
            os << range.min() << ':' << range.max();
            break;

        case scalarRange::ALWAYS:
            os << "true";
            break;

        default:
            os << "none";
            break;
    }

    return os;
}


// ************************************************************************* //

 } // End namespace Foam
