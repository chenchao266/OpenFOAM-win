/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2020 OpenCFD Ltd.
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

#include "token.H"
#include "List.H"
#include "_Istream.H"
#include "_Ostream.H"
#include <numeric>

// * * * * * * * * * * * * * * * Local Functions * * * * * * * * * * * * * * //

namespace Foam
{

    template<class T>
    inline static List<label> makeIdentity(const IntRange<T>& range)
    {
        if (range.size() < 0)
        {
            // Skip this check?
            return List<label>();
        }

        List<label> result(range.size());
        std::iota(result.begin(), result.end(), range.start());

        return result;
    }

    template<class T>
    inline static Istream& input(Istream& is, IntRange<T>& range)
    {
        is.readBegin("IntRange");
        is >> range.start() >> range.size();
        is.readEnd("IntRange");

        is.check(FUNCTION_NAME);
        return is;
    }

    template<class T>
    inline static Ostream& output(Ostream& os, const IntRange<T>& range)
    {
        os  << token::BEGIN_LIST
            << range.start() << token::SPACE << range.size()
            << token::END_LIST;

        os.check(FUNCTION_NAME);
        return os;
    }

} // End namespace Foam


// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //


 namespace Foam{
List<label> identity(const IntRange<int32_t>& range)
{
    return makeIdentity(range);
}


#if defined(WM_LABEL_SIZE) && (WM_LABEL_SIZE >= 64)
List<label> identity(const IntRange<int64_t>& range)
{
    return makeIdentity(range);
}
#endif


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

Istream& operator>>(Istream& is, IntRange<int32_t>& range)
{
    return input(is, range);
}


Istream& operator>>(Istream& is, IntRange<int64_t>& range)
{
    return input(is, range);
}


Ostream& operator<<(Ostream& os, const IntRange<int32_t>& range)
{
    return output(os, range);
}


Ostream& operator<<(Ostream& os, const IntRange<int64_t>& range)
{
    return output(os, range);
}


// ************************************************************************* //

 } // End namespace Foam
