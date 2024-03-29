﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
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
inline const Map<label>& cellMatcher::localPoint() const
{
    return localPoint_;
}


inline const faceList& cellMatcher::localFaces() const
{
    return localFaces_;
}


inline const labelList& cellMatcher::faceSize() const
{
    return faceSize_;
}


inline const labelList& cellMatcher::pointMap() const
{
    return pointMap_;
}


inline const labelList& cellMatcher::faceMap() const
{
    return faceMap_;
}


inline const labelList& cellMatcher::edgeFaces() const
{
    return edgeFaces_;
}


inline const labelListList& cellMatcher::pointFaceIndex() const
{
    return pointFaceIndex_;
}


inline const labelList& cellMatcher::vertLabels() const
{
    return vertLabels_;
}


inline const labelList& cellMatcher::faceLabels() const
{
    return faceLabels_;
}


inline const cellModel& cellMatcher::model() const
{
    if (cellModelPtr_ == nullptr)
    {
        cellModelPtr_ = cellModel::ptr(cellModelName_);
    }
    return *cellModelPtr_;
}


// Key into edgeFaces_. key and key+1 are the entries for edge going from
// v0 to v1
inline label cellMatcher::edgeKey
(
    const label numVert,
    const label v0,
    const label v1
)
{
    return 2*(v0*numVert + v1);
}


// Walk along face consistent with face orientation
inline label cellMatcher::nextVert
(
    const label localVertI,
    const label size,
    const bool rightHand
)
{
    if (rightHand)
    {
        // face oriented acc. to righthand rule
        return (localVertI + 1) % size;
    }
    else
    {
        // face oriented acc. to lefthand rule
        return (size + localVertI - 1) % size;
    }
}


// ************************************************************************* //

 } // End namespace Foam
