﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2012-2018 Bernhard Gschaider
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

Namespace
    Foam::expressions::exprString

Description
    A variant of Foam::string with expansion of dictionary variables
    into a comma-separated form.

SourceFiles
    exprString.C
    exprStringI.H

SeeAlso
    Foam::exprTools::expressionEntry

\*---------------------------------------------------------------------------*/

#ifndef expressions_exprString_H
#define expressions_exprString_H

#include "_string.H"
#include "dictionary2.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace expressions
{

/*---------------------------------------------------------------------------*\
                           Class exprString Declaration
\*---------------------------------------------------------------------------*/

class exprString
:
    public string
{
public:

    // Constructors

        //- Default construct
        exprString() = default;

        //- Copy construct
        exprString(const exprString& rhs) = default;

        //- Move construct
        exprString(exprString&& rhs) = default;

        //- Copy construct from std::string
        inline explicit exprString(const std::string& s, bool doValidate=true);

        //- Move construct from std::string
        inline explicit exprString(std::string&& s, bool doValidate=true);

        //- Construct as copy of character array
        inline explicit exprString(const char* s, bool doValidate=true);

        //- Copy construct and expand with dictionary variables,
        //- and strip C/C++ comments from the input
        inline exprString
        (
            const std::string& str,
            const dictionary& dict,
            const bool stripComments = true
        );

        //- Move construct and expand with dictionary variables,
        //- and strip C/C++ comments from the input
        inline exprString
        (
            std::string&& str,
            const dictionary& dict,
            const bool stripComments = true
        );

        //- Construct from Istream and expand with dictionary variables,
        //- and strip C/C++ comments from the input
        inline exprString
        (
            Istream& is,
            const dictionary& dict,
            const bool stripComments = true
        );


    //- Destructor
    ~exprString() = default;


    // Static Member Functions

        //- Inplace expansion with dictionary variables,
        //- and strip C/C++ comments from the input.
        //
        // \par Expansion behaviour
        // - alternatives = True
        // - environment = True
        // - allow empty = True
        // - subDict = False
        // .
        static void inplaceExpand
        (
            std::string& str,
            const dictionary& dict,
            const bool stripComments = true
        );

        //- Get and expand expression with dictionary entries,
        //- optionally strip C/C++ comments from the input.
        // Expansion behaviour as per inplaceExpand
        static exprString getEntry
        (
            const word& keyword,  //!< Lookup key. Uses LITERAL (not REGEX)
            const dictionary& dict,
            const bool stripComments = true
        );

        //- Get and expand expression with dictionary entries,
        //- optionally strip C/C++ comments from the input.
        // Expansion behaviour as per inplaceExpand
        static exprString getOptional
        (
            const word& keyword,  //!< Lookup key. Uses LITERAL (not REGEX)
            const dictionary& dict,
            const bool stripComments = true
        );

        //- Copy convert string to exprString.
        //  No expansions, know what you are doing.
        inline static exprString toExpr(const std::string& str);

        //- Move convert string to exprString.
        //  No expansions, know what you are doing.
        inline static exprString toExpr(std::string&& str);


    // Member Functions

        //- Check for unexpanded '$' entries. Fatal if any exist.
        inline bool valid() const;

        //- Inplace expansion with dictionary variables,
        //- and strip C/C++ comments from the input
        void expand(const dictionary& dict, const bool stripComments = true);

        //- Inplace trim leading and trailing whitespace
        void trim();

        //- Read/expand entry with dictionary variables,
        //- and strip C/C++ comments from the input
        bool readEntry
        (
            const word& keyword,  //!< Lookup key. Uses LITERAL (not REGEX)
            const dictionary& dict,
            bool mandatory = true,
            const bool stripComments = true
        );


    // Member Operators

        //- Copy assign
        exprString& operator=(const exprString& str) = default;

        //- Move assign
        exprString& operator=(exprString&& str) = default;

        //- Copy assign from c-string. No expansions, no comment stripping
        inline exprString& operator=(const char* str);

        //- Copy assign from string. No expansions, no comment stripping
        inline exprString& operator=(const std::string& str);

        //- Move assign from string. No expansions, no comment stripping
        inline exprString& operator=(std::string&& str);


    // Write

        //- Dictionary entry for expression string, normally suppressing
        //- empty strings. Generally used verbatim output (readability)
        //  \return true if entry was written
        bool writeEntry
        (
            const word& keyword,
            Ostream& os,
            bool writeEmpty = false
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace expressions


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//- Hashing for exprString is the same as string
template<> struct Hash<expressions::exprString> : string::hasher {};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "exprStringI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
