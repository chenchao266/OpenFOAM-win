/*--------------------------------*- C++ -*----------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2016-2018 OpenCFD Ltd.
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
    Flex-based parsing of STL ASCII format

\*---------------------------------------------------------------------------*/
 
 /* ------------------------------------------------------------------------ *\
   ------ local definitions
 \* ------------------------------------------------------------------------ */

#include "STLAsciiParse.H"
#include "STLReader.H"
#include "OSspecific.H"

// Flex may use register, which is deprecated and incompatible with C++17
#pragma clang diagnostic ignored "-Wdeprecated-register"

using namespace Foam;
 

//- A lexer for parsing STL ASCII files.
//  Returns DynamicList(s) of points and facets (zoneIds).
//  The facets are within a solid/endsolid grouping
class STLAsciiParseFlex
:
    public Detail::STLAsciiParse 
{
    word startError_;

public:

    // Constructors

    //- From input stream and the approximate number of vertices in the STL
    STLAsciiParseFlex(istream* is, const label approxNpoints)
    :
        Detail::STLAsciiParse(approxNpoints)
    {}


    // Member Functions

    //- The lexer function itself
    int lex() 
    {
        return 0;
    }

    //- Execute lexer
    void execute()
    {
 
    }
};


 /* ------------------------------------------------------------------------ *\
   ------ cppLexer::yylex()
 \* ------------------------------------------------------------------------ */
 
//
// Member Function
//
bool Foam::fileFormats::STLReader::readAsciiFlex
(
    const fileName& filename
)
{
    IFstream is(filename);
    if (!is)
    {
        FatalErrorInFunction
            << "file " << filename << " not found"
            << exit(FatalError);
    }

    // Create with approx number of vertices in the STL (from file size)
    STLAsciiParseFlex lexer(&(is.stdStream()), Foam::fileSize(filename)/400);
    lexer.execute();

    transfer(lexer);

    return true;
}
 