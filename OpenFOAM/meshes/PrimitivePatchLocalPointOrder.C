﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
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

Description
     Orders the local points on the patch for most efficient search

\*---------------------------------------------------------------------------*/

#include "SLList.H"
#include "boolList.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

namespace Foam {
template<class FaceList, class PointField>
void
PrimitivePatch<FaceList, PointField>::calcLocalPointOrder() const
{
    // Note: Cannot use bandCompressing as point-point addressing does
    // not exist and is not considered generally useful.

    DebugInFunction << "Calculating local point order" << endl;

    if (localPointOrderPtr_)
    {
        // An error to recalculate if already allocated
        FatalErrorInFunction
            << "local point order already calculated"
            << abort(FatalError);
    }

    const List<face_type>& lf = localFaces();

    const labelListList& ff = faceFaces();

    boolList visitedFace(lf.size(), false);

    localPointOrderPtr_.reset(new labelList(meshPoints().size(), -1));
    auto& pointOrder = *localPointOrderPtr_;

    boolList visitedPoint(pointOrder.size(), false);

    label nPoints = 0;

    forAll(lf, facei)
    {
        if (!visitedFace[facei])
        {
            SLList<label> faceOrder(facei);

            do
            {
                const label curFace = faceOrder.first();

                faceOrder.removeHead();

                if (!visitedFace[curFace])
                {
                    visitedFace[curFace] = true;

                    const labelList& curPoints = lf[curFace];

                    // mark points
                    forAll(curPoints, pointi)
                    {
                        if (!visitedPoint[curPoints[pointi]])
                        {
                            visitedPoint[curPoints[pointi]] = true;

                            pointOrder[nPoints] = curPoints[pointi];

                            nPoints++;
                        }
                    }

                    // add face neighbours to the list
                    const labelList& nbrs = ff[curFace];

                    forAll(nbrs, nbrI)
                    {
                        if (!visitedFace[nbrs[nbrI]])
                        {
                            faceOrder.append(nbrs[nbrI]);
                        }
                    }
                }
            } while (faceOrder.size());
        }
    }

    DebugInfo << "Calculated local point order" << endl;
}


// ************************************************************************* //

 } // End namespace Foam
