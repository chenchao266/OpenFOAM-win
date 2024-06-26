/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2013-2015 OpenFOAM Foundation
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

#include "basicKinematicMPPICCloud.H"

#include "makeParcelCloudFunctionObjects.H"

// Kinematic sub-models
#include "makeParcelForces.H"
#include "makeParcelDispersionModels.H"
#include "makeParcelInjectionModels.H"
#include "makeParcelPatchInteractionModels.H"
#include "makeParcelStochasticCollisionModels.H"
#include "makeParcelSurfaceFilmModels.H"

// MPPIC sub-models
#include "makeMPPICParcelDampingModels.H"
#include "makeMPPICParcelIsotropyModels.H"
#include "makeMPPICParcelPackingModels.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
namespace Foam
{
    //defineTemplateTypeNameAndDebug(PackingModel<KinematicCloud<Cloud<basicKinematicMPPICParcel>>>, 0);
    //defineTemplateTypeNameAndDebug(DampingModel<KinematicCloud<Cloud<basicKinematicMPPICParcel>>>, 0);
    //defineTemplateTypeNameAndDebug(IsotropyModel<KinematicCloud<Cloud<basicKinematicMPPICParcel>>>, 0);
    //defineRunTimeSelectionTable(PackingModel<KinematicCloud<Cloud<basicKinematicMPPICParcel>>>, dictionary);
    //defineRunTimeSelectionTable(DampingModel<KinematicCloud<Cloud<basicKinematicMPPICParcel>>>, dictionary);
    //defineRunTimeSelectionTable(IsotropyModel<KinematicCloud<Cloud<basicKinematicMPPICParcel>>>, dictionary);
}
makeParcelCloudFunctionObjects(basicKinematicMPPICCloud);

// Kinematic sub-models
makeParcelForces(basicKinematicMPPICCloud);

makeParcelDispersionModels(basicKinematicMPPICCloud);
makeParcelInjectionModels(basicKinematicMPPICCloud);
makeParcelPatchInteractionModels(basicKinematicMPPICCloud);
makeParcelStochasticCollisionModels(basicKinematicMPPICCloud);
makeParcelSurfaceFilmModels(basicKinematicMPPICCloud);

// MPPIC sub-models
makeMPPICParcelDampingModels(basicKinematicMPPICCloud);
makeMPPICParcelIsotropyModels(basicKinematicMPPICCloud);
makeMPPICParcelPackingModels(basicKinematicMPPICCloud);


// ************************************************************************* //
