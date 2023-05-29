/*---------------------------------------------------------------------------*\
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

#include "labelRange.H"
#include "List.H"
#include "MinMax.H"
#include <numeric>

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    int labelRange::debug(debug::debugSwitch("labelRange", 0));



    // * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

    labelRange::labelRange(const MinMax<label>& range) noexcept
        :
        labelRange()
    {
        if (range.min() < range.max())
        {
            start() = range.min();
            size() = (range.max() - range.min()); // Hope for no overflow?
        }
    }


    labelRange::labelRange(Istream& is)
        :
        labelRange()
    {
        is >> *this;
    }


    // * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

    List<label> labelRange::labels() const
    {
        if (size() < 0)
        {
            // Skip this check?
            return List<label>();
        }

        List<label> result(this->size());
        std::iota(result.begin(), result.end(), this->start());

        return result;
    }


    void labelRange::adjust() noexcept
    {
        if (this->start() < 0)
        {
            if (this->size() > 0)
            {
                // Decrease size accordingly
                this->size() += this->start();
            }
            this->start() = 0;
        }
        clampSize();
    }


    bool labelRange::overlaps(const labelRange& range, bool touches) const
    {
        const label extra = touches ? 1 : 0;

        return
            (
                this->size() && range.size()
                &&
                (
                (
                    range.first() >= this->first()
                    && range.first() <= this->last() + extra
                    )
                    ||
                    (
                        this->first() >= range.first()
                        && this->first() <= range.last() + extra
                        )
                    )
                );
    }


    labelRange labelRange::join(const labelRange& range) const
    {
        // Trivial cases first
        if (!this->size())
        {
            return *this;
        }
        else if (!range.size())
        {
            return range;
        }

        const label lower = ::Foam::min(this->first(), range.first());
        const label upper = ::Foam::max(this->last(), range.last());
        const label total = upper + 1 - lower;
        // last = start+size-1
        // size = last+1-start

        labelRange newRange(lower, total);
        newRange.clampSize();

        return newRange;
    }


    labelRange labelRange::subset(const labelRange& range) const
    {
        const label lower = ::Foam::max(this->first(), range.first());
        const label upper = ::Foam::min(this->last(), range.last());
        const label total = upper + 1 - lower;
        // last = start+size-1
        // size = last+1-start

        if (total > 0)
        {
            return labelRange(lower, total);
        }

        return labelRange();
    }


    labelRange labelRange::subset
    (
        const label start,
        const label size
    ) const
    {
        const label lower = ::Foam::max(this->start(), start);
        const label upper = ::Foam::min(this->last(), start + ::Foam::max(0, size - 1));
        const label total = upper + 1 - lower;
        // last = start+size-1
        // size = last+1-start

        if (total > 0)
        {
            return labelRange(lower, total);
        }

        return labelRange();
    }


    labelRange labelRange::subset0(const label size) const
    {
        const label lower = ::Foam::max(this->start(), 0);
        const label upper = ::Foam::min(this->last(), ::Foam::max(0, size - 1));
        const label total = upper + 1 - lower;
        // last = start+size-1
        // size = last+1-start

        if (total > 0)
        {
            return labelRange(lower, total);
        }

        return labelRange();
    }

}
// ************************************************************************* //
