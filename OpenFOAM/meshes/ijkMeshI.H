/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
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

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


 namespace Foam{
inline ijkMesh::ijkMesh()
:
    ijkAddressing()
{}


inline ijkMesh::ijkMesh(const labelVector& ijk)
:
    ijkAddressing(ijk)
{}


inline ijkMesh::ijkMesh
(
    const label nx,
    const label ny,
    const label nz
)
:
    ijkAddressing(nx, ny, nz)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline label ijkMesh::nPoints() const
{
    if (ijkAddressing::empty())
    {
        return 0;
    }

    const labelVector& n = ijkAddressing::sizes();

    return ((n.x()+1) * (n.y()+1) * (n.z()+1));
}


inline label ijkMesh::nCells() const
{
    return ijkAddressing::size();
}


inline label ijkMesh::nFaces() const
{
    if (ijkAddressing::empty())
    {
        return 0;
    }

    const labelVector& n = ijkAddressing::sizes();

    return
    (
        ((n.x()+1) * n.y() * n.z())
      + ((n.y()+1) * n.z() * n.x())
      + ((n.z()+1) * n.x() * n.y())
    );
}


inline label ijkMesh::nInternalFaces() const
{
    if (ijkAddressing::empty())
    {
        return 0;
    }

    const labelVector& n = ijkAddressing::sizes();

    return
    (
        ((n.x()-1) * n.y() * n.z())
      + ((n.y()-1) * n.z() * n.x())
      + ((n.z()-1) * n.x() * n.y())
    );
}


inline label ijkMesh::nBoundaryFaces() const
{
    if (ijkAddressing::empty())
    {
        return 0;
    }

    const labelVector& n = ijkAddressing::sizes();

    return
    (
        (2 * n.y() * n.z())
      + (2 * n.z() * n.x())
      + (2 * n.x() * n.y())
    );
}


inline label ijkMesh::nBoundaryFaces
(
    const direction shapeFacei
) const
{
    if (ijkAddressing::empty())
    {
        return 0;
    }

    const labelVector& n = ijkAddressing::sizes();

    switch (shapeFacei)
    {
        // Face 0,1 == x-min, x-max
        case 0:
        case 1:
            return n.y()*n.z();
            break;

        // Face 2,3 == y-min, y-max
        case 2:
        case 3:
            return n.z()*n.x();
            break;

        // Face 4,5 == z-min, z-max
        case 4:
        case 5:
            return n.x()*n.y();
            break;
    }

    return 0;
}


inline label ijkMesh::cellLabel
(
    const label i,
    const label j,
    const label k
) const
{
    return ijkAddressing::index(i, j, k);
}


inline label ijkMesh::cellLabel(const labelVector& ijk) const
{
    return ijkAddressing::index(ijk);
}


inline label ijkMesh::pointLabel
(
    const label i,
    const label j,
    const label k
) const
{
    #ifdef FULLDEBUG
    checkIndex(i, j, k, true);
    #endif

    const labelVector& n = sizes();

    return (i + ((n.x()+1) * (j + (n.y()+1) * k)));
}


label ijkMesh::pointLabel(const labelVector& ijk) const
{
    #ifdef FULLDEBUG
    checkIndex(ijk, true);
    #endif

    const labelVector& n = sizes();

    return (ijk.x() + ((n.x()+1) * (ijk.y() + (n.y()+1) * ijk.z())));
}


// ************************************************************************* //

 } // End namespace Foam
