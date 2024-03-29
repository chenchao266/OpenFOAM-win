﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
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

Class
    Foam::SHA1Digest

Description
    The SHA1 message digest.

See also
    Foam::SHA1

SourceFiles
    SHA1Digest.C

\*---------------------------------------------------------------------------*/

#ifndef SHA1Digest_H
#define SHA1Digest_H

#include <array>
#include <string>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward Declarations
class SHA1;
class Istream;
class Ostream;

/*---------------------------------------------------------------------------*\
                         Class SHA1Digest Declaration
\*---------------------------------------------------------------------------*/

class SHA1Digest
{
    // Private Data

        //- The digest contents, which has 20 (uncoded) bytes
        std::array<unsigned char, 20> dig_;


    // Private Member Functions

        //- Pointer to the underlying digest data
        unsigned char* data()
        {
            return dig_.data();
        }

        // Permit SHA1 to calculate the digest
        friend class SHA1;


public:


    // Static Data Members

        //- A null digest (ie, all zero)
        static const SHA1Digest null;


    // Constructors

        //- Construct a zero digest
        SHA1Digest();

        //- Read construct a digest
        explicit SHA1Digest(Istream& is);


    // Member Functions

        //- Reset the digest to zero
        void clear();

        //- Return true if the digest is empty (ie, all zero).
        bool empty() const;

        //- Return (40-byte) text representation, optionally with '_' prefix
        std::string str(const bool prefixed=false) const;

        //- Read (40-byte) text representation.
        //  Since leading and intermediate underscores are skipped, a '_' can
        //  be prefixed to the text representation to use an unquoted
        //  SHA1Digest without parsing ambiguities as a number.
        Istream& read(Istream& is);

        //- Write (40-byte) text representation, optionally with '_' prefix
        Ostream& write(Ostream& os, const bool prefixed=false) const;


    // Member Operators

        //- Equality operator
        bool operator==(const SHA1Digest&) const;

        //- Compare to (40-byte) text representation (eg, from sha1sum)
        //  An %empty string is equivalent to
        //  "0000000000000000000000000000000000000000"
        //  The hexdigits may optionally start with a '_' prefix
        bool operator==(const std::string& hexdigits) const;

        //- Compare to (40-byte) text representation (eg, from sha1sum)
        //  A %null or %empty string is equivalent to
        //  "0000000000000000000000000000000000000000"
        //  The hexdigits may optionally start with a '_' prefix
        bool operator==(const char* hexdigits) const;


        //- Inequality operator
        bool operator!=(const SHA1Digest&) const;

        //- Inequality operator
        bool operator!=(const std::string& hexdigits) const;

        //- Inequality operator
        bool operator!=(const char* hexdigits) const;
};


// IOstream Operators

//- Read (40-byte) text representation (ignoring leading '_' prefix)
Istream& operator>>(Istream&is , SHA1Digest& dig);

//- Write (40-byte) text representation, unquoted and without prefix
Ostream& operator<<(Ostream& os, const SHA1Digest& dig);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
