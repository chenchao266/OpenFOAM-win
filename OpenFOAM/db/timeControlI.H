/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016 OpenFOAM Foundation
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

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


 namespace Foam{
inline const Time& timeControl::time() const
{
    return time_;
}


inline const word& timeControl::name() const
{
    return prefix_;
}


inline const word& timeControl::type() const
{
    return controlNames_[timeControl_];
}


inline timeControl::timeControls timeControl::control() const
{
    return timeControl_;
}


inline bool timeControl::always() const
{
    return
    (
        (timeControls::ocAlways == timeControl_)
     || (timeControls::ocTimeStep == timeControl_ && intInterval_ <= 1)
    );
}


inline scalar timeControl::interval() const
{
    return interval_;
}


inline label timeControl::executionIndex() const
{
    return executionIndex_;
}


// ************************************************************************* //

 } // End namespace Foam
