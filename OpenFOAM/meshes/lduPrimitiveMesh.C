﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2013-2017 OpenFOAM Foundation
    Copyright (C) 2019 OpenCFD Ltd.
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

#include "lduPrimitiveMesh.H"
#include "processorLduInterface.H"
#include "edgeHashes.H"
#include "labelPair.H"
#include "processorGAMGInterface.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(lduPrimitiveMesh, 0);

    //- Less operator for pairs of \<processor\>\<index\>
    class procLess
    {
        const labelPairList& list_;

    public:

        procLess(const labelPairList& list)
        :
            list_(list)
        {}

        bool operator()(const label a, const label b)
        {
            return list_[a].first() < list_[b].first();
        }
    };

 


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void lduPrimitiveMesh::checkUpperTriangular
(
    const label size,
    const labelUList& l,
    const labelUList& u
)
{
    forAll(l, facei)
    {
        if (u[facei] < l[facei])
        {
            FatalErrorInFunction
                << "Reversed face. Problem at face " << facei
                << " l:" << l[facei] << " u:" << u[facei]
                << abort(FatalError);
        }
        if (l[facei] < 0 || u[facei] < 0 || u[facei] >= size)
        {
            FatalErrorInFunction
                << "Illegal cell label. Problem at face " << facei
                << " l:" << l[facei] << " u:" << u[facei]
                << abort(FatalError);
        }
    }

    for (label facei=1; facei < l.size(); ++facei)
    {
        if (l[facei-1] > l[facei])
        {
            FatalErrorInFunction
                << "Lower not in incremental cell order."
                << " Problem at face " << facei
                << " l:" << l[facei] << " u:" << u[facei]
                << " previous l:" << l[facei-1]
                << abort(FatalError);
        }
        else if (l[facei-1] == l[facei])
        {
            // Same cell.
            if (u[facei-1] > u[facei])
            {
                FatalErrorInFunction
                    << "Upper not in incremental cell order."
                    << " Problem at face " << facei
                    << " l:" << l[facei] << " u:" << u[facei]
                    << " previous u:" << u[facei-1]
                    << abort(FatalError);
            }
        }
    }
}


labelListList lduPrimitiveMesh::globalCellCells
(
    const lduMesh& mesh,
    const globalIndex& globalNumbering
)
{
    const lduAddressing& addr = mesh.lduAddr();
    lduInterfacePtrsList interfaces = mesh.interfaces();

    const labelList globalIndices
    (
        identity
        (
            addr.size(),
            globalNumbering.localStart(UPstream::myProcNo(mesh.comm()))
        )
    );

    // Get the interface cells
    PtrList<labelList> nbrGlobalCells(interfaces.size());
    {
        // Initialise transfer of restrict addressing on the interface
        forAll(interfaces, inti)
        {
            if (interfaces.set(inti))
            {
                interfaces[inti].initInternalFieldTransfer
                (
                    Pstream::commsTypes::nonBlocking,
                    globalIndices
                );
            }
        }

        if (Pstream::parRun())
        {
            Pstream::waitRequests();
        }

        forAll(interfaces, inti)
        {
            if (interfaces.set(inti))
            {
                nbrGlobalCells.set
                (
                    inti,
                    new labelList
                    (
                        interfaces[inti].internalFieldTransfer
                        (
                            Pstream::commsTypes::nonBlocking,
                            globalIndices
                        )
                    )
                );
            }
        }
    }


    // Scan the neighbour list to find out how many times the cell
    // appears as a neighbour of the face. Done this way to avoid guessing
    // and resizing list
    labelList nNbrs(addr.size(), Zero);

    const labelUList& nbr = addr.upperAddr();
    const labelUList& own = addr.lowerAddr();

    {
        forAll(nbr, facei)
        {
            nNbrs[nbr[facei]]++;
            nNbrs[own[facei]]++;
        }

        forAll(interfaces, inti)
        {
            if (interfaces.set(inti))
            {
                for (const label celli : interfaces[inti].faceCells())
                {
                    nNbrs[celli]++;
                }
            }
        }
    }


    // Create cell-cells addressing
    labelListList cellCells(addr.size());

    forAll(cellCells, celli)
    {
        cellCells[celli].setSize(nNbrs[celli], -1);
    }

    // Reset the list of number of neighbours to zero
    nNbrs = 0;

    // Scatter the neighbour faces
    forAll(nbr, facei)
    {
        const label c0 = own[facei];
        const label c1 = nbr[facei];

        cellCells[c0][nNbrs[c0]++] = globalIndices[c1];
        cellCells[c1][nNbrs[c1]++] = globalIndices[c0];
    }
    forAll(interfaces, inti)
    {
        if (interfaces.set(inti))
        {
            const labelUList& faceCells = interfaces[inti].faceCells();

            forAll(faceCells, facei)
            {
                const label c0 = faceCells[facei];
                cellCells[c0][nNbrs[c0]++] = nbrGlobalCells[inti][facei];
            }
        }
    }

    return cellCells;
}


label lduPrimitiveMesh::totalSize
(
    const PtrList<lduPrimitiveMesh>& meshes
)
{
    label size = 0;

    for (const lduPrimitiveMesh& msh : meshes)
    {
        size += msh.lduAddr().size();
    }
    return size;
}


labelList lduPrimitiveMesh::upperTriOrder
(
    const label nCells,
    const labelUList& lower,
    const labelUList& upper
)
{
    labelList nNbrs(nCells, Zero);

    // Count number of upper neighbours
    forAll(lower, facei)
    {
        if (upper[facei] < lower[facei])
        {
            FatalErrorInFunction
                << "Problem at face:" << facei
                << " lower:" << lower[facei]
                << " upper:" << upper[facei]
                << exit(FatalError);
        }
        nNbrs[lower[facei]]++;
    }

    // Construct cell-upper cell addressing
    labelList offsets(nCells+1);
    offsets[0] = 0;
    forAll(nNbrs, celli)
    {
        offsets[celli+1] = offsets[celli]+nNbrs[celli];
    }

    nNbrs = offsets;

    labelList cellToFaces(offsets.last());
    forAll(upper, facei)
    {
        const label celli = lower[facei];
        cellToFaces[nNbrs[celli]++] = facei;
    }

    // Sort

    labelList oldToNew(lower.size());

    DynamicList<label> order;
    DynamicList<label> nbr;

    label newFacei = 0;

    for (label celli = 0; celli < nCells; ++celli)
    {
        const label startOfCell = offsets[celli];
        const label nNbr = offsets[celli+1] - startOfCell;

        nbr.resize(nNbr);
        order.resize(nNbr);

        forAll(nbr, i)
        {
            nbr[i] = upper[cellToFaces[offsets[celli]+i]];
        }
        sortedOrder(nbr, order);

        for (const label index : order)
        {
            oldToNew[cellToFaces[startOfCell + index]] = newFacei++;
        }
    }

    return oldToNew;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

lduPrimitiveMesh::lduPrimitiveMesh
(
    const label nCells,
    labelList& l,
    labelList& u,
    const label comm,
    bool reuse
)
:
    lduAddressing(nCells),
    lowerAddr_(l, reuse),
    upperAddr_(u, reuse),
    comm_(comm)
{}

void lduPrimitiveMesh::addInterfaces
(
    lduInterfacePtrsList& interfaces,
    const lduSchedule& ps
)
{
    interfaces_ = interfaces;
    patchSchedule_ = ps;

    // Create interfaces
    primitiveInterfaces_.setSize(interfaces_.size());
    forAll(interfaces_, i)
    {
        if (interfaces_.set(i))
        {
            primitiveInterfaces_.set(i, &interfaces_[i]);
        }
    }
}


lduPrimitiveMesh::lduPrimitiveMesh
(
    const label nCells
)
:
    lduAddressing(nCells),
    comm_(0)
{}


lduPrimitiveMesh::lduPrimitiveMesh
(
    const label nCells,
    labelList& l,
    labelList& u,
    PtrList<const lduInterface>& primitiveInterfaces,
    const lduSchedule& ps,
    const label comm
)
:
    lduAddressing(nCells),
    lowerAddr_(l, true),
    upperAddr_(u, true),
    primitiveInterfaces_(),
    patchSchedule_(ps),
    comm_(comm)
{
    primitiveInterfaces_.transfer(primitiveInterfaces);

    // Create interfaces
    interfaces_.setSize(primitiveInterfaces_.size());
    forAll(primitiveInterfaces_, i)
    {
        if (primitiveInterfaces_.set(i))
        {
            interfaces_.set(i, &primitiveInterfaces_[i]);
        }
    }
}


lduPrimitiveMesh::lduPrimitiveMesh
(
    const label comm,
    const labelList& procAgglomMap,

    const labelList& procIDs,
    const lduMesh& myMesh,
    const PtrList<lduPrimitiveMesh>& otherMeshes,

    labelList& cellOffsets,
    labelList& faceOffsets,
    labelListList& faceMap,
    labelListList& boundaryMap,
    labelListListList& boundaryFaceMap
)
:
    lduAddressing(myMesh.lduAddr().size() + totalSize(otherMeshes)),
    comm_(comm)
{
    const label currentComm = myMesh.comm();

    forAll(otherMeshes, i)
    {
        if (otherMeshes[i].comm() != currentComm)
        {
            WarningInFunction
                << "Communicator " << otherMeshes[i].comm()
                << " at index " << i
                << " differs from that of predecessor "
                << currentComm
                << endl;    //exit(FatalError);
        }
    }

    const label nMeshes = otherMeshes.size()+1;

    const label myAgglom = procAgglomMap[UPstream::myProcNo(currentComm)];

    if (lduPrimitiveMesh::debug)
    {
        Pout<< "I am " << UPstream::myProcNo(currentComm)
            << " agglomerating into " << myAgglom
            << " as are " << findIndices(procAgglomMap, myAgglom)
            << endl;
    }


    forAll(procIDs, i)
    {
        if (procAgglomMap[procIDs[i]] != procAgglomMap[procIDs[0]])
        {
            FatalErrorInFunction
                << "Processor " << procIDs[i]
                << " agglomerates to " << procAgglomMap[procIDs[i]]
                << " whereas other processors " << procIDs
                << " agglomerate to "
                << labelUIndList(procAgglomMap, procIDs)
                << exit(FatalError);
        }
    }


    // Cells get added in order.
    cellOffsets.setSize(nMeshes+1);
    cellOffsets[0] = 0;
    for (label procMeshI = 0; procMeshI < nMeshes; ++procMeshI)
    {
        const lduMesh& procMesh = mesh(myMesh, otherMeshes, procMeshI);

        cellOffsets[procMeshI+1] =
            cellOffsets[procMeshI]
          + procMesh.lduAddr().size();
    }


    // Faces initially get added in order, sorted later
    labelList internalFaceOffsets(nMeshes+1);
    internalFaceOffsets[0] = 0;
    for (label procMeshI = 0; procMeshI < nMeshes; ++procMeshI)
    {
        const lduMesh& procMesh = mesh(myMesh, otherMeshes, procMeshI);

        internalFaceOffsets[procMeshI+1] =
            internalFaceOffsets[procMeshI]
          + procMesh.lduAddr().lowerAddr().size();
    }

    // Count how faces get added. Interfaces inbetween get merged.

    // Merged interfaces: map from two coarse processors back to
    // - procMeshes
    // - interface in procMesh
    // (estimate size from number of patches of mesh0)
    EdgeMap<labelPairList> mergedMap(2*myMesh.interfaces().size());

    // Unmerged interfaces: map from two coarse processors back to
    // - procMeshes
    // - interface in procMesh
    EdgeMap<labelPairList> unmergedMap(mergedMap.size());

    boundaryMap.setSize(nMeshes);
    boundaryFaceMap.setSize(nMeshes);


    label nOtherInterfaces = 0;
    labelList nCoupledFaces(nMeshes, Zero);

    for (label procMeshI = 0; procMeshI < nMeshes; ++procMeshI)
    {
        const lduInterfacePtrsList interfaces =
            mesh(myMesh, otherMeshes, procMeshI).interfaces();

        // Initialise all boundaries as merged
        boundaryMap[procMeshI].setSize(interfaces.size(), -1);
        boundaryFaceMap[procMeshI].setSize(interfaces.size());

        // Get sorted order of processors
        forAll(interfaces, intI)
        {
            if (interfaces.set(intI))
            {
                const lduInterface& ldui = interfaces[intI];

                if (isA<processorLduInterface>(ldui))
                {
                    const processorLduInterface& pldui =
                        refCast<const processorLduInterface>(ldui);

                    label agglom0 = procAgglomMap[pldui.myProcNo()];
                    label agglom1 = procAgglomMap[pldui.neighbProcNo()];

                    const edge procEdge(agglom0, agglom1);

                    if (agglom0 != myAgglom && agglom1 != myAgglom)
                    {
                        FatalErrorInFunction
                            << "At mesh from processor " << procIDs[procMeshI]
                            << " have interface " << intI
                            << " with myProcNo:" << pldui.myProcNo()
                            << " with neighbProcNo:" << pldui.neighbProcNo()
                            << exit(FatalError);
                    }
                    else if (agglom0 == myAgglom && agglom1 == myAgglom)
                    {
                        // Merged interface
                        if (debug)
                        {
                            Pout<< "merged interface: myProcNo:"
                                << pldui.myProcNo()
                                << " nbr:" << pldui.neighbProcNo()
                                << " size:" << ldui.faceCells().size()
                                << endl;
                        }

                        const label nbrProcMeshI =
                            procIDs.find(pldui.neighbProcNo());

                        if (procMeshI < nbrProcMeshI)
                        {
                            // I am 'master' since get lowest numbered cells
                            nCoupledFaces[procMeshI] += ldui.faceCells().size();
                        }

                        mergedMap(procEdge).append
                        (
                            labelPair(procMeshI, intI)
                        );
                    }
                    else
                    {
                        if (debug)
                        {
                            Pout<< "external interface: myProcNo:"
                                << pldui.myProcNo()
                                << " nbr:" << pldui.neighbProcNo()
                                << " size:" << ldui.faceCells().size()
                                << endl;
                        }

                        unmergedMap(procEdge).append
                        (
                            labelPair(procMeshI, intI)
                        );
                    }
                }
                else
                {
                    // Still external (non proc) interface
                    FatalErrorInFunction
                        << "At mesh from processor " << procIDs[procMeshI]
                        << " have interface " << intI
                        << " of unhandled type " << interfaces[intI].type()
                        << exit(FatalError);

                    ++nOtherInterfaces;
                }
            }
        }
    }



    if (debug)
    {
        Pout<< "Remaining interfaces:" << endl;
        forAllConstIters(unmergedMap, iter)
        {
            Pout<< "    agglom procEdge:" << iter.key() << endl;
            const labelPairList& elems = iter.val();
            forAll(elems, i)
            {
                label procMeshI = elems[i][0];
                label interfacei = elems[i][1];
                const lduInterfacePtrsList interfaces =
                    mesh(myMesh, otherMeshes, procMeshI).interfaces();

                const processorLduInterface& pldui =
                    refCast<const processorLduInterface>
                    (
                        interfaces[interfacei]
                    );

                Pout<< "        proc:" << procIDs[procMeshI]
                    << " interfacei:" << interfacei
                    << " between:" << pldui.myProcNo()
                    << " and:" << pldui.neighbProcNo()
                    << endl;
            }
            Pout<< endl;
        }
    }
    if (debug)
    {
        Pout<< "Merged interfaces:" << endl;
        forAllConstIters(mergedMap, iter)
        {
            Pout<< "    agglom procEdge:" << iter.key() << endl;
            const labelPairList& elems = iter.val();

            forAll(elems, i)
            {
                label procMeshI = elems[i][0];
                label interfacei = elems[i][1];
                const lduInterfacePtrsList interfaces =
                    mesh(myMesh, otherMeshes, procMeshI).interfaces();
                const processorLduInterface& pldui =
                    refCast<const processorLduInterface>
                    (
                        interfaces[interfacei]
                    );

                Pout<< "        proc:" << procIDs[procMeshI]
                    << " interfacei:" << interfacei
                    << " between:" << pldui.myProcNo()
                    << " and:" << pldui.neighbProcNo()
                    << endl;
            }
            Pout<< endl;
        }
    }


    // Adapt faceOffsets for internal interfaces
    faceOffsets.setSize(nMeshes+1);
    faceOffsets[0] = 0;
    faceMap.setSize(nMeshes);
    for (label procMeshI = 0; procMeshI < nMeshes; ++procMeshI)
    {
        const lduMesh& procMesh = mesh(myMesh, otherMeshes, procMeshI);
        label nInternal = procMesh.lduAddr().lowerAddr().size();

        faceOffsets[procMeshI+1] =
            faceOffsets[procMeshI]
          + nInternal
          + nCoupledFaces[procMeshI];

        labelList& map = faceMap[procMeshI];
        map.setSize(nInternal);
        forAll(map, i)
        {
            map[i] = faceOffsets[procMeshI] + i;
        }
    }


    // Combine upper and lower
    lowerAddr_.setSize(faceOffsets.last(), -1);
    upperAddr_.setSize(lowerAddr_.size(), -1);


    // Old internal faces and resolved coupled interfaces
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    for (label procMeshI = 0; procMeshI < nMeshes; ++procMeshI)
    {
        const lduMesh& procMesh = mesh(myMesh, otherMeshes, procMeshI);

        const labelUList& l = procMesh.lduAddr().lowerAddr();
        const labelUList& u = procMesh.lduAddr().upperAddr();

        // Add internal faces
        label allFacei = faceOffsets[procMeshI];

        forAll(l, facei)
        {
            lowerAddr_[allFacei] = cellOffsets[procMeshI]+l[facei];
            upperAddr_[allFacei] = cellOffsets[procMeshI]+u[facei];
            allFacei++;
        }


        // Add merged interfaces
        const lduInterfacePtrsList interfaces = procMesh.interfaces();

        forAll(interfaces, intI)
        {
            if (interfaces.set(intI))
            {
                const lduInterface& ldui = interfaces[intI];

                if (isA<processorLduInterface>(ldui))
                {
                    const processorLduInterface& pldui =
                        refCast<const processorLduInterface>(ldui);

                    // Look up corresponding interfaces
                    label myP = pldui.myProcNo();
                    label nbrP = pldui.neighbProcNo();
                    label nbrProcMeshI = procIDs.find(nbrP);

                    if (procMeshI < nbrProcMeshI)
                    {
                        // I am 'master' since my cell numbers will be lower
                        // since cells get added in procMeshI order.

                        const label agglom0 = procAgglomMap[myP];
                        const label agglom1 = procAgglomMap[nbrP];

                        const auto fnd =
                            mergedMap.cfind(edge(agglom0, agglom1));

                        if (fnd.found())
                        {
                            const labelPairList& elems = fnd();

                            // Find nbrP in elems
                            label nbrIntI = -1;
                            forAll(elems, i)
                            {
                                label proci = elems[i][0];
                                label interfacei = elems[i][1];
                                const lduInterfacePtrsList interfaces =
                                    mesh
                                    (
                                        myMesh,
                                        otherMeshes,
                                        proci
                                    ).interfaces();
                                const processorLduInterface& pldui =
                                    refCast<const processorLduInterface>
                                    (
                                        interfaces[interfacei]
                                    );

                                if
                                (
                                    elems[i][0] == nbrProcMeshI
                                 && pldui.neighbProcNo() == procIDs[procMeshI]
                                )
                                {
                                    nbrIntI = elems[i][1];
                                    break;
                                }
                            }


                            if (nbrIntI == -1)
                            {
                                FatalErrorInFunction
                                    << "elems:" << elems << abort(FatalError);
                            }


                            const lduInterfacePtrsList nbrInterfaces = mesh
                            (
                                myMesh,
                                otherMeshes,
                                nbrProcMeshI
                            ).interfaces();


                            const labelUList& faceCells =
                                ldui.faceCells();
                            const labelUList& nbrFaceCells =
                                nbrInterfaces[nbrIntI].faceCells();

                            if (faceCells.size() != nbrFaceCells.size())
                            {
                                FatalErrorInFunction
                                    << "faceCells:" << faceCells
                                    << " nbrFaceCells:" << nbrFaceCells
                                    << abort(FatalError);
                            }


                            labelList& bfMap =
                                boundaryFaceMap[procMeshI][intI];
                            labelList& nbrBfMap =
                                boundaryFaceMap[nbrProcMeshI][nbrIntI];

                            bfMap.setSize(faceCells.size());
                            nbrBfMap.setSize(faceCells.size());

                            forAll(faceCells, pfI)
                            {
                                lowerAddr_[allFacei] =
                                    cellOffsets[procMeshI]+faceCells[pfI];
                                bfMap[pfI] = allFacei;
                                upperAddr_[allFacei] =
                                    cellOffsets[nbrProcMeshI]+nbrFaceCells[pfI];
                                nbrBfMap[pfI] = (-allFacei-1);
                                allFacei++;
                            }
                        }
                    }
                }
            }
        }
    }


    // Sort upper-tri order
    {
        labelList oldToNew
        (
            upperTriOrder
            (
                cellOffsets.last(), //nCells
                lowerAddr_,
                upperAddr_
            )
        );

        forAll(faceMap, procMeshI)
        {
            labelList& map = faceMap[procMeshI];
            forAll(map, i)
            {
                if (map[i] >= 0)
                {
                    map[i] = oldToNew[map[i]];
                }
                else
                {
                    label allFacei = -map[i]-1;
                    map[i] = -oldToNew[allFacei]-1;
                }
            }
        }


        inplaceReorder(oldToNew, lowerAddr_);
        inplaceReorder(oldToNew, upperAddr_);

        forAll(boundaryFaceMap, proci)
        {
            const labelList& bMap = boundaryMap[proci];
            forAll(bMap, intI)
            {
                if (bMap[intI] == -1)
                {
                    // Merged interface
                    labelList& bfMap = boundaryFaceMap[proci][intI];

                    forAll(bfMap, i)
                    {
                        if (bfMap[i] >= 0)
                        {
                            bfMap[i] = oldToNew[bfMap[i]];
                        }
                        else
                        {
                            label allFacei = -bfMap[i]-1;
                            bfMap[i] = (-oldToNew[allFacei]-1);
                        }
                    }
                }
            }
        }
    }


    // Kept interfaces
    // ~~~~~~~~~~~~~~~

    interfaces_.setSize(unmergedMap.size() + nOtherInterfaces);
    primitiveInterfaces_.setSize(interfaces_.size());

    label allInterfacei = 0;

    forAllConstIters(unmergedMap, iter)
    {
        const labelPairList& elems = iter.val();

        // Sort processors in increasing order so both sides walk through in
        // same order.
        labelPairList procPairs(elems.size());
        forAll(elems, i)
        {
            const labelPair& elem = elems[i];
            label procMeshI = elem[0];
            label interfacei = elem[1];
            const lduInterfacePtrsList interfaces = mesh
            (
                myMesh,
                otherMeshes,
                procMeshI
            ).interfaces();

            const processorLduInterface& pldui =
                refCast<const processorLduInterface>
                (
                    interfaces[interfacei]
                );
            label myProcNo = pldui.myProcNo();
            label nbrProcNo = pldui.neighbProcNo();
            procPairs[i] = labelPair
            (
                min(myProcNo, nbrProcNo),
                max(myProcNo, nbrProcNo)
            );
        }

        labelList order(sortedOrder(procPairs));

        // Count
        label n = 0;

        forAll(order, i)
        {
            const labelPair& elem = elems[order[i]];
            label procMeshI = elem[0];
            label interfacei = elem[1];
            const lduInterfacePtrsList interfaces = mesh
            (
                myMesh,
                otherMeshes,
                procMeshI
            ).interfaces();

            n += interfaces[interfacei].faceCells().size();
        }

        // Size
        labelField allFaceCells(n);
        labelField allFaceRestrictAddressing(n);
        n = 0;

        // Fill
        forAll(order, i)
        {
            const labelPair& elem = elems[order[i]];
            label procMeshI = elem[0];
            label interfacei = elem[1];
            const lduInterfacePtrsList interfaces = mesh
            (
                myMesh,
                otherMeshes,
                procMeshI
            ).interfaces();

            boundaryMap[procMeshI][interfacei] = allInterfacei;
            labelList& bfMap = boundaryFaceMap[procMeshI][interfacei];

            const labelUList& l = interfaces[interfacei].faceCells();
            bfMap.setSize(l.size());

            forAll(l, facei)
            {
                allFaceCells[n] = cellOffsets[procMeshI]+l[facei];
                allFaceRestrictAddressing[n] = n;
                bfMap[facei] = n;
                n++;
            }
        }


        // Find out local and remote processor in new communicator

        label neighbProcNo = -1;

        // See what the two processors map onto

        if (iter.key()[0] == myAgglom)
        {
            if (iter.key()[1] == myAgglom)
            {
                FatalErrorInFunction
                    << "problem procEdge:" << iter.key()
                    << exit(FatalError);
            }

            neighbProcNo = iter.key()[1];
        }
        else
        {
            if (iter.key()[1] != myAgglom)
            {
                FatalErrorInFunction
                    << "problem procEdge:" << iter.key()
                    << exit(FatalError);
            }

            neighbProcNo = iter.key()[0];
        }

        primitiveInterfaces_.set
        (
            allInterfacei,
            new processorGAMGInterface
            (
                allInterfacei,
                interfaces_,
                allFaceCells,
                allFaceRestrictAddressing,
                comm_,
                myAgglom,
                neighbProcNo,
                tensorField(),          // forwardT
                Pstream::msgType()      // tag
            )
        );
        interfaces_.set(allInterfacei, &primitiveInterfaces_[allInterfacei]);

        if (debug)
        {
            Pout<< "Created " << interfaces_[allInterfacei].type()
                << " interface at " << allInterfacei
                << " comm:" << comm_
                << " myProcNo:" << myAgglom
                << " neighbProcNo:" << neighbProcNo
                << " nFaces:" << allFaceCells.size()
                << endl;
        }


        allInterfacei++;
    }


    patchSchedule_ = nonBlockingSchedule<processorGAMGInterface>(interfaces_);

    if (debug)
    {
        checkUpperTriangular(cellOffsets.last(), lowerAddr_, upperAddr_);
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const lduMesh& lduPrimitiveMesh::mesh
(
    const lduMesh& myMesh,
    const PtrList<lduPrimitiveMesh>& otherMeshes,
    const label meshI
)
{
    return (meshI == 0 ? myMesh : otherMeshes[meshI-1]);
}


void lduPrimitiveMesh::gather
(
    const label comm,
    const lduMesh& mesh,
    const labelList& procIDs,
    PtrList<lduPrimitiveMesh>& otherMeshes
)
{
    // Force calculation of schedule (since does parallel comms)
    (void)mesh.lduAddr().patchSchedule();

    if (Pstream::myProcNo(comm) == procIDs[0])
    {
        otherMeshes.setSize(procIDs.size()-1);

        // Slave meshes
        for (label i = 1; i < procIDs.size(); ++i)
        {
            //Pout<< "on master :"
            //    << " receiving from slave " << procIDs[i] << endl;

            IPstream fromSlave
            (
                Pstream::commsTypes::scheduled,
                procIDs[i],
                0,          // bufSize
                Pstream::msgType(),
                comm
            );

            label nCells = readLabel(fromSlave);
            labelList lowerAddr(fromSlave);
            labelList upperAddr(fromSlave);
            boolList validInterface(fromSlave);


            // Construct mesh without interfaces
            otherMeshes.set
            (
                i-1,
                new lduPrimitiveMesh
                (
                    nCells,
                    lowerAddr,
                    upperAddr,
                    comm,
                    true    // reuse
                )
            );

            // Construct GAMGInterfaces
            lduInterfacePtrsList newInterfaces(validInterface.size());
            forAll(validInterface, intI)
            {
                if (validInterface[intI])
                {
                    word coupleType(fromSlave);

                    newInterfaces.set
                    (
                        intI,
                        GAMGInterface::New
                        (
                            coupleType,
                            intI,
                            otherMeshes[i-1].rawInterfaces(),
                            fromSlave
                        ).ptr()
                    );
                }
            }

            otherMeshes[i-1].addInterfaces
            (
                newInterfaces,
                nonBlockingSchedule<processorGAMGInterface>
                (
                    newInterfaces
                )
            );
       }
    }
    else if (procIDs.found(Pstream::myProcNo(comm)))
    {
        // Send to master

        const lduAddressing& addressing = mesh.lduAddr();
        lduInterfacePtrsList interfaces(mesh.interfaces());
        boolList validInterface(interfaces.size());
        forAll(interfaces, intI)
        {
            validInterface[intI] = interfaces.set(intI);
        }

        OPstream toMaster
        (
            Pstream::commsTypes::scheduled,
            procIDs[0],
            0,
            Pstream::msgType(),
            comm
        );

        toMaster
            << addressing.size()
            << addressing.lowerAddr()
            << addressing.upperAddr()
            << validInterface;

        forAll(interfaces, intI)
        {
            if (interfaces.set(intI))
            {
                const GAMGInterface& interface = refCast<const GAMGInterface>
                (
                    interfaces[intI]
                );

                toMaster << interface.type();
                interface.write(toMaster);
            }
        }
    }
}

}
// ************************************************************************* //
