/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2017 OpenFOAM Foundation
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

#include "labelRange.H"
#include "token.T.H"


// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //
namespace Foam {
    const labelRange labelRange::endLabelRange_;
    const labelRange::const_iterator labelRange::endIter_;
    int labelRange::debug(::Foam::debug::debugSwitch("labelRange", 0));


    // * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

    labelRange::labelRange(Istream& is) : start_(0),
        size_(0)
    {
        is >> *this;
    }


    // * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

    bool labelRange::intersects
    (
        const labelRange& range,
        const bool touches
    ) const
    {
        label final = touches ? 1 : 0;

        return
            (
                this->size()
                && range.size()
                &&
                (
                (
                    range.first() >= this->first()
                    && range.first() <= this->last() + final
                    )
                    ||
                    (
                        this->first() >= range.first()
                        && this->first() <= range.last() + final
                        )
                    )
                );
    }


    labelRange labelRange::join(const labelRange& range) const
    {
        // trivial cases first
        if (!size_)
        {
            return *this;
        }
        else if (!range.size_)
        {
            return range;
        }

        const label lower = min(this->first(), range.first());
        const label upper = max(this->last(), range.last());
        const label sz = upper - lower + 1;

        return labelRange(lower, sz);
    }


    // * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

    labelRange& labelRange::operator+=(const labelRange& rhs)
    {
        if (!size_)
        {
            // trivial case
            operator=(rhs);
        }
        else if (rhs.size_)
        {
            const label lower = min(this->first(), rhs.first());
            const label upper = max(this->last(), rhs.last());

            start_ = lower;
            size_ = upper - lower + 1;
        }

        return *this;
    }


    // * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

    Istream& operator>>(Istream& is, labelRange& range)
    {
        is.readBegin("labelRange");
        is >> range.start_ >> range.size_;
        is.readEnd("labelRange");

        is.check("operator>>(Istream&, labelRange&)");

        // disallow invalid sizes
        if (range.size_ <= 0)
        {
            range.clear();
        }

        return is;
    }


    Ostream& operator<<(Ostream& os, const labelRange& range)
    {
        // write ASCII only for now
        os << token::BEGIN_LIST
            << range.start_ << token::SPACE << range.size_
            << token::END_LIST;

        //    os  << token::BEGIN_BLOCK
        //        << range.start_ << "-" << range.last()
        //        << token::END_BLOCK;

        os.check("operator<<(Ostream&, const labelRange&)");
        return os;
    }

}
// ************************************************************************* //
