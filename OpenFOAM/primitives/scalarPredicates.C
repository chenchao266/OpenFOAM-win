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

#include "scalarPredicates.H"
#include "HashSet.H"
#include "FlatOutput.H"
#include "Tuple2.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


 namespace Foam{
const Enum
<
    predicates::scalars::opType
>
predicates::scalars::opNames
({
    { opType::EQUAL, "eq" },
    { opType::EQUAL, "equal" },
    { opType::NOT_EQUAL,  "neq" },
    { opType::NOT_EQUAL,  "notEqual" },
    { opType::LESS, "lt" },
    { opType::LESS, "less" },
    { opType::LESS_EQ, "le" },
    { opType::LESS_EQ, "lessEq" },
    { opType::GREATER, "gt" },
    { opType::GREATER, "greater" },
    { opType::GREATER_EQ, "ge" },
    { opType::GREATER_EQ, "greaterEq" },
});


// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

std::function<bool(scalar)> predicates::scalars::operation
(
    const enum predicates::scalars::opType op,
    const scalar opVal,
    const scalar tol
)
{
    switch (op)
    {
        case opType::EQUAL:
            return equalOp(opVal, tol);
            break;
        case opType::NOT_EQUAL:
            return notEqualOp(opVal, tol);
            break;
        case opType::LESS:
            return lessOp(opVal);
            break;
        case opType::LESS_EQ:
            return lessEqOp(opVal);
            break;
        case opType::GREATER:
            return greaterOp(opVal);
            break;
        case opType::GREATER_EQ:
            return greaterEqOp(opVal);
            break;
        case opType::ALWAYS:
            return trueOp();
            break;
        default:
            break;
    }

    return falseOp();
}


// * * * * * * * * * * * * * * * Local Functions * * * * * * * * * * * * * * //


 } // End namespace Foam
namespace Foam
{
    // Check for bad/unknown operations
    template<class Container, class Get0>
    static bool hasBadEntries
    (
        const Container& entries,
        const Get0& get0
    )
    {
        for (const auto& entry : entries)
        {
            if (!predicates::scalars::opNames.found(get0(entry)))
            {
                return true;
            }
        }

        return false;
    }


    // Print bad/unknown operations
    template<class Error, class Container, class Get0, class Get1>
    static Error& printBadEntries
    (
        Error& err,
        const Container& entries,
        const Get0& get0,
        const Get1& get1
    )
    {
        labelHashSet badIndices;

        label idx = 0;

        for (const auto& entry : entries)
        {
            if (!predicates::scalars::opNames.found(get0(entry)))
            {
                badIndices.insert(idx);
            }
            ++idx;
        }

        err
            << "Entries with unknown operations:" << nl
            << idx << nl
            << '(' << nl;

        idx = 0;
        for (const auto& entry : entries)
        {
            const bool bad = badIndices.found(idx);
            ++idx;

            if (bad)
            {
                err << ">>> ";
            }
            else
            {
                err << "    ";
            }
            err << '(' << get0(entry) << ' ' << get1(entry) << ')';

            if (bad)
            {
                err << " <<<";
            }
            err << nl;
        }

        err << ')' << nl;

        return err;
    }

} // End namespace Foam


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


 namespace Foam{
predicates::scalars::scalars
(
    std::initializer_list<std::pair<word, scalar>> entries
)
:
    List<unary>(entries.size())
{
    // Access
    const auto get0 =
        [](const std::pair<word,scalar>& entry) { return entry.first; };
    const auto get1 =
        [](const std::pair<word,scalar>& entry) { return entry.second; };

    // Check for bad/unknown operations
    if (hasBadEntries(entries, get0))
    {
        printBadEntries(FatalErrorInFunction, entries, get0, get1)
            << exit(FatalError);
    }

    // Appears to be good
    auto& list = *this;
    label idx = 0;
    for (const auto& entry : entries)
    {
        list[idx] = predicates::scalars::operation(entry);
        ++idx;
    }
}


predicates::scalars::scalars
(
    const UList<Tuple2<word, scalar>>& entries
)
:
    List<unary>(entries.size())
{
    // Access
    const auto get0 =
        [](const Tuple2<word,scalar>& entry) { return entry.first(); };
    const auto get1 =
        [](const Tuple2<word,scalar>& entry) { return entry.second(); };

    // Check for bad/unknown operations
    if (hasBadEntries(entries, get0))
    {
        printBadEntries(FatalErrorInFunction, entries, get0, get1)
            << exit(FatalError);
    }

    // Appears to be good
    auto& list = *this;
    label idx = 0;
    for (const auto& entry : entries)
    {
        list[idx] = predicates::scalars::operation(entry);
        ++idx;
    }
}


predicates::scalars::scalars(Istream& is)
:
    List<unary>()
{
    is >> *this;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

label predicates::scalars::find
(
    const scalar value,
    label pos
) const
{
    const label len = this->size();

    if (pos >= 0 && len)
    {
        // auto iter = this->cbegin();

        while (pos < len)
        {
            if ((*this)[pos](value))
            {
                return pos;
            }

            ++pos;
        }
    }

    return -1;
}


label predicates::scalars::rfind
(
    const scalar value,
    label pos
) const
{
    // pos == -1 has same meaning as std::string::npos - search from end
    if (pos < 0 || pos >= this->size())
    {
        pos = this->size()-1;
    }

    while (pos >= 0)
    {
        if ((*this)[pos](value))
        {
            return pos;
        }

        --pos;
    }

    return -1;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

Istream& operator>>(Istream& is, predicates::scalars& list)
{
    // Read tuples
    List<Tuple2<word, scalar>> entries(is);

    // Access
    const auto get0 =
        [](const Tuple2<word,scalar>& entry) { return entry.first(); };
    const auto get1 =
        [](const Tuple2<word,scalar>& entry) { return entry.second(); };


    // Check for bad/unknown operations
    if (hasBadEntries(entries, get0))
    {
        printBadEntries(FatalIOErrorInFunction(is), entries, get0, get1)
            << exit(FatalIOError);
    }

    // Appears to be good
    list.resize(entries.size());

    label idx = 0;
    for (const Tuple2<word, scalar>& entry : entries)
    {
        list[idx] = predicates::scalars::operation(entry);
        ++idx;
    }

    return is;
}


// ************************************************************************* //

 } // End namespace Foam
