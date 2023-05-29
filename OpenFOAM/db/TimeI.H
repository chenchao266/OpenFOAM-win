/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
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

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


 namespace Foam{
inline Time::Time
(
    const word& ctrlDictName,
    const argList& args,
    const bool enableFunctionObjects,
    const bool enableLibs
)
:
    Time
    (
        ctrlDictName,
        args,
        "system",
        "constant",
        enableFunctionObjects,
        enableLibs
    )
{}


inline Time::Time
(
    const word& ctrlDictName,
    const fileName& rootPath,
    const fileName& caseName,
    const bool enableFunctionObjects,
    const bool enableLibs
)
:
    Time
    (
        ctrlDictName,
        rootPath,
        caseName,
        "system",
        "constant",
        enableFunctionObjects,
        enableLibs
)
{}


inline Time::Time
(
    const dictionary& dict,
    const fileName& rootPath,
    const fileName& caseName,
    const bool enableFunctionObjects,
    const bool enableLibs
)
:
    Time
    (
        dict,
        rootPath,
        caseName,
        "system",
        "constant",
        enableFunctionObjects,
        enableLibs
    )
{}


inline Time::Time
(
    const fileName& rootPath,
    const fileName& caseName,
    const bool enableFunctionObjects,
    const bool enableLibs
)
:
    Time
    (
        rootPath,
        caseName,
        "system",
        "constant",
        enableFunctionObjects,
        enableLibs
    )
{}


// ************************************************************************* //

 } // End namespace Foam
