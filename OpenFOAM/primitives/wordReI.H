/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2016 OpenFOAM Foundation
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

// * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * * //
namespace Foam {
    inline bool wordRe::meta(char c)
    {
        return regExp::meta(c);
    }


    inline bool wordRe::isPattern(const string& str)
    {
        return string::meta<regExp>(str);
    }


    // * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

    inline wordRe::wordRe()
        :
        word(),
        re_()
    {}


    inline wordRe::wordRe(const wordRe& str)
        :
        word(str),
        re_()
    {
        if (str.isPattern())
        {
            compile();
        }
    }


    inline wordRe::wordRe(const word& str)
        :
        word(str),
        re_()
    {}


    inline wordRe::wordRe(const keyType& str)
        :
        word(str, false),
        re_()
    {
        if (str.isPattern())
        {
            compile();
        }
    }


    inline wordRe::wordRe(const keyType& str, const compOption opt)
        :
        word(str, false),
        re_()
    {
        if (str.isPattern())
        {
            compile(opt);
        }
    }


    inline wordRe::wordRe(const char* str, const compOption opt)
        :
        word(str, false),
        re_()
    {
        compile(opt);
    }


    inline wordRe::wordRe(const string& str, const compOption opt)
        :
        word(str, false),
        re_()
    {
        compile(opt);
    }


    inline wordRe::wordRe(const std::string& str, const compOption opt)
        :
        word(str, false),
        re_()
    {
        compile(opt);
    }


    // * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

    inline bool wordRe::isPattern() const
    {
        return re_.exists();
    }


    inline bool wordRe::compile(const compOption opt) const
    {
        bool doCompile = false;

        if (opt & wordRe::REGEXP)
        {
            doCompile = true;
        }
        else if (opt & wordRe::DETECT)
        {
            if (string::meta<regExp>(*this) || !string::valid<word>(*this))
            {
                doCompile = true;
            }
        }
        else if (opt & wordRe::NOCASE)
        {
            doCompile = true;
        }


        if (doCompile)
        {
            re_.set(*this, (opt & wordRe::NOCASE));
        }
        else
        {
            re_.clear();
        }

        return re_.exists();
    }


    inline bool wordRe::compile() const
    {
        re_ = *this;
        return re_.exists();
    }


    inline bool wordRe::recompile() const
    {
        if (re_.exists())
        {
            re_ = *this;
        }

        return re_.exists();
    }


    inline void wordRe::uncompile(const bool doStripInvalid) const
    {
        if (re_.clear())
        {
            // skip stripping unless debug is active to avoid costly operations
            if (word::debug && doStripInvalid)
            {
                string::stripInvalid<word>
                    (
                        const_cast<word&>(static_cast<const word&>(*this))
                        );
            }
        }
    }


    inline void wordRe::clear()
    {
        word::clear();
        re_.clear();
    }


    inline bool wordRe::match(const std::string& str, bool literalMatch) const
    {
        if (literalMatch || !re_.exists())
        {
            // check as string
            return (str == *this);
        }
        else
        {
            // check as regex
            return re_.match(str);
        }
    }


    inline string wordRe::quotemeta() const
    {
        return string::quotemeta<regExp>(*this);
    }


    inline void wordRe::set(const std::string& str, const compOption opt)
    {
        string::operator=(str);
        compile(opt);
    }


    inline void wordRe::set(const char* str, const compOption opt)
    {
        string::operator=(str);
        compile(opt);
    }


    // * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

    inline void wordRe::operator=(const wordRe& str)
    {
        string::operator=(str);

        if (str.isPattern())
        {
            compile();
        }
        else
        {
            re_.clear();
        }
    }


    inline void wordRe::operator=(const word& str)
    {
        word::operator=(str);
        re_.clear();
    }


    inline void wordRe::operator=(const keyType& str)
    {
        string::operator=(str);
        if (str.isPattern())
        {
            compile();
        }
    }


    inline void wordRe::operator=(const string& str)
    {
        string::operator=(str);
        compile(DETECT);  // auto-detect regex
    }


    inline void wordRe::operator=(const std::string& str)
    {
        string::operator=(str);
        compile(DETECT);  // auto-detect regex
    }


    inline void wordRe::operator=(const char* str)
    {
        string::operator=(str);
        compile(DETECT);  // auto-detect regex
    }

}
// ************************************************************************* //
