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

#include <cctype>

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //
namespace Foam {
    inline void word::stripInvalid()
    {
        // skip stripping unless debug is active to avoid
        // costly operations
        if (debug && string::stripInvalid<word>(*this))
        {
            std::cerr
                << "word::stripInvalid() called for word "
                << this->c_str() << std::endl;

            if (debug > 1)
            {
                std::cerr
                    << "    For debug level (= " << debug
                    << ") > 1 this is considered fatal" << std::endl;
                std::abort();
            }
        }
    }


    // * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

    inline word::word(const word& w)
        :
        string(w)
    {}


    inline word::word()
        :
        string()
    {}


    inline word::word(const string& s, const bool doStripInvalid)
        :
        string(s)
    {
        if (doStripInvalid)
        {
            stripInvalid();
        }
    }


    inline word::word(const std::string& s, const bool doStripInvalid)
        :
        string(s)
    {
        if (doStripInvalid)
        {
            stripInvalid();
        }
    }


    inline word::word(const char* s, const bool doStripInvalid)
        :
        string(s)
    {
        if (doStripInvalid)
        {
            stripInvalid();
        }
    }

    inline word::word
    (
        const char* s,
        const size_type n,
        const bool doStripInvalid
    )
        :
        string(s, n)
    {
        if (doStripInvalid)
        {
            stripInvalid();
        }
    }


    // * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

    inline bool word::valid(char c)
    {
        return
            (
                !isspace(c)
                && c != '"'   // string quote
                && c != '\''  // string quote
                && c != '/'   // path separator
                && c != ';'   // end statement
                && c != '{'   // beg subdict
                && c != '}'   // end subdict
                );
    }


    // * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

    inline void word::operator=(const word& q)
    {
        string::operator=(q);
    }


    inline void word::operator=(const string& q)
    {
        string::operator=(q);
        stripInvalid();
    }


    inline void word::operator=(const std::string& q)
    {
        string::operator=(q);
        stripInvalid();
    }


    inline void word::operator=(const char* q)
    {
        string::operator=(q);
        stripInvalid();
    }


    // * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

    inline word operator&(const word& a, const word& b)
    {
        if (b.size())
        {
            string ub = b;
            ub.string::operator[](0) = char(toupper(ub.string::operator[](0)));

            return a + ub;
        }
        else
        {
            return a;
        }
    }

}
// ************************************************************************* //
