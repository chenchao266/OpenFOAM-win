/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2013 OpenFOAM Foundation
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

#include "argList.H"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //
namespace Foam {
inline const word& argList::executable() const
{
    return executable_;
}


inline const fileName& argList::rootPath() const
{
    return rootPath_;
}


inline const fileName& argList::caseName() const
{
    return case_;
}


inline const fileName& argList::globalCaseName() const
{
    return globalCase_;
}


inline const ParRunControl& argList::parRunControl() const
{
    return parRunControl_;
}


inline fileName argList::path() const
{
    return rootPath()/caseName();
}


inline const stringList& argList::args() const
{
    return args_;
}


inline stringList& argList::args()
{
    return args_;
}


inline const string& argList::arg(const label index) const
{
    return args_[index];
}


inline label argList::size() const
{
    return args_.size();
}


inline const HashTable<string>& argList::options() const
{
    return options_;
}


inline HashTable<string>& argList::options()
{
    return options_;
}


inline const string& argList::option(const word& opt) const
{
    return options_[opt];
}


inline bool argList::optionFound(const word& opt) const
{
    return options_.found(opt);
}


inline IStringStream argList::optionLookup(const word& opt) const
{
    return IStringStream(options_[opt]);
}


// * * * * * * * * * * * * Template Specializations  * * * * * * * * * * * * //

 
    // Template specialization for string
    template<>
    inline string
    argList::argRead<string>(const label index) const
    {
        return args_[index];
    }

    // Template specialization for word
    template<>
    inline word
    argList::argRead<word>(const label index) const
    {
        return args_[index];
    }

    // Template specialization for fileName
    template<>
    inline fileName
    argList::argRead<fileName>(const label index) const
    {
        return args_[index];
    }

    // Template specialization for string
    template<>
    inline string
    argList::optionRead<string>(const word& opt) const
    {
        return options_[opt];
    }

    // Template specialization for word
    template<>
    inline word
    argList::optionRead<word>(const word& opt) const
    {
        return options_[opt];
    }

    // Template specialization for fileName
    template<>
    inline fileName
    argList::optionRead<fileName>(const word& opt) const
    {
        return options_[opt];
    }
 


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T>
inline T argList::argRead(const label index) const
{
    T val;

    IStringStream(args_[index])() >> val;
    return val;
}


template<class T>
inline T argList::optionRead(const word& opt) const
{
    T val;

    optionLookup(opt)() >> val;
    return val;
}


template<class T>
inline bool argList::optionReadIfPresent
(
    const word& opt,
    T& val
) const
{
    if (optionFound(opt))
    {
        val = optionRead<T>(opt);
        return true;
    }
    else
    {
        return false;
    }
}


template<class T>
inline bool argList::optionReadIfPresent
(
    const word& opt,
    T& val,
    const T& deflt
) const
{
    if (optionReadIfPresent<T>(opt, val))
    {
        return true;
    }
    else
    {
        val = deflt;
        return false;
    }
}


template<class T>
inline T argList::optionLookupOrDefault
(
    const word& opt,
    const T& deflt
) const
{
    if (optionFound(opt))
    {
        return optionRead<T>(opt);
    }
    else
    {
        return deflt;
    }
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline const string& argList::operator[](const label index) const
{
    return args_[index];
}


inline const string& argList::operator[](const word& opt) const
{
    return options_[opt];
}

}
// ************************************************************************* //
