﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
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

#include "dictionary2.H"
#include "Pstream.H"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


 namespace Foam{
dictionary::const_searcher dictionary::csearchCompat
(
    const word& keyword,
    std::initializer_list<std::pair<const char*,int>> compat,
    enum keyType::option matchOpt
) const
{
    const_searcher finder(csearch(keyword, matchOpt));

    if (finder.good())
    {
        return finder;
    }

    for (const std::pair<const char*,int>& alt : compat)
    {
        finder = csearch(word::validate(alt.first), matchOpt);

        if (finder.good())
        {
            if (error::warnAboutAge(alt.second) && error::master())
            {
                std::cerr
                    << "--> FOAM IOWarning :" << nl
                    << "    Found [v" << alt.second << "] '"
                    << alt.first << "' entry instead of '"
                    << keyword.c_str() << "' in dictionary \""
                    << relativeName() << '"' << nl
                    << std::endl;

                error::warnAboutAge("keyword", alt.second);
            }

            break;
        }
    }

    return finder;
}


bool dictionary::foundCompat
(
    const word& keyword,
    std::initializer_list<std::pair<const char*,int>> compat,
    enum keyType::option matchOpt
) const
{
    return csearchCompat(keyword, compat, matchOpt).good();
}


const entry* dictionary::findCompat
(
    const word& keyword,
    std::initializer_list<std::pair<const char*,int>> compat,
    enum keyType::option matchOpt
) const
{
    return csearchCompat(keyword, compat, matchOpt).ptr();
}


const entry& dictionary::lookupEntryCompat
(
    const word& keyword,
    std::initializer_list<std::pair<const char*,int>> compat,
    enum keyType::option matchOpt
) const
{
    const const_searcher finder(csearchCompat(keyword, compat, matchOpt));

    if (!finder.good())
    {
        FatalIOErrorInFunction(*this)
            << "Entry '" << keyword << "' not found in dictionary "
            << relativeName()
            << exit(FatalIOError);
    }

    return finder.ref();
}


ITstream& dictionary::lookupCompat
(
    const word& keyword,
    std::initializer_list<std::pair<const char*,int>> compat,
    enum keyType::option matchOpt
) const
{
    return lookupEntryCompat(keyword, compat, matchOpt).stream();
}


// ************************************************************************* //

 } // End namespace Foam
