﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2015-2017 OpenFOAM Foundation
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

//#include "IOobject.H"
#include "fileOperation.H"
#include "_Istream.H"
#include "IOstreams.H"
#include "Pstream.H"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


 namespace Foam{
template<class Type>
bool IOobject::typeHeaderOk
(
    const bool checkType,
    const bool search,
    const bool verbose
)
{
    bool ok = true;

    // Everyone check or just master
    bool masterOnly =
        typeGlobal<Type>()
     && (
            IOobject::fileModificationChecking == timeStampMaster
         || IOobject::fileModificationChecking == inotifyMaster
        );

    const fileOperation& fp = fileHandler();

    // Determine local status
    if (!masterOnly || Pstream::master())
    {
        fileName fName(typeFilePath<Type>(*this, search));

        ok = fp.readHeader(*this, fName, Type::typeName);
        if (ok && checkType && headerClassName_ != Type::typeName)
        {
            if (verbose)
            {
                WarningInFunction
                    << "unexpected class name " << headerClassName_
                    << " expected " << Type::typeName
                    << " when reading " << fName << endl;
            }

            ok = false;
        }
    }

    // If masterOnly make sure all processors know about it
    if (masterOnly)
    {
        Pstream::scatter(ok);
    }

    return ok;
}


template<class Type>
void IOobject::warnNoRereading() const
{
    if (readOpt() == IOobject::MUST_READ_IF_MODIFIED)
    {
        WarningInFunction
            << Type::typeName << ' ' << name()
            << " constructed with IOobject::MUST_READ_IF_MODIFIED but "
            << Type::typeName << " does not support automatic rereading."
            << endl;
    }
}


// ************************************************************************* //

 } // End namespace Foam
