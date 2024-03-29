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

//#include "UList.H"
#include "_Ostream.H"
#include "token.H"
#include <vector>

// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //


 namespace Foam{
template<class T>
Ostream& operator<<(Ostream& os, const std::vector<T>& list)
{
    auto iter = list.cbegin();
    const auto last = list.cend();

    // Write ascii list contents, no line breaks

    os << label(list.size()) << token::BEGIN_LIST;

    if (iter != last)
    {
        os << *iter;

        for (++iter; iter != last; ++iter)
        {
            os << token::SPACE << *iter;
        }
    }

    os << token::END_LIST;

    os.check(FUNCTION_NAME);
    return os;
}


// ************************************************************************* //

 } // End namespace Foam
