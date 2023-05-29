/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2014 OpenFOAM Foundation
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

#include "prefixOSstream.H"
#include "Pstream.H"
#include "token.H"

// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //


 namespace Foam{
inline void prefixOSstream::checkWritePrefix()
{
    if (printPrefix_ && !prefix_.empty())
    {
        OSstream::write(prefix_.c_str());
        printPrefix_ = false;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

prefixOSstream::prefixOSstream
(
    std::ostream& os,
    const string& streamName,
    IOstreamOption streamOpt
)
:
    OSstream(os, streamName, streamOpt),
    printPrefix_(true),
    prefix_()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void prefixOSstream::print(Ostream& os) const
{
    os  << "prefixOSstream ";
    OSstream::print(os);
}


bool prefixOSstream::write(const token& tok)
{
    return OSstream::write(tok);
}


Ostream& prefixOSstream::write(const char c)
{
    checkWritePrefix();
    OSstream::write(c);

    if (c == token::NL)
    {
        printPrefix_ = true;
    }

    return *this;
}


Ostream& prefixOSstream::write(const char* str)
{
    checkWritePrefix();
    OSstream::write(str);

    const size_t len = strlen(str);
    if (len && str[len-1] == token::NL)
    {
        printPrefix_ = true;
    }

    return *this;
}


Ostream& prefixOSstream::write(const word& val)
{
    checkWritePrefix();
    return OSstream::write(val);
}


Ostream& prefixOSstream::write(const string& val)
{
    checkWritePrefix();
    return OSstream::write(val);
}


Ostream& prefixOSstream::writeQuoted
(
    const std::string& val,
    const bool quoted
)
{
    checkWritePrefix();
    return OSstream::writeQuoted(val, quoted);
}


Ostream& prefixOSstream::write(const int32_t val)
{
    checkWritePrefix();
    return OSstream::write(val);
}


Ostream& prefixOSstream::write(const int64_t val)
{
    checkWritePrefix();
    return OSstream::write(val);
}


Ostream& prefixOSstream::write(const floatScalar val)
{
    checkWritePrefix();
    return OSstream::write(val);
}


Ostream& prefixOSstream::write(const doubleScalar val)
{
    checkWritePrefix();
    return OSstream::write(val);
}


Ostream& prefixOSstream::write
(
    const char* buf,
    std::streamsize count
)
{
    checkWritePrefix();
    return OSstream::write(buf, count);
}


void prefixOSstream::indent()
{
    checkWritePrefix();
    OSstream::indent();
}


// ************************************************************************* //

 } // End namespace Foam
