/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2015 OpenFOAM Foundation
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

#include "basicKinematicCloud.H"

#include "makeParcelCloudFunctionObjects.H"

// Kinematic
#include "makeParcelForces.H"
#include "makeParcelDispersionModels.H"
#include "makeParcelInjectionModels.H"
#include "makeParcelPatchInteractionModels.H"
#include "makeParcelStochasticCollisionModels.H"
#include "makeParcelSurfaceFilmModels.H"
#include "makeMPPICParcelDampingModels.H"
#include "makeMPPICParcelIsotropyModels.H"
#include "makeMPPICParcelPackingModels.H"
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
namespace Foam
{
    //defineTemplateTypeNameAndDebug(PackingModel<KinematicCloud<Cloud<basicKinematicParcel>>>, 0);
    //defineTemplateTypeNameAndDebug(DampingModel<KinematicCloud<Cloud<basicKinematicParcel>>>, 0);
    //defineTemplateTypeNameAndDebug(IsotropyModel<KinematicCloud<Cloud<basicKinematicParcel>>>, 0);
    //defineRunTimeSelectionTable(PackingModel<KinematicCloud<Cloud<basicKinematicParcel>>>, dictionary);
    //defineRunTimeSelectionTable(DampingModel<KinematicCloud<Cloud<basicKinematicParcel>>>, dictionary);
    //defineRunTimeSelectionTable(IsotropyModel<KinematicCloud<Cloud<basicKinematicParcel>>>, dictionary);
}
makeParcelCloudFunctionObjects(basicKinematicCloud);

// Kinematic sub-models
makeParcelForces(basicKinematicCloud);
makeParcelDispersionModels(basicKinematicCloud);
makeParcelInjectionModels(basicKinematicCloud);
makeParcelPatchInteractionModels(basicKinematicCloud);
makeParcelStochasticCollisionModels(basicKinematicCloud);
makeParcelSurfaceFilmModels(basicKinematicCloud);

makeMPPICParcelDampingModels(basicKinematicCloud);
makeMPPICParcelIsotropyModels(basicKinematicCloud);
makeMPPICParcelPackingModels(basicKinematicCloud);
// ************************************************************************* //
