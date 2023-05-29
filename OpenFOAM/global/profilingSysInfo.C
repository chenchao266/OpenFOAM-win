/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
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

#include "profilingSysInfo.H"
#include "foamVersion.H"
#include "clock.H"
#include "_Ostream.H"
#include "OSspecific.H"

// * * * * * * * * * * * * * * * Local Functions * * * * * * * * * * * * * * //
namespace Foam{
namespace
{

// Write environment entry
inline void printEnv
(
    Ostream& os,
    const word& key,
    const std::string& envName
)
{
    const std::string value(getEnv(envName));
    if (!value.empty())
    {
        os.writeEntry(key, value);
    }
}

} // End anonymous namespace


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

Ostream& profilingSysInfo::write
(
    Ostream& os
) const
{
    os.writeEntry("host",       hostName()); // short name
    os.writeEntry("date",       clock::dateTime());

    // compile-time information
    os.writeEntry("version",    foamVersion::version);
    os.writeEntry("build",      foamVersion::build);

    printEnv(os, "arch",         "WM_ARCH");
    printEnv(os, "compilerType", "WM_COMPILER_TYPE");
    printEnv(os, "compiler",     "WM_COMPILER");
    printEnv(os, "mplib",        "WM_MPLIB");
    printEnv(os, "options",      "WM_OPTIONS");

    return os;
}


// ************************************************************************* //

 } // End namespace Foam
