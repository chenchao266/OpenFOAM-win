﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2014 OpenFOAM Foundation
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

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


 namespace Foam{
inline ISstream::ISstream
(
    std::istream& is,
    const string& streamName,
    IOstreamOption streamOpt
)
:
    Istream(streamOpt),
    name_(streamName),
    is_(is)
{
    if (is_.good())
    {
        setOpened();
        setGood();
    }
    else
    {
        setState(is_.rdstate());
    }
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

inline ISstream& ISstream::get(char& c)
{
    is_.get(c);
    setState(is_.rdstate());

    if (good() && c == '\n')
    {
        ++lineNumber_;
    }

    return *this;
}


inline int ISstream::peek()
{
    return is_.peek();
}


inline ISstream& ISstream::getLine(std::string& str, char delim)
{
    std::getline(is_, str, delim);
    setState(is_.rdstate());

    if (delim == '\n')
    {
        ++lineNumber_;
    }

    return *this;
}


inline std::streamsize ISstream::getLine(std::nullptr_t, char delim)
{
    is_.ignore(std::numeric_limits<std::streamsize>::max(), delim);
    setState(is_.rdstate());

    std::streamsize count = is_.gcount();

    if (count && delim == '\n')
    {
        ++lineNumber_;
    }

    return count;
}


inline ISstream& ISstream::putback(const char c)
{
    if (c == '\n')
    {
        --lineNumber_;
    }

    if (!is_.putback(c))
    {
        setBad();
    }

    setState(is_.rdstate());

    return *this;
}


// ************************************************************************* //

 } // End namespace Foam
