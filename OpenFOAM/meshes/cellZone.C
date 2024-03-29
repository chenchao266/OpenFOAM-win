﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011 OpenFOAM Foundation
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

#include "cellZone.H"
#include "addToRunTimeSelectionTable.H"
#include "cellZoneMesh.H"
#include "polyMesh.H"
#include "primitiveMesh.H"
#include "_IOstream.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(cellZone, 0);
    defineRunTimeSelectionTable(cellZone, dictionary);
    addToRunTimeSelectionTable(cellZone, cellZone, dictionary);


    const char * const cellZone::labelsName = "cellLabels";


    // * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

    cellZone::cellZone
    (
        const word& name,
        const label index,
        const cellZoneMesh& zm
    )
        :
        zone(name, index),
        zoneMesh_(zm)
    {}


    cellZone::cellZone
    (
        const word& name,
        const labelUList& addr,
        const label index,
        const cellZoneMesh& zm
    )
        :
        zone(name, addr, index),
        zoneMesh_(zm)
    {}


    cellZone::cellZone
    (
        const word& name,
        labelList&& addr,
        const label index,
        const cellZoneMesh& zm
    )
        :
        zone(name, std::move(addr), index),
        zoneMesh_(zm)
    {}


    cellZone::cellZone
    (
        const word& name,
        const dictionary& dict,
        const label index,
        const cellZoneMesh& zm
    )
        :
        zone(name, dict, this->labelsName, index),
        zoneMesh_(zm)
    {}


    cellZone::cellZone
    (
        const cellZone& origZone,
        const labelUList& addr,
        const label index,
        const cellZoneMesh& zm
    )
        :
        zone(origZone, addr, index),
        zoneMesh_(zm)
    {}


    cellZone::cellZone
    (
        const cellZone& origZone,
        labelList&& addr,
        const label index,
        const cellZoneMesh& zm
    )
        :
        zone(origZone, std::move(addr), index),
        zoneMesh_(zm)
    {}


    // * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

    label cellZone::whichCell(const label globalCellID) const
    {
        return zone::localID(globalCellID);
    }


    bool cellZone::checkDefinition(const bool report) const
    {
        return zone::checkDefinition(zoneMesh_.mesh().nCells(), report);
    }


    void cellZone::writeDict(Ostream& os) const
    {
        os.beginBlock(name());

        os.writeEntry("type", type());
        zoneIdentifier::write(os);
        writeEntry(this->labelsName, os);

        os.endBlock();
    }


    // * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

    void cellZone::operator=(const cellZone& zn)
    {
        clearAddressing();
        labelList::operator=(zn);
    }


    void cellZone::operator=(const labelUList& addr)
    {
        clearAddressing();
        labelList::operator=(addr);
    }


    void cellZone::operator=(labelList&& addr)
    {
        clearAddressing();
        labelList::transfer(addr);
    }


    // * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

    Ostream& operator<<(Ostream& os, const cellZone& zn)
    {
        zn.write(os);
        os.check(FUNCTION_NAME);
        return os;
    }

}
// ************************************************************************* //
