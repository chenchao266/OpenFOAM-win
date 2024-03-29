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

#include "faceZone.H"
#include "addToRunTimeSelectionTable.H"
#include "faceZoneMesh.H"
#include "polyMesh.H"
#include "primitiveMesh.H"
#include "demandDrivenData.H"
#include "mapPolyMesh.H"
#include "syncTools.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(faceZone, 0);
    defineRunTimeSelectionTable(faceZone, dictionary);
    addToRunTimeSelectionTable(faceZone, faceZone, dictionary);

    const char* const faceZone::labelsName = "faceLabels";


    // * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

    void faceZone::setFlipMap(const bool val)
    {
        // Match size for flipMap
        if (flipMap_.size() == this->size())
        {
            flipMap_ = val;
        }
        else
        {
            // Avoid copying old values on resize
            flipMap_.clear();
            flipMap_.resize(this->size(), val);
        }
    }


    // * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

    void faceZone::calcFaceZonePatch() const
    {
        DebugInFunction << "Calculating primitive patch" << endl;

        if (patchPtr_)
        {
            FatalErrorInFunction
                << "primitive face zone patch already calculated"
                << abort(FatalError);
        }

        patchPtr_ =
            new primitiveFacePatch
            (
                faceList(size()),
                zoneMesh().mesh().points()
            );

        primitiveFacePatch& patch = *patchPtr_;

        const faceList& f = zoneMesh().mesh().faces();

        const labelList& addr = *this;
        const boolList& flip = flipMap();

        forAll(addr, facei)
        {
            if (flip[facei])
            {
                patch[facei] = f[addr[facei]].reverseFace();
            }
            else
            {
                patch[facei] = f[addr[facei]];
            }
        }

        DebugInfo << "Finished calculating primitive patch" << endl;
    }


    void faceZone::calcCellLayers() const
    {
        DebugInFunction << "Calculating master cells" << endl;

        // It is an error to attempt to recalculate edgeCells
        // if the pointer is already set
        if (masterCellsPtr_ || slaveCellsPtr_)
        {
            FatalErrorInFunction
                << "cell layers already calculated"
                << abort(FatalError);
        }
        else
        {
            // Go through all the faces in the master zone.  Choose the
            // master or slave cell based on the face flip

            const labelList& own = zoneMesh().mesh().faceOwner();
            const labelList& nei = zoneMesh().mesh().faceNeighbour();

            const labelList& mf = *this;

            const boolList& faceFlip = flipMap();

            masterCellsPtr_ = new labelList(mf.size());
            labelList& mc = *masterCellsPtr_;

            slaveCellsPtr_ = new labelList(mf.size());
            labelList& sc = *slaveCellsPtr_;

            forAll(mf, facei)
            {
                const label ownCelli = own[mf[facei]];
                const label neiCelli =
                    (
                        zoneMesh().mesh().isInternalFace(mf[facei])
                        ? nei[mf[facei]]
                        : -1
                        );

                if (!faceFlip[facei])
                {
                    // Face is oriented correctly, no flip needed
                    mc[facei] = neiCelli;
                    sc[facei] = ownCelli;
                }
                else
                {
                    mc[facei] = ownCelli;
                    sc[facei] = neiCelli;
                }
            }
        }
    }


    void faceZone::checkAddressing() const
    {
        const labelList& addr = *this;

        if (addr.size() != flipMap_.size())
        {
            FatalErrorInFunction
                << "Size of addressing: " << addr.size()
                << " size of flip map: " << flipMap_.size()
                << abort(FatalError);
        }

        // Note: nFaces, nCells might not be set yet on mesh so use owner size
        const label nFaces = zoneMesh().mesh().faceOwner().size();

        for (const label facei : addr)
        {
            if (facei < 0 || facei >= nFaces)
            {
                WarningInFunction
                    << "Illegal face index " << facei
                    << " outside range 0.." << nFaces - 1 << endl;
                break;  // Only report once
            }
        }
    }


    // * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

    faceZone::faceZone
    (
        const word& name,
        const label index,
        const faceZoneMesh& zm
    )
        :
        zone(name, index),
        flipMap_(),
        zoneMesh_(zm),
        patchPtr_(nullptr),
        masterCellsPtr_(nullptr),
        slaveCellsPtr_(nullptr),
        mePtr_(nullptr)
    {}


    faceZone::faceZone
    (
        const word& name,
        const labelUList& addr,
        const bool flipMapValue,
        const label index,
        const faceZoneMesh& zm
    )
        :
        zone(name, addr, index),
        flipMap_(),
        zoneMesh_(zm),
        patchPtr_(nullptr),
        masterCellsPtr_(nullptr),
        slaveCellsPtr_(nullptr),
        mePtr_(nullptr)
    {
        flipMap_.resize(size(), flipMapValue);
        checkAddressing();
    }


    faceZone::faceZone
    (
        const word& name,
        labelList&& addr,
        const bool flipMapValue,
        const label index,
        const faceZoneMesh& zm
    )
        :
        zone(name, std::move(addr), index),
        flipMap_(),
        zoneMesh_(zm),
        patchPtr_(nullptr),
        masterCellsPtr_(nullptr),
        slaveCellsPtr_(nullptr),
        mePtr_(nullptr)
    {
        flipMap_.resize(size(), flipMapValue);
        checkAddressing();
    }


    faceZone::faceZone
    (
        const word& name,
        const labelUList& addr,
        const boolUList& fm,
        const label index,
        const faceZoneMesh& zm
    )
        :
        zone(name, addr, index),
        flipMap_(fm),
        zoneMesh_(zm),
        patchPtr_(nullptr),
        masterCellsPtr_(nullptr),
        slaveCellsPtr_(nullptr),
        mePtr_(nullptr)
    {
        checkAddressing();
    }


    faceZone::faceZone
    (
        const word& name,
        labelList&& addr,
        boolList&& fm,
        const label index,
        const faceZoneMesh& zm
    )
        :
        zone(name, std::move(addr), index),
        flipMap_(std::move(fm)),
        zoneMesh_(zm),
        patchPtr_(nullptr),
        masterCellsPtr_(nullptr),
        slaveCellsPtr_(nullptr),
        mePtr_(nullptr)
    {
        checkAddressing();
    }


    faceZone::faceZone
    (
        const word& name,
        const dictionary& dict,
        const label index,
        const faceZoneMesh& zm
    )
        :
        zone(name, dict, this->labelsName, index),
        flipMap_(dict.lookup("flipMap")),
        zoneMesh_(zm),
        patchPtr_(nullptr),
        masterCellsPtr_(nullptr),
        slaveCellsPtr_(nullptr),
        mePtr_(nullptr)
    {
        checkAddressing();
    }


    faceZone::faceZone
    (
        const faceZone& origZone,
        const labelUList& addr,
        const boolUList& fm,
        const label index,
        const faceZoneMesh& zm
    )
        :
        zone(origZone, addr, index),
        flipMap_(fm),
        zoneMesh_(zm),
        patchPtr_(nullptr),
        masterCellsPtr_(nullptr),
        slaveCellsPtr_(nullptr),
        mePtr_(nullptr)
    {
        checkAddressing();
    }


    faceZone::faceZone
    (
        const faceZone& origZone,
        labelList&& addr,
        boolList&& fm,
        const label index,
        const faceZoneMesh& zm
    )
        :
        zone(origZone, std::move(addr), index),
        flipMap_(std::move(fm)),
        zoneMesh_(zm),
        patchPtr_(nullptr),
        masterCellsPtr_(nullptr),
        slaveCellsPtr_(nullptr),
        mePtr_(nullptr)
    {
        checkAddressing();
    }


    // * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

    faceZone::~faceZone()
    {
        clearAddressing();
    }


    // * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

    label faceZone::whichFace(const label globalFaceID) const
    {
        return zone::localID(globalFaceID);
    }


    const primitiveFacePatch& faceZone::operator()() const
    {
        if (!patchPtr_)
        {
            calcFaceZonePatch();
        }

        return *patchPtr_;
    }


    const labelList& faceZone::masterCells() const
    {
        if (!masterCellsPtr_)
        {
            calcCellLayers();
        }

        return *masterCellsPtr_;
    }


    const labelList& faceZone::slaveCells() const
    {
        if (!slaveCellsPtr_)
        {
            calcCellLayers();
        }

        return *slaveCellsPtr_;
    }


    const labelList& faceZone::meshEdges() const
    {
        if (!mePtr_)
        {
            mePtr_ =
                new labelList
                (
                    operator()().meshEdges
                    (
                        zoneMesh().mesh().edges(),
                        zoneMesh().mesh().pointEdges()
                    )
                );
        }

        return *mePtr_;
    }


    void faceZone::clearAddressing()
    {
        zone::clearAddressing();

        deleteDemandDrivenData(patchPtr_);

        deleteDemandDrivenData(masterCellsPtr_);
        deleteDemandDrivenData(slaveCellsPtr_);

        deleteDemandDrivenData(mePtr_);
    }


    void faceZone::resetAddressing
    (
        const labelUList& addr,
        const bool flipMapValue
    )
    {
        clearAddressing();
        labelList::operator=(addr);
        setFlipMap(flipMapValue);
    }


    void faceZone::resetAddressing
    (
        const labelUList& addr,
        const boolUList& flipMap
    )
    {
        clearAddressing();
        labelList::operator=(addr);
        flipMap_ = flipMap;
    }


    void faceZone::resetAddressing
    (
        labelList&& addr,
        const bool flipMapValue
    )
    {
        clearAddressing();
        labelList::transfer(addr);
        setFlipMap(flipMapValue);
    }


    void faceZone::updateMesh(const mapPolyMesh& mpm)
    {
        clearAddressing();

        labelList newAddressing(size());
        boolList newFlipMap(flipMap_.size());
        label nFaces = 0;

        const labelList& addr = *this;
        const labelList& faceMap = mpm.reverseFaceMap();

        forAll(addr, i)
        {
            const label facei = addr[i];

            if (faceMap[facei] >= 0)
            {
                newAddressing[nFaces] = faceMap[facei];
                newFlipMap[nFaces] = flipMap_[i];       // Keep flip map.
                nFaces++;
            }
        }

        newAddressing.setSize(nFaces);
        newFlipMap.setSize(nFaces);

        transfer(newAddressing);
        flipMap_.transfer(newFlipMap);
    }


    bool faceZone::checkDefinition(const bool report) const
    {
        return zone::checkDefinition(zoneMesh().mesh().faces().size(), report);
    }


    bool faceZone::checkParallelSync(const bool report) const
    {
        const polyMesh& mesh = zoneMesh().mesh();
        const polyBoundaryMesh& bm = mesh.boundaryMesh();

        bool hasError = false;


        // Check that zone faces are synced
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        {
            const labelList& addr = *this;

            boolList neiZoneFace(mesh.nBoundaryFaces(), false);
            boolList neiZoneFlip(mesh.nBoundaryFaces(), false);

            forAll(addr, i)
            {
                const label facei = addr[i];

                if (!mesh.isInternalFace(facei))
                {
                    neiZoneFace[facei - mesh.nInternalFaces()] = true;
                    neiZoneFlip[facei - mesh.nInternalFaces()] = flipMap()[i];
                }
            }
            boolList myZoneFace(neiZoneFace);
            syncTools::swapBoundaryFaceList(mesh, neiZoneFace);
            boolList myZoneFlip(neiZoneFlip);
            syncTools::swapBoundaryFaceList(mesh, neiZoneFlip);

            forAll(addr, i)
            {
                const label facei = addr[i];
                const label patchi = bm.whichPatch(facei);

                if (patchi != -1 && bm[patchi].coupled())
                {
                    const label bFacei = facei - mesh.nInternalFaces();

                    // Check face in zone on both sides
                    if (myZoneFace[bFacei] != neiZoneFace[bFacei])
                    {
                        hasError = true;

                        if (report)
                        {
                            Pout << " ***Problem with faceZone " << index()
                                << " named " << name()
                                << ". Face " << facei
                                << " on coupled patch "
                                << bm[patchi].name()
                                << " is not consistent with its coupled neighbour."
                                << endl;
                        }
                        else
                        {
                            // w/o report - can stop checking now
                            break;
                        }
                    }
                    else if (myZoneFlip[bFacei] == neiZoneFlip[bFacei])
                    {
                        // Flip state should be opposite.
                        hasError = true;

                        if (report)
                        {
                            Pout << " ***Problem with faceZone " << index()
                                << " named " << name()
                                << ". Face " << facei
                                << " on coupled patch "
                                << bm[patchi].name()
                                << " does not have consistent flipMap"
                                << " across coupled faces."
                                << endl;
                        }
                        else
                        {
                            // w/o report - can stop checking now
                            break;
                        }
                    }
                }
            }
        }

        return returnReduce(hasError, orOp<bool>());
    }


    void faceZone::movePoints(const pointField& pts)
    {
        if (patchPtr_)
        {
            patchPtr_->movePoints(pts);
        }
    }

    void faceZone::write(Ostream& os) const
    {
        os << nl << name()
            << nl << static_cast<const labelList&>(*this)
            << nl << flipMap();
    }


    void faceZone::writeDict(Ostream& os) const
    {
        os.beginBlock(name());

        os.writeEntry("type", type());
        zoneIdentifier::write(os);
        writeEntry(this->labelsName, os);
        flipMap().writeEntry("flipMap", os);

        os.endBlock();
    }


    // * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

    Ostream& operator<<(Ostream& os, const faceZone& zn)
    {
        zn.write(os);
        os.check(FUNCTION_NAME);
        return os;
    }

}
// ************************************************************************* //
