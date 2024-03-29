﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2015 OpenFOAM Foundation
    Copyright (C) 2017-2021 OpenCFD Ltd.
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

// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //


 namespace Foam{
inline std::string::size_type string::find_ext(const std::string& str)
{
    const auto i = str.find_last_of("./");

    if (i == npos || i == 0 || str[i] == '/')
    {
        return npos;
    }

    return i;
}


inline std::string::size_type string::find_ext() const
{
    return find_ext(*this);
}


inline bool string::hasPath() const
{
    return (npos != find('/'));
}


inline bool string::hasExt() const
{
    return (npos != find_ext());
}


inline bool string::hasExt(const char* ending) const
{
    return (ending && string::hasExt(std::string(ending)));
}


inline bool string::hasExt(const std::string& ending) const
{
    const auto len = ending.size();
    auto i = find_ext();
    if (i == npos || !len)
    {
        return false;
    }

    ++i; // Compare *after* the dot
    return ((size() - i) == len) && !compare(i, npos, ending);
}


inline bool string::removePath()
{
    const auto i = rfind('/');

    if (npos == i)
    {
        return false;
    }

    erase(0, i+1);
    return true;
}


inline bool string::removeExt()
{
    const auto i = find_ext();

    if (npos == i)
    {
        return false;
    }

    erase(i);
    return true;
}



// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline string::string(const std::string& str)
:
    std::string(str)
{}


inline string::string(std::string&& str)
:
    std::string(std::move(str))
{}


inline string::string(const char* str)
:
    std::string(str)
{}


inline string::string(const char* str, const size_type len)
:
    std::string(str, len)
{}


inline string::string(const char c)
:
    std::string(1, c)
{}


inline string::string(const size_type len, const char c)
:
    std::string(len, c)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class StringType>
inline bool string::valid(const std::string& str)
{
    for (auto iter = str.cbegin(); iter != str.cend(); ++iter)
    {
        if (!StringType::valid(*iter))
        {
            return false;
        }
    }

    return true;
}


template<class StringType>
inline bool string::stripInvalid(std::string& str)
{
    if (!string::valid<StringType>(str))
    {
        size_type nChar = 0;
        iterator outIter = str.begin();

        for (auto iter = str.cbegin(); iter != str.cend(); ++iter)
        {
            const char c = *iter;

            if (StringType::valid(c))
            {
                *outIter = c;
                ++outIter;
                ++nChar;
            }
        }

        str.erase(nChar);

        return true;
    }

    return false;
}


template<class StringType>
inline StringType string::validate(const std::string& str)
{
    StringType out;
    out.resize(str.size());

    size_type len = 0;
    for (auto iter = str.cbegin(); iter != str.cend(); ++iter)
    {
        const char c = *iter;
        if (StringType::valid(c))
        {
            out[len] = c;
            ++len;
        }
    }

    out.erase(len);

    return out;
}


inline bool string::match(const std::string& text) const
{
    return !compare(text);  // Always compare as literal string
}


inline void string::swap(std::string& str)
{
    if (this != &str)
    {
        // Self-swap is a no-op
        std::string::swap(str);
    }
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline bool string::operator()(const std::string& text) const
{
    return !compare(text);  // Always compare as literal string
}


// ************************************************************************* //

 } // End namespace Foam
