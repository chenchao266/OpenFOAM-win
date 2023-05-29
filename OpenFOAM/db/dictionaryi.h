/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2021 OpenCFD Ltd.
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

//#include "dictionary2.H"

// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //


 namespace Foam{
inline int dictionary::reportOptional() noexcept
{
    return writeOptionalEntries;
}


inline int dictionary::reportOptional(const int level) noexcept
{
    int old(writeOptionalEntries);
    writeOptionalEntries = level;
    return old;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const fileName& dictionary::name() const noexcept
{
    return name_;
}


inline fileName& dictionary::name() noexcept
{
    return name_;
}


inline word dictionary::dictName() const
{
    word scopedName(name_.name());

    const auto i = scopedName.rfind('.');
    if (i == std::string::npos)
    {
        return scopedName;
    }

    return scopedName.substr(i+1);
}


inline bool dictionary::isNullDict() const noexcept
{
    return (this == &dictionary::null);
}


inline const dictionary& dictionary::parent() const noexcept
{
    return parent_;
}


inline bool dictionary::found
(
    const word& keyword,
     keyType::option matchOpt
) const
{
    return csearch(keyword, matchOpt).good();
}


inline entry* dictionary::findEntry
(
    const word& keyword,
     keyType::option matchOpt
)
{
    return search(keyword, matchOpt).ptr();
}


inline const entry* dictionary::findEntry
(
    const word& keyword,
     keyType::option matchOpt
) const
{
    return csearch(keyword, matchOpt).ptr();
}


inline const entry* dictionary::findScoped
(
    const word& keyword,
     keyType::option matchOpt
) const
{
    return csearchScoped(keyword, matchOpt).ptr();
}


inline dictionary* dictionary::findDict
(
    const word& keyword,
     keyType::option matchOpt
)
{
    return search(keyword, matchOpt).dictPtr();
}


inline const dictionary* dictionary::findDict
(
    const word& keyword,
     keyType::option matchOpt
) const
{
    return csearch(keyword, matchOpt).dictPtr();
}


inline bool dictionary::isDict
(
    const word& keyword,
     keyType::option matchOpt
) const
{
    return csearch(keyword, matchOpt).isDict();
}


// ************************************************************************* //

 } // End namespace Foam
