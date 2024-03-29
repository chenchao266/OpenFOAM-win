﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
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

Class
    Foam::cellZone

Description
    A subset of mesh cells.

    Currently set up as an indirect list but will be extended to use a
    primitive mesh.  For quick check whether a cell belongs to the zone use
    the lookup mechanism in cellZoneMesh, where all the zoned cells are
    registered with their zone number.

SourceFiles
    cellZone.C
    cellZoneNew.C

\*---------------------------------------------------------------------------*/

#ifndef cellZone_H
#define cellZone_H

#include "zone.H"
#include "cellZoneMeshFwd.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward Declarations
class cellZone;
Ostream& operator<<(Ostream& os, const cellZone& zn);

/*---------------------------------------------------------------------------*\
                           Class cellZone Declaration
\*---------------------------------------------------------------------------*/

class cellZone
:
    public zone
{
    // Private Data

        //- Reference to zone list
        const cellZoneMesh& zoneMesh_;


    // Private Member Functions

        //- No copy construct
        cellZone(const cellZone&) = delete;


public:

    // Static Data Members

        //- The name associated with the zone-labels dictionary entry
        //- ("cellLabels")
        static const char * const labelsName;


    //- Runtime type information
    TypeName("cellZone");


    // Declare run-time constructor selection tables

        declareRunTimeSelectionTable
        (
            autoPtr,
            cellZone,
            dictionary,
            (
                const word& name,
                const dictionary& dict,
                const label index,
                const cellZoneMesh& zm
            ),
            (name, dict, index, zm)
        );


    // Constructors

        //- Construct an empty zone
        cellZone
        (
            const word& name,
            const label index,
            const cellZoneMesh& zm
        );

        //- Construct from components
        cellZone
        (
            const word& name,
            const labelUList& addr,
            const label index,
            const cellZoneMesh& zm
        );

        //- Construct from components, transferring addressing
        cellZone
        (
            const word& name,
            labelList&& addr,
            const label index,
            const cellZoneMesh& zm
        );

        //- Construct from dictionary
        cellZone
        (
            const word& name,
            const dictionary& dict,
            const label index,
            const cellZoneMesh& zm
        );

        //- Construct given the original zone (name is used),
        //- and resetting the cell list and zone mesh information
        cellZone
        (
            const cellZone& origZone,
            const labelUList& addr,
            const label index,
            const cellZoneMesh& zm
        );

        //- Construct with a new index and zone mesh information, the name
        //- of the original zone, resetting the cell addressing.
        cellZone
        (
            const cellZone& origZone,
            labelList&& addr,
            const label index,
            const cellZoneMesh& zm
        );


        //- Construct and return a clone, resetting the zone mesh
        virtual autoPtr<cellZone> clone(const cellZoneMesh& zm) const
        {
            return autoPtr<cellZone>::New(*this, *this, index(), zm);
        }

        //- Construct and return a clone,
        //- resetting the cell list and zone mesh
        virtual autoPtr<cellZone> clone
        (
            const labelUList& addr,
            const label index,
            const cellZoneMesh& zm
        ) const
        {
            return autoPtr<cellZone>::New(*this, addr, index, zm);
        }


    // Selectors

        //- Return a pointer to a new cell zone
        //- created on freestore from dictionary
        static autoPtr<cellZone> New
        (
            const word& name,
            const dictionary& dict,
            const label index,
            const cellZoneMesh& zm
        );


    //- Destructor
    virtual ~cellZone() = default;


    // Member Functions

        //- Return reference to the zone mesh
        const cellZoneMesh& zoneMesh() const noexcept
        {
            return zoneMesh_;
        }

        //- Helper function to re-direct to zone::localID(...)
        label whichCell(const label globalCellID) const;


        //- Check zone definition. Return true if in error.
        virtual bool checkDefinition(const bool report = false) const;

        //- Check whether zone is synchronised across coupled boundaries.
        //  \return True if any errors.
        virtual bool checkParallelSync(const bool report = false) const
        {
            return false;
        }

        //- Write dictionary
        virtual void writeDict(Ostream& os) const;


    // Member Operators

        //- Assign addressing, clearing demand-driven data
        void operator=(const cellZone& zn);

        //- Assign addressing, clearing demand-driven data
        void operator=(const labelUList& addr);

        //- Move assign addressing, clearing demand-driven data
        void operator=(labelList&& addr);


    // I-O

        //- Ostream Operator
        friend Ostream& operator<<(Ostream& os, const cellZone& zn);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
