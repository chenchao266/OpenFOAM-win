﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
    Copyright (C) 2021 OpenCFD Ltd.
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

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


 namespace Foam{
inline label tetIndices::cell() const
{
    return celli_;
}


inline label& tetIndices::cell()
{
    return celli_;
}


inline label tetIndices::face() const
{
    return facei_;
}


inline label& tetIndices::face()
{
    return facei_;
}


inline label tetIndices::tetPt() const
{
    return tetPti_;
}


inline label& tetIndices::tetPt()
{
    return tetPti_;
}


inline triFace tetIndices::faceTriIs
(
    const polyMesh& mesh,
    const bool warn
) const
{
    const ::Foam::face& f = mesh.faces()[face()];

    label faceBasePtI = mesh.tetBasePtIs()[face()];

    if (faceBasePtI < 0)
    {
        faceBasePtI = 0;

        if (warn)
        {
            if (nWarnings < maxNWarnings)
            {
                WarningInFunction
                    << "No base point for face " << face() << ", " << f
                    << ", produces a valid tet decomposition." << endl;
                ++nWarnings;
            }
            if (nWarnings == maxNWarnings)
            {
                Warning
                    << "Suppressing any further warnings." << endl;
                ++nWarnings;
            }
        }
    }

    label facePtI = (tetPti_ + faceBasePtI) % f.size();
    label faceOtherPtI = f.fcIndex(facePtI);

    if (mesh.faceOwner()[face()] != cell())
    {
        std::swap(facePtI, faceOtherPtI);
    }

    return triFace(f[faceBasePtI], f[facePtI], f[faceOtherPtI]);
}


inline triFace tetIndices::triIs
(
    const polyMesh& mesh,
    const bool warn
) const
{
    const ::Foam::face& f = mesh.faces()[face()];

    label faceBasePtI = mesh.tetBasePtIs()[face()];

    if (faceBasePtI < 0)
    {
        faceBasePtI = 0;

        if (warn)
        {
            if (nWarnings < maxNWarnings)
            {
                WarningInFunction
                    << "No base point for face " << face() << ", " << f
                    << ", produces a valid tet decomposition." << endl;
                ++nWarnings;
            }
            if (nWarnings == maxNWarnings)
            {
                Warning
                    << "Suppressing any further warnings." << endl;
                ++nWarnings;
            }
        }
    }

    label facePtI = (tetPti_ + faceBasePtI) % f.size();
    label faceOtherPtI = f.fcIndex(facePtI);

    if (mesh.faceOwner()[face()] != cell())
    {
        std::swap(facePtI, faceOtherPtI);
    }

    return triFace(faceBasePtI, facePtI, faceOtherPtI);
}


inline tetPointRef tetIndices::tet(const polyMesh& mesh) const
{
    const pointField& meshPoints = mesh.points();
    const triFace tri = faceTriIs(mesh);

    return tetPointRef
    (
        mesh.cellCentres()[cell()],
        meshPoints[tri[0]],
        meshPoints[tri[1]],
        meshPoints[tri[2]]
    );
}


inline triPointRef tetIndices::faceTri(const polyMesh& mesh) const
{
    const pointField& meshPoints = mesh.points();
    const triFace tri = faceTriIs(mesh);

    return triPointRef
    (
        meshPoints[tri[0]],
        meshPoints[tri[1]],
        meshPoints[tri[2]]
    );
}


inline triPointRef tetIndices::oldFaceTri
(
    const polyMesh& mesh
) const
{
    const pointField& meshOldPoints = mesh.oldPoints();
    const triFace tri = faceTriIs(mesh);

    return triPointRef
    (
        meshOldPoints[tri[0]],
        meshOldPoints[tri[1]],
        meshOldPoints[tri[2]]
    );
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline bool tetIndices::operator==(const tetIndices& rhs) const
{
    return
        cell() == rhs.cell()
     && face() == rhs.face()
     && tetPt() == rhs.tetPt();
}


inline bool tetIndices::operator!=(const tetIndices& rhs) const
{
    return !(*this == rhs);
}


// ************************************************************************* //

 } // End namespace Foam
