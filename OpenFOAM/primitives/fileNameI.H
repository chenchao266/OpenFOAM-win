/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011 OpenFOAM Foundation
    Copyright (C) 2016-2021 OpenCFD Ltd.
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

#include <iostream>  // For std::cerr

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


 namespace Foam{
inline fileName::fileName(const word& s)
:
    string(s)
{}


inline fileName::fileName(word&& s)
:
    string(std::move(s))
{}


inline fileName::fileName(const string& s, bool doStrip)
:
    string(s)
{
    if (doStrip)
    {
        stripInvalid();
    }
}


inline fileName::fileName(string&& s, bool doStrip)
:
    string(std::move(s))
{
    if (doStrip)
    {
        stripInvalid();
    }
}


inline fileName::fileName(const std::string& s, bool doStrip)
:
    string(s)
{
    if (doStrip)
    {
        stripInvalid();
    }
}


inline fileName::fileName(std::string&& s, bool doStrip)
:
    string(std::move(s))
{
    if (doStrip)
    {
        stripInvalid();
    }
}


inline fileName::fileName(const char* s, bool doStrip)
:
    string(s)
{
    if (doStrip)
    {
        stripInvalid();
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline bool fileName::valid(char c)
{
    return
    (
        c != '"'    // string quote
     && c != '\''   // string quote
     && (!isspace(c) || (allowSpaceInFileName && c == ' '))
    );
}


inline void fileName::stripInvalid()
{
    // Only strip when debug is active (potentially costly operation)
    if (debug && string::stripInvalid<fileName>(*this))
    {
        std::cerr
            << "fileName::stripInvalid() called for invalid fileName "
            << this->c_str() << std::endl;

        if (debug > 1)
        {
            std::cerr
                << "    For debug level (= " << debug
                << ") > 1 this is considered fatal" << std::endl;
            std::exit(1);
        }

        removeRepeated('/');
        removeEnd('/');
    }
}


inline bool fileName::isAbsolute(const std::string& str)
{
    return !str.empty() &&
    (
        // Starts with '/', but also accept '\\' since it will be
        // converted to a generic '/' or it is part of a (windows)
        // UNC '\\server-name\path'
        // - accept even on non-windows systems

        (str[0] == '/' || str[0] == '\\')

#ifdef _WIN32
         // Filesytem root - eg, d:/path or d:\path
     || (
            (str.length() > 2 && str[1] == ':')
         && (str[2] == '/' || str[2] == '\\')
        )
#endif
    );
}


inline bool fileName::isAbsolute() const
{
    return isAbsolute(*this);
}


inline bool fileName::isBackup() const
{
    return isBackup(*this);
}


inline bool fileName::hasPath() const
{
    return string::hasPath();
}


inline std::string fileName::path(const std::string& str)
{
    const auto i = str.rfind('/');

    if (i == npos)
    {
        return ".";
    }
    else if (i)
    {
        return str.substr(0, i);
    }

    return "/";
}


inline fileName fileName::path() const
{
    return path(*this);
}


inline std::string fileName::name(const std::string& str)
{
    const auto i = str.rfind('/');

    if (npos == i)
    {
        return str;
    }

    return str.substr(i+1);
}


inline word fileName::name() const
{
    return fileName::name(*this);
}


word fileName::ext() const
{
    return string::ext();
}


inline word fileName::nameLessExt() const
{
    return nameLessExt(*this);
}


inline fileName fileName::lessExt() const
{
    const auto i = find_ext();

    if (i == npos)
    {
        return *this;
    }

    return substr(0, i);
}


inline bool fileName::removePath()
{
    return string::removePath();
}


inline fileName& fileName::ext(const word& ending)
{
    string::ext(ending);
    return *this;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline fileName& fileName::operator=(const fileName& str)
{
    // Self-assignment is a no-op
    if (this != &str)
    {
        assign(str);
    }
    return *this;
}


inline fileName& fileName::operator=(fileName&& str)
{
    // Self-assignment is a no-op
    if (this != &str)
    {
        assign(std::move(str));
    }
    return *this;
}


inline fileName& fileName::operator=(const word& str)
{
    assign(str);
    return *this;
}


inline fileName& fileName::operator=(word&& str)
{
    assign(std::move(str));
    return *this;
}


inline fileName& fileName::operator=(const string& str)
{
    assign(str);
    stripInvalid();
    return *this;
}


inline fileName& fileName::operator=(string&& str)
{
    assign(std::move(str));
    stripInvalid();
    return *this;
}


inline fileName& fileName::operator=(const std::string& str)
{
    assign(str);
    stripInvalid();
    return *this;
}


inline fileName& fileName::operator=(std::string&& str)
{
    assign(std::move(str));
    stripInvalid();
    return *this;
}


inline fileName& fileName::operator=(const char* str)
{
    assign(str);
    stripInvalid();
    return *this;
}


// ************************************************************************* //

 } // End namespace Foam
