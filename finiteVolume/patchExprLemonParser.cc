
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
    Lemon grammar for patch expressions.

    https://www.sqlite.org/src/doc/trunk/doc/lemon.html

    See detailed notes in the field expression parser.

\*---------------------------------------------------------------------------*/


/*
 * include patchExprLemonParserMacros.m4
 *include(`patchExprLemonParserMacros.m4')dnl
 !done in a comment since many editors have issues matching m4 quotes!
 */

#include "patchExprDriver.H"
#include "patchExprParser.H"
#include "patchExprScanner.H"
#include "unitConversion.H"
#include "volFields.H"
#include "error.H"
#include "IOmanip.H"
#include "exprOps.H"
#include "exprDriverOps.H"
#include "GeometricFieldOps.H"

// Enable ParseTrace
#undef NDEBUG
 

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::expressions::patchExpr::parser::stop()
{
    if (lemon_)
    {
 
        lemon_ = nullptr;
    }
}

Foam::word Foam::expressions::patchExpr::parser::tokenName(int tokenId)
{
    return word();
}

void Foam::expressions::patchExpr::parser::printTokenNames(Ostream& os)
{
}

void Foam::expressions::patchExpr::parser::printRules(Ostream& os)
{
}


void Foam::expressions::patchExpr::parser::start(parseDriver& driver_)
{
    this->stop();
 
    if ((debug & 0x4) || driver_.debugParser())
    {
 
    }
}


void Foam::expressions::patchExpr::parser::parse
(
    int tokenId,
    scanToken* tokenVal
)
{
 
}


 
// ************************************************************************* //
