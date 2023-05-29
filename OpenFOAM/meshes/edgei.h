﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011 OpenFOAM Foundation
    Copyright (C) 2017-2020 OpenCFD Ltd.
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

#include "IOstreams.H"

// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //


 namespace Foam{
inline int edge::compare(const edge& a, const edge& b)
{
    return labelPair::compare(a, b);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline edge::edge()
:
    labelPair(-1, -1)
{}


inline edge::edge(const label from, const label to)
:
    labelPair(from, to)
{}


inline edge::edge(const labelPair& pair)
:
    labelPair(pair.first(), pair.second())
{}


inline edge::edge(const FixedList<label, 2>& list)
:
    labelPair(list.first(), list.last())
{}


inline edge::edge(const label from, const label to, const bool doSort)
:
    labelPair(from, to, doSort)
{}


inline edge::edge(const FixedList<label, 2>& list, const bool doSort)
:
    labelPair(list, doSort)
{}


inline edge::edge
(
    const labelUList& list,
    const FixedList<label, 2>& indices
)
:
    labelPair(list[indices.first()], list[indices.last()])
{}


inline edge::edge(Istream& is)
:
    labelPair(is)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline label edge::start() const
{
    return first();
}

inline label& edge::start()
{
    return first();
}


inline label edge::end() const
{
    return second();
}


inline label& edge::end()
{
    return second();
}


inline label edge::minVertex() const
{
    return (first() < second() ? first() : second());
}


inline label edge::maxVertex() const
{
    return (first() > second() ? first() : second());
}


inline bool edge::valid() const
{
    return (first() != second() && first() >= 0 && second() >= 0);
}


inline bool edge::found(const label pointLabel) const
{
    // -1: always false
    return
    (
        pointLabel >= 0
     && (pointLabel == first() || pointLabel == second())
    );
}


inline label edge::which(const label pointLabel) const
{
    // -1: always false
    if (pointLabel >= 0)
    {
        if (pointLabel == first())
        {
            return 0;
        }
        if (pointLabel == second())
        {
            return 1;
        }
    }

    return -1;
}


inline bool edge::connects(const edge& other) const
{
    return (other.found(first()) || other.found(second()));
}


inline label edge::commonVertex(const edge& other) const
{
    if (other.found(first()))
    {
        return first();
    }
    if (other.found(second()))
    {
        return second();
    }

    // No shared vertex.
    return -1;
}


inline label edge::otherVertex(const label pointLabel) const
{
    if (pointLabel == first())
    {
        return second();
    }
    if (pointLabel == second())
    {
        return first();
    }

    // The given vertex is not on the edge in the first place.
    return -1;
}


inline label edge::collapse()
{
    // Cannot resize FixedList, so mark duplicates with '-1'
    // (the lower vertex is retained)
    // catch any '-1' (eg, if called multiple times)

    label n = 2;
    if (first() == second() || second() < 0)
    {
        second() = -1;
        --n;
    }
    if (first() < 0)
    {
        --n;
    }

    return n;
}


inline edge edge::reverseEdge() const
{
    return edge(second(), first());
}


inline void edge::clear()
{
    first()  = -1;
    second() = -1;
}


inline label edge::count() const
{
    label n = 2;
    if (first() == second() || second() < 0)
    {
        --n;
    }
    if (first() < 0)
    {
        --n;
    }

    return n;
}


inline bool edge::empty() const
{
    return (first() < 0 && second() < 0);
}


inline bool edge::insert(const label index)
{
    if (index < 0)
    {
        // Cannot insert invalid point labels (use direct assignment for that)
        return false;
    }

    if (first() < 0)
    {
        // Store at first, if not duplicate of second
        if (index != second())
        {
            first() = index;
            return true;
        }
    }
    else if (second() < 0)
    {
        // Store at second, if not duplicate of first
        if (index != first())
        {
            second() = index;
            return true;
        }
    }

    return false;
}


template<class InputIterator>
inline label edge::insert
(
    InputIterator begIter,
    InputIterator endIter
)
{
    // Available slots.
    // Don't use count() since it has special treatment for duplicates
    const int maxChange = ((first() < 0 ? 1 : 0) + (second() < 0 ? 1 : 0));

    int changed = 0;
    for (; changed < maxChange && begIter != endIter; ++begIter)
    {
        if (insert(*begIter))
        {
            ++changed;
        }
    }

    return changed;
}


inline label edge::insert(std::initializer_list<label> list)
{
    return insert(list.begin(), list.end());
}


template<unsigned N>
inline label edge::insert(const FixedList<label, N>& list)
{
    return insert(list.begin(), list.end());
}


inline label edge::insert(const labelUList& list)
{
    return insert(list.begin(), list.end());
}


inline label edge::erase(const label index)
{
    if (index < 0)
    {
        // Can never remove invalid point labels!
        return 0;
    }

    label n = 0;
    if (index == first())
    {
        first() = -1;
        ++n;
    }

    // Automatically handle duplicates, which should not have been there anyhow
    if (index == second())
    {
        second() = -1;
        ++n;
    }

    return n;
}


template<class InputIterator>
inline label edge::erase
(
    InputIterator begIter,
    InputIterator endIter
)
{
    // Occupied slots.
    // Don't use count() since it has special treatment for duplicates
    const int maxChange = ((first() >= 0 ? 1 : 0) + (second() >= 0 ? 1 : 0));

    int changed = 0;
    for (; changed < maxChange && begIter != endIter; ++begIter)
    {
        changed += erase(*begIter);
    }

    return changed;
}


inline label edge::erase(std::initializer_list<label> list)
{
    return erase(list.begin(), list.end());
}


template<unsigned N>
inline label edge::erase(const FixedList<label, N>& list)
{
    return erase(list.begin(), list.end());
}


inline label edge::erase(const labelUList& list)
{
    return erase(list.begin(), list.end());
}


// Geometric

inline point edge::centre(const UList<point>& pts) const
{
    #ifdef FULLDEBUG
    if (first() < 0 || second() < 0)
    {
        FatalErrorInFunction
            << "negative point index on edge " << *this
            << abort(FatalError);
    }
    #endif

    return 0.5*(pts[first()] + pts[second()]);
}


inline vector edge::vec(const UList<point>& pts) const
{
    #ifdef FULLDEBUG
    if (first() < 0 || second() < 0)
    {
        FatalErrorInFunction
            << "negative point index on edge " << *this
            << abort(FatalError);
    }
    #endif

    return pts[second()] - pts[first()];
}


inline vector edge::unitVec(const UList<point>& pts) const
{
    #ifdef FULLDEBUG
    if (first() < 0 || second() < 0)
    {
        FatalErrorInFunction
            << "negative point index on edge " << *this
            << abort(FatalError);
    }
    #endif

    const vector v = (pts[second()] - pts[first()]);
    const scalar s(::Foam::mag(v));

    return s < ROOTVSMALL ? Zero : v/s;
}


inline scalar edge::mag(const UList<point>& pts) const
{
    return ::Foam::mag(vec(pts));
}


inline linePointRef edge::line(const UList<point>& pts) const
{
    #ifdef FULLDEBUG
    if (first() < 0 || second() < 0)
    {
        FatalErrorInFunction
            << "negative point index on edge " << *this
            << abort(FatalError);
    }
    #endif

    return linePointRef(pts[first()], pts[second()]);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline label& edge::operator[](const label i)
{
    #ifdef FULLDEBUG
    if (i < 0 || i > 1)
    {
        FatalErrorInFunction
            << "Index " << i << " out of range [0,1]" << abort(FatalError);
    }
    #endif
    return (i ? second() : first());
}


inline const label& edge::operator[](const label i) const
{
    #ifdef FULLDEBUG
    if (i < 0 || i > 1)
    {
        FatalErrorInFunction
            << "Index " << i << " out of range [0,1]" << abort(FatalError);
    }
    #endif
    return (i ? second() : first());
}


// * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * * //

inline bool operator==(const edge& a, const edge& b)
{
    return edge::compare(a,b) != 0;
}


inline bool operator!=(const edge& a, const edge& b)
{
    return edge::compare(a,b) == 0;
}


// ************************************************************************* //

 } // End namespace Foam
