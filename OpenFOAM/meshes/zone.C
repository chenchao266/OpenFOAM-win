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

\*---------------------------------------------------------------------------*/

#include "zone.H"
#include "dictionary2.H"
#include "HashSet.H"
#include "_IOstream.H"
#include "demandDrivenData.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(zone, 0);



    // * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

    zone::zone()
        :
        zoneIdentifier(),
        labelList(),
        lookupMapPtr_(nullptr)
    {}


    zone::zone(const word& name, const label index)
        :
        zoneIdentifier(name, index),
        labelList(),
        lookupMapPtr_(nullptr)
    {}


    zone::zone
    (
        const word& name,
        const labelUList& addr,
        const label index
    )
        :
        zoneIdentifier(name, index),
        labelList(addr),
        lookupMapPtr_(nullptr)
    {}


    zone::zone
    (
        const word& name,
        labelList&& addr,
        const label index
    )
        :
        zoneIdentifier(name, index),
        labelList(std::move(addr)),
        lookupMapPtr_(nullptr)
    {}


    zone::zone
    (
        const word& name,
        const dictionary& dict,
        const word& labelsName,
        const label index
    )
        :
        zone(name, dict.get<labelList>(labelsName), index)
    {}


    zone::zone
    (
        const zone& origZone,
        const labelUList& addr,
        const label index
    )
        :
        zone(origZone.name(), addr, index)
    {}


    zone::zone
    (
        const zone& origZone,
        labelList&& addr,
        const label index
    )
        :
        zone(origZone.name(), std::move(addr), index)
    {}


    // * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

    zone::~zone()
    {
        clearAddressing();
    }


    // * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

    const Map<label>& zone::lookupMap() const
    {
        if (!lookupMapPtr_)
        {
            DebugInFunction << "Calculating lookup map" << endl;

            const labelList& addr = *this;

            lookupMapPtr_ = new Map<label>(2 * addr.size());
            auto& lm = *lookupMapPtr_;

            forAll(addr, i)
            {
                lm.insert(addr[i], i);
            }
        }

        return *lookupMapPtr_;
    }


    label zone::localID(const label globalID) const
    {
        return lookupMap().lookup(globalID, -1);
    }


    void zone::clearAddressing()
    {
        deleteDemandDrivenData(lookupMapPtr_);
    }


    bool zone::checkDefinition(const label maxSize, const bool report) const
    {
        const labelList& addr = *this;

        bool hasError = false;

        // To check for duplicate entries
        labelHashSet elems(size());

        for (const label idx : addr)
        {
            if (idx < 0 || idx >= maxSize)
            {
                hasError = true;

                if (report)
                {
                    SeriousErrorInFunction
                        << "Zone " << this->name()
                        << " contains invalid index label " << idx << nl
                        << "Valid index labels are 0.."
                        << maxSize - 1 << endl;
                }
                else
                {
                    // w/o report - can stop checking now
                    break;
                }
            }
            else if (!elems.insert(idx))
            {
                if (report)
                {
                    WarningInFunction
                        << "Zone " << this->name()
                        << " contains duplicate index label " << idx << endl;
                }
            }
        }

        return hasError;
    }


    void zone::write(Ostream& os) const
    {
        os << nl << this->name()
            << nl << static_cast<const labelList&>(*this);
    }


    // * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

    Ostream& operator<<(Ostream& os, const zone& zn)
    {
        zn.write(os);
        os.check(FUNCTION_NAME);
        return os;
    }

}
// ************************************************************************* //
