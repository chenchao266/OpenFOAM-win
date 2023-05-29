/*--------------------------------*- C++ -*----------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
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

#include "exprScanToken.H"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


 namespace Foam{
expressions::scanToken expressions::scanToken::null()
{
    scanToken tok;
    tok.type_ = LABEL;
    tok.labelValue = 0;

    return tok;
}


void expressions::scanToken::destroy()
{
    if (type_ == VECTOR)
    {
        delete vectorPtr;
        vectorPtr = nullptr;
    }
    else if (type_ == WORD)
    {
        delete wordPtr;
        wordPtr = nullptr;
    }
}


void expressions::scanToken::setLabel(label val)
{
    type_ = LABEL;
    labelValue = val;
}


void expressions::scanToken::setScalar(scalar val)
{
    type_ = SCALAR;
    scalarValue = val;
}


void expressions::scanToken::setVector(scalar x, scalar y, scalar z)
{
    type_ = VECTOR;
    vectorPtr = new vector(x, y, z);
}


void expressions::scanToken::setVector(const vector& val)
{
    type_ = VECTOR;
    vectorPtr = new vector(val);
}


void expressions::scanToken::setWord(const word& val)
{
    type_ = WORD;
    wordPtr = new word(val);
}


// ************************************************************************* //

 } // End namespace Foam
