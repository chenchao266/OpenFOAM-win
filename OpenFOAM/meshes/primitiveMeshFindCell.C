﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2015 OpenFOAM Foundation
    Copyright (C) 2017 OpenCFD Ltd.
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

#include "primitiveMesh.H"
#include "cell.H"
#include "boundBox.H"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


 namespace Foam{
bool primitiveMesh::pointInCellBB
(
    const point& p,
    label celli,
    scalar inflationFraction
) const
{
    boundBox bb
    (
        cells()[celli].points
        (
            faces(),
            points()
        ),
        false
    );

    if (inflationFraction > SMALL)
    {
        bb.inflate(inflationFraction);
    }

    return bb.contains(p);
}


bool primitiveMesh::pointInCell(const point& p, label celli) const
{
    const labelList& f = cells()[celli];
    const labelList& owner = this->faceOwner();
    const vectorField& cf = faceCentres();
    const vectorField& Sf = faceAreas();

    forAll(f, facei)
    {
        label nFace = f[facei];
        vector proj = p - cf[nFace];
        vector normal = Sf[nFace];
        if (owner[nFace] != celli)
        {
            normal = -normal;
        }

        if ((normal & proj) > 0)
        {
            return false;
        }
    }

    return true;
}


label primitiveMesh::findNearestCell(const point& location) const
{
    const vectorField& centres = cellCentres();

    if (!centres.size())
    {
        return -1;
    }

    label nearestCelli = 0;
    scalar minProximity = magSqr(centres[0] - location);

    for (label celli = 1; celli < centres.size(); celli++)
    {
        scalar proximity = magSqr(centres[celli] - location);

        if (proximity < minProximity)
        {
            nearestCelli = celli;
            minProximity = proximity;
        }
    }

    return nearestCelli;
}


label primitiveMesh::findCell(const point& location) const
{
    if (nCells() == 0)
    {
        return -1;
    }

    // Find the nearest cell centre to this location
    label celli = findNearestCell(location);

    // If point is in the nearest cell return
    if (pointInCell(location, celli))
    {
        return celli;
    }
    else // point is not in the nearest cell so search all cells
    {
        bool cellFound = false;
        label n = 0;

        while ((!cellFound) && (n < nCells()))
        {
            if (pointInCell(location, n))
            {
                cellFound = true;
                celli = n;
            }
            else
            {
                n++;
            }
        }
        if (cellFound)
        {
            return celli;
        }
        else
        {
            return -1;
        }
    }
}


// ************************************************************************* //

 } // End namespace Foam
