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

#include "basicReactingCloud.H"

#include "makeParcelCloudFunctionObjects.H"

// Kinematic
#include "makeThermoParcelForces.H" // thermo variant
#include "makeParcelDispersionModels.H"
#include "makeReactingParcelInjectionModels.H" // Reacting variant
#include "makeParcelPatchInteractionModels.H"
#include "makeParcelStochasticCollisionModels.H"
#include "makeReactingParcelSurfaceFilmModels.H" // Reacting variant

// Thermodynamic
#include "makeParcelHeatTransferModels.H"

// Reacting
#include "makeReactingParcelCompositionModels.H"
#include "makeReactingParcelPhaseChangeModels.H"
#include "makeMPPICParcelDampingModels.H"
#include "makeMPPICParcelIsotropyModels.H"
#include "makeMPPICParcelPackingModels.H"
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
namespace Foam
{
    //defineTemplateTypeNameAndDebug(PackingModel<KinematicCloud<Cloud<basicReactingParcel>>>, 0);
    //defineTemplateTypeNameAndDebug(DampingModel<KinematicCloud<Cloud<basicReactingParcel>>>, 0);
    //defineTemplateTypeNameAndDebug(IsotropyModel<KinematicCloud<Cloud<basicReactingParcel>>>, 0);
    //defineRunTimeSelectionTable(PackingModel<KinematicCloud<Cloud<basicReactingParcel>>>, dictionary);
    //defineRunTimeSelectionTable(DampingModel<KinematicCloud<Cloud<basicReactingParcel>>>, dictionary);
    //defineRunTimeSelectionTable(IsotropyModel<KinematicCloud<Cloud<basicReactingParcel>>>, dictionary);
}
makeParcelCloudFunctionObjects(basicReactingCloud);

// Kinematic sub-models
makeThermoParcelForces(basicReactingCloud);
makeParcelDispersionModels(basicReactingCloud);
makeReactingParcelInjectionModels(basicReactingCloud);
makeParcelPatchInteractionModels(basicReactingCloud);
makeParcelStochasticCollisionModels(basicReactingCloud);
makeReactingParcelSurfaceFilmModels(basicReactingCloud);

// Thermo sub-models
makeParcelHeatTransferModels(basicReactingCloud);

// Reacting sub-models
makeReactingParcelCompositionModels(basicReactingCloud);
makeReactingParcelPhaseChangeModels(basicReactingCloud);


makeMPPICParcelDampingModels(basicReactingCloud);
makeMPPICParcelIsotropyModels(basicReactingCloud);
makeMPPICParcelPackingModels(basicReactingCloud);
// ************************************************************************* //
