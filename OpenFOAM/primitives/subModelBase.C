/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2016 OpenFOAM Foundation
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

#include "subModelBase.H"

// * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * * //
using namespace Foam;
bool subModelBase::subModelBase::inLine() const
{
    return (modelName_ != word::null);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

subModelBase::subModelBase(dictionary& properties) :    modelName_(word::null),
    properties_(properties),
    dict_(dictionary::null),
    baseName_(word::null),
    modelType_(word::null),
    coeffDict_(dictionary::null)
{}


subModelBase::subModelBase
(
    dictionary& properties,
    const dictionary& dict,
    const word& baseName,
    const word& modelType,
    const word& dictExt
) :    modelName_(word::null),
    properties_(properties),
    dict_(dict),
    baseName_(baseName),
    modelType_(modelType),
    coeffDict_(dict.subDict(modelType + dictExt))
{}


subModelBase::subModelBase
(
    const word& modelName,
    dictionary& properties,
    const dictionary& dict,
    const word& baseName,
    const word& modelType
) :    modelName_(modelName),
    properties_(properties),
    dict_(dict),
    baseName_(baseName),
    modelType_(modelType),
    coeffDict_(dict)
{}


subModelBase::subModelBase(const subModelBase& smb) :    modelName_(smb.modelName_),
    properties_(smb.properties_),
    dict_(smb.dict_),
    baseName_(smb.baseName_),
    modelType_(smb.modelType_),
    coeffDict_(smb.coeffDict_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

subModelBase::~subModelBase()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const word& subModelBase::modelName() const
{
    return modelName_;
}


const dictionary& subModelBase::dict() const
{
    return dict_;
}


const word& subModelBase::baseName() const
{
    return baseName_;
}


const word& subModelBase::modelType() const
{
    return modelType_;
}


const dictionary& subModelBase::coeffDict() const
{
    return coeffDict_;
}


const dictionary& subModelBase::properties() const
{
    return properties_;
}


bool subModelBase::defaultCoeffs(const bool printMsg) const
{
    bool def = coeffDict_.lookupOrDefault<bool>("defaultCoeffs", false);
    if (printMsg && def)
    {
        Info<< incrIndent;
        Info<< indent << "Employing default coefficients" << endl;
        Info<< decrIndent;
    }

    return def;
}


bool subModelBase::active() const
{
    return true;
}


void subModelBase::cacheFields(const bool)
{}


bool subModelBase::writeTime() const
{
    return active();
}


void subModelBase::write(Ostream& os) const
{
    os  << coeffDict_;
}


// ************************************************************************* //
