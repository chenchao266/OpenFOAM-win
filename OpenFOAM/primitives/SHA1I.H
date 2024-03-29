﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011 OpenFOAM Foundation
    Copyright (C) 2019-2022 OpenCFD Ltd.
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

//#include "SHA1.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


 namespace Foam{
inline SHA1::SHA1()
{
    clear();
}


inline SHA1::SHA1(const char* str)
{
    clear();
    append(str);
}


inline SHA1::SHA1(const std::string& str)
{
    clear();
    append(str);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline void SHA1::append(char c)
{
    processBytes(&c, 1);
}


inline SHA1& SHA1::append(const char* data, size_t len)
{
    processBytes(data, len);
    return *this;
}


inline SHA1& SHA1::append(const char* str)
{
    if (str && *str)
    {
        processBytes(str, std::char_traits<char>::length(str));
    }
    return *this;
}


inline SHA1& SHA1::append(const std::string& str)
{
    processBytes(str.data(), str.length());
    return *this;
}


inline SHA1& SHA1::append
(
    const std::string& str,
    size_t pos,
    size_t len
)
{
    if (std::string::npos != pos && pos < str.length())
    {
        if (std::string::npos == len || pos + len > str.length())
        {
            len = str.length() - pos;
        }

        processBytes(str.data() + pos, len);
    }

    return *this;
}


inline SHA1Digest SHA1::digest() const
{
    SHA1Digest dig;

    if (finalized_)
    {
        calcDigest(dig);
    }
    else
    {
        // Avoid disturbing current data - use a copy
        SHA1 sha(*this);
        if (sha.finalize())
        {
            sha.calcDigest(dig);
        }
    }

    return dig;
}


inline std::string SHA1::str(const bool prefixed) const
{
    return digest().str(prefixed);
}


inline Ostream& SHA1::write(Ostream& os, const bool prefixed) const
{
    return digest().write(os, prefixed);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline SHA1::operator SHA1Digest() const
{
    return digest();
}


inline bool SHA1::operator==(const SHA1& rhs) const
{
    return this->digest() == rhs.digest();
}


inline bool SHA1::operator==(const SHA1Digest& dig) const
{
    return this->digest() == dig;
}


inline bool SHA1::operator==(const std::string& hexdigits) const
{
    return this->digest() == hexdigits;
}


inline bool SHA1::operator==(const char* hexdigits) const
{
    return this->digest() == hexdigits;
}


inline bool SHA1::operator!=(const SHA1& rhs) const
{
    return !this->operator==(rhs);
}


inline bool SHA1::operator!=(const SHA1Digest& rhs) const
{
    return !this->operator==(rhs);
}


inline bool SHA1::operator!=(const std::string& rhs) const
{
    return !this->operator==(rhs);
}


inline bool SHA1::operator!=(const char* rhs) const
{
    return !this->operator==(rhs);
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

inline Ostream& operator<<(Ostream& os, const SHA1& sha)
{
    return sha.write(os);
}


// ************************************************************************* //

 } // End namespace Foam
