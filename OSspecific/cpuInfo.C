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

#include "cpuInfo.H"
#include "IOstreams.H"
#include "_Ostream.H"
#include "token.H"
#include "keyType.H"

#include <thread>

// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::cpuInfo::cpuInfo()
:
    vendor_id(),
    model_name(),
    cpu_family(-1),
    model(-1),
    cpu_MHz(0),
    siblings(0),
    cpu_cores(std::thread::hardware_concurrency())
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::cpuInfo::write(Ostream& os) const
{
    if (!vendor_id.empty())
    {
        os.writeEntry(keyType("vendor_id"), vendor_id);
    }
    if (!model_name.empty())
    {
        os.writeEntry(keyType("model_name"), model_name);
    }

    os.writeEntryIfDifferent<int>("cpu_family", -1, cpu_family);
    os.writeEntryIfDifferent<int>("model", -1, model);
    os.writeEntryIfDifferent<float>("cpu_MHz", 0, cpu_MHz);
    os.writeEntryIfDifferent<int>("cpu_cores", 0, cpu_cores);
    os.writeEntryIfDifferent<int>("siblings", 0, siblings);
}


// ************************************************************************* //
