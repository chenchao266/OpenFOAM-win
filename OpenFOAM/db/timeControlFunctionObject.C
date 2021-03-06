﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2016 OpenFOAM Foundation
     \\/     M anipulation  |
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

#include "timeControlFunctionObject.H"
#include "polyMesh.H"
#include "mapPolyMesh.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //
 
namespace Foam
{
namespace functionObjects
{
    defineTypeNameAndDebug(timeControl, 0);
}
}


// * * * * * * * * * * * * * * * Private Members * * * * * * * * * * * * * * //
namespace Foam
{
    namespace functionObjects
    {
        void timeControl::readControls()
        {
            dict_.readIfPresent("timeStart", timeStart_);
            dict_.readIfPresent("timeEnd", timeEnd_);
            dict_.readIfPresent("nStepsToStartTimeChange", nStepsToStartTimeChange_);
        }


        bool timeControl::active() const
        {
            return
                time_.value() >= timeStart_
                && time_.value() <= timeEnd_;
        }


        // * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

        timeControl::timeControl
        (
            const word& name,
            const Time& t,
            const dictionary& dict
        ) : functionObject(name),
            time_(t),
            dict_(dict),
            timeStart_(-VGREAT),
            timeEnd_(VGREAT),
            nStepsToStartTimeChange_
            (
                dict.lookupOrDefault("nStepsToStartTimeChange", 3)
            ),
            executeControl_(t, dict, "execute"),
            writeControl_(t, dict, "write"),
            foPtr_(functionObject::New(name, t, dict_))
        {
            readControls();
        }


        // * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

        bool timeControl::execute()
        {
            if (active() && (postProcess || executeControl_.execute()))
            {
                foPtr_->execute();
            }

            return true;
        }


        bool timeControl::write()
        {
            if (active() && (postProcess || writeControl_.execute()))
            {
                foPtr_->write();
            }

            return true;
        }


        bool timeControl::end()
        {
            if (active() && (executeControl_.execute() || writeControl_.execute()))
            {
                foPtr_->end();
            }

            return true;
        }



        bool timeControl::adjustTimeStep()
        {
            if
                (
                    active()
                    && writeControl_.control()
                    == ::Foam::timeControl::ocAdjustableRunTime
                    )
            {
                const label  writeTimeIndex = writeControl_.executionIndex();
                const scalar writeInterval = writeControl_.interval();

                scalar timeToNextWrite = max
                (
                    0.0,
                    (writeTimeIndex + 1)*writeInterval
                    - (time_.value() - time_.startTime().value())
                );

                scalar deltaT = time_.deltaTValue();

                scalar nSteps = timeToNextWrite / deltaT - SMALL;

                // functionObjects modify deltaT within nStepsToStartTimeChange
                // NOTE: Potential problems arise if two function objects dump within
                // the same interval
                if (nSteps < nStepsToStartTimeChange_)
                {
                    label nStepsToNextWrite = label(nSteps) + 1;

                    scalar newDeltaT = timeToNextWrite / nStepsToNextWrite;

                    // Adjust time step
                    if (newDeltaT < deltaT)
                    {
                        deltaT = max(newDeltaT, 0.2*deltaT);
                        const_cast<Time&>(time_).setDeltaT(deltaT, false);
                    }
                }
            }

            return true;
        }


        bool timeControl::read
        (
            const dictionary& dict
        )
        {
            if (dict != dict_)
            {
                dict_ = dict;

                writeControl_.read(dict);
                executeControl_.read(dict);
                readControls();

                return true;
            }
            else
            {
                return false;
            }
        }


        void timeControl::updateMesh
        (
            const mapPolyMesh& mpm
        )
        {
            if (active())
            {
                foPtr_->updateMesh(mpm);
            }
        }


        void timeControl::movePoints
        (
            const polyMesh& mesh
        )
        {
            if (active())
            {
                foPtr_->movePoints(mesh);
            }
        }

    }
}

// ************************************************************************* //
