/*--------------------------------*- C++ -*----------------------------------*\
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

Description
    Lemon grammar for volume expressions.

    https://www.sqlite.org/src/doc/trunk/doc/lemon.html

    See detailed notes in the field expression parser.

\*---------------------------------------------------------------------------*/

/*
 * include volumeExprLemonParserMacros.m4
 *include(`volumeExprLemonParserMacros.m4')dnl
 !done in a comment since many editors have issues matching m4 quotes!
 */
 
#include "volumeExprDriver.H"
#include "volumeExprParser.H"
#include "volumeExprScanner.H"
#include "volFields.H"
#include "surfaceFields.H"
#include "pointFields.H"
#include "unitConversion.H"
#include "error.H"
#include "IOmanip.H"
#include "exprOps.H"
#include "exprDriverOps.H"
#include "GeometricFieldOps.H"
#include "fvcReconstruct.H"


// Enable ParseTrace
#undef NDEBUG


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::expressions::volumeExpr::parser::stop()
{
    if (lemon_)
    {

        lemon_ = nullptr;
    }
}

Foam::word Foam::expressions::volumeExpr::parser::tokenName(int tokenId)
{
    return word();
}

void Foam::expressions::volumeExpr::parser::printTokenNames(Ostream& os)
{
}

void Foam::expressions::volumeExpr::parser::printRules(Ostream& os)
{
}


void Foam::expressions::volumeExpr::parser::start(parseDriver& driver_)
{
    this->stop();

    if ((debug & 0x4) || driver_.debugParser())
    {

    }
}


void Foam::expressions::volumeExpr::parser::parse
(
    int tokenId,
    scanToken* tokenVal
)
{

}

 

// ************************************************************************* //
