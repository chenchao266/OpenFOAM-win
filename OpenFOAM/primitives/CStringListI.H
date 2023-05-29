/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016-2017 OpenCFD Ltd.
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

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


 namespace Foam{
inline char* CStringList::stringCopy(char *dest, const std::string& str)
{
    for (auto iter = str.cbegin(); iter != str.cend(); ++iter)
    {
        *dest = *iter;
        ++dest;
    }
    *(dest++) = '\0';

    return dest;
}


// * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * * //

inline int CStringList::count(const char * const argv[])
{
    int n = 0;
    if (argv)
    {
        while (argv[n])
        {
            ++n;
        }
    }
    return n;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline CStringList::CStringList()
:
    argc_(0),
    len_(0),
    argv_(nullptr),
    data_(nullptr)
{}


template<class StringType>
inline CStringList::CStringList(const UList<StringType>& input)
:
    CStringList()
{
    reset(input);
}


template<class StringType>
inline CStringList::CStringList(const SubStrings<StringType>& input)
:
    CStringList()
{
    reset(input);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

inline CStringList::~CStringList()
{
    clear();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline void CStringList::clear()
{
    argc_ = 0;
    len_  = 0;

    if (data_)
    {
        delete[] data_;
        data_ = nullptr;
    }
    if (argv_)
    {
        delete[] argv_;
        argv_ = nullptr;
    }
}


inline bool CStringList::empty() const noexcept
{
    return !argc_;
}


inline int CStringList::size() const noexcept
{
    return argc_;
}


inline size_t CStringList::length() const
{
    return len_;
}


inline char** CStringList::strings() const
{
    return argv_;
}


inline char** CStringList::strings(int start) const
{
    return &(argv_[start]);
}


inline const char* CStringList::data() const
{
    return data_;
}


template<class StringType>
inline int CStringList::reset(const UList<StringType>& input)
{
    return resetContent(input);
}


template<class StringType>
inline int CStringList::reset(const SubStrings<StringType>& input)
{
    return resetContent(input);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline const char* CStringList::operator[](int i) const
{
    return argv_[i];
}


// ************************************************************************* //

 } // End namespace Foam
