﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2016 OpenFOAM Foundation
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

#include "direction.H"
#include "IOstreams.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
namespace Foam {
    direction readDirection(Istream& is)
    {
        direction val;
        is >> val;

        return val;
    }


    Istream& operator>>(Istream& is, direction& d)
    {
        token t(is);

        if (!t.good())
        {
            is.setBad();
            return is;
        }

        if (t.isLabel())
        {
            d = direction(t.labelToken());
        }
        else
        {
            is.setBad();
            FatalIOErrorInFunction(is)
                << "wrong token type - expected direction, found " << t.info()
                << exit(FatalIOError);

            return is;
        }

        // Check state of Istream
        is.check("Istream& operator>>(Istream&, direction&)");

        return is;
    }


    Ostream& operator<<(Ostream& os, const direction d)
    {
        os.write(label(d));
        os.check("Ostream& operator<<(Ostream&, const direction)");
        return os;
    }


    std::ostream& operator<<(std::ostream& os, const direction d)
    {
        os << int(d);
        return os;
    }

}
// ************************************************************************* //
