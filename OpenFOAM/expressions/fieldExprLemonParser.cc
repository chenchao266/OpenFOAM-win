/*--------------------------------*- C++ -*----------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2019-2021 OpenCFD Ltd.
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
    Lemon grammar for field expressions.

    https://www.sqlite.org/src/doc/trunk/doc/lemon.html

    This code is leans heavily on the features included in the
    swak4Foam bison code by Bernhard Gschaider <bgschaid@hfd-research.com>
    but is a complete rewrite.

    The generated parser is localized in an anonymous namespace.
    Interface code wrapping is near the bottom of the file.

    Field naming conventions:
        'l' (Logical), 's' (Scalar), 'v' (Vector),
        'h' (spHerical), 'y' (sYmmetrical), 't' (Tensor)
    Prefixes
        '' (volume), 's' (surface), 'p' (point)

    Thus "tfield" is a volTensorField, "psfield" is a pointScalarField.

Notes
    The grammar productions rules are indeed long-winded. There are standard
    field types (scalar, vector, tensor, ...), an additional logical field
    type (stored internally as a scalar).
    For patches there will be face/point versions of each.
    For volume fields, there will be volume/surface/point versions of each.

    The grammar is thus wrapped with m4(1) macros. Some caution may be
    required with m4-specific words such as `format' etc and careful
    attention must be paid to quoting when defining the macros.

    The "declare_field" macro emits the parse token type for Lemon and
    defines various accessor macros:
       _new_NNfield, _get_NNfield, _value_type_NNfield

    In some cases, for example where the support for point fields (eg,
    pointScalarField) is incomplete or not yet defined, operations are
    restricted to the internal fields. For this, two macros will be defined:
    "field_read_access" and "field_write_access". The default versions of
    these should be defined to pass through their arguments, with workaround
    versions adding primitiveField(), primitiveFieldRef() methods to wrap the
    access.

    To further allow for intervention, the macros are structured to permit
    overrides of the individual components prior to invoking. These means that
    the most general rules appear first with the component implementation
    delayed until later.

TBD
    Modulo:
      - currently uses the standard definition, not the swak definition

    Hodge dual:
    Double inner-product:
      - Ignored for the moment.

    Outer-product:
      - Common mistake to use '*' when meaning '&' for the inner-product.
        Perhaps ignore or offer some other syntax such
          "a [inner] b"
          "a [outer] b"
          "a [cross] b"

\*---------------------------------------------------------------------------*/
 
 
#include "exprScanToken.H"
#include "fieldExprDriver.H"
#include "fieldExprParser.H"
#include "fieldExprScanner.H"
#include "unitConversion.H"
#include "error.H"
#include "IOmanip.H"
#include "exprOps.H"
#include "exprDriverOps.H"

// Enable ParseTrace
#undef NDEBUG
 

// ------------------------------------------------------------------------- //

// File-scope visibility for exposed Lemon parser routines
 
// Use extra argument for the return value

 namespace Foam{ 

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void expressions::fieldExpr::parser::stop()
{
    if (lemon_)
    {
 
        lemon_ = nullptr;
    }
}


void expressions::fieldExpr::parser::start(parseDriver& driver_)
{
    this->stop();
 

    if ((debug & 0x4) || driver_.debugParser())
    {
 
    }
}


void expressions::fieldExpr::parser::parse(int tokenId)
{
 
}


void expressions::fieldExpr::parser::parse(int tokenId, scanToken tok)
{
 
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
 

// ************************************************************************* //

 } // End namespace Foam
