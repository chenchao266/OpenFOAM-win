/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016-2021 OpenCFD Ltd.
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
    Foam::ensightReadFile

Description
    Ensight output with specialized read() for strings, integers and floats.
    Correctly handles binary read as well.

\*---------------------------------------------------------------------------*/

#ifndef ensightReadFile_H
#define ensightReadFile_H

#include "IFstream.H"
#include "_IOstream.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                       Class ensightReadFile Declaration
\*---------------------------------------------------------------------------*/

class ensightReadFile
:
    public IFstream
{
    // Private Member Functions

        //- No copy construct
        ensightReadFile(const ensightReadFile&) = delete;

        //- No copy assignment
        void operator=(const ensightReadFile&) = delete;


public:

    // Constructors

        //- Construct from pathname. Default format is binary.
        explicit ensightReadFile
        (
            const fileName& pathname,
            IOstreamOption::streamFormat fmt = IOstreamOption::BINARY
        );


    //- Destructor
    ~ensightReadFile() = default;


    // Output

        //- Inherit read from Istream
        using Istream::read;

        //- Binary read
        virtual Istream& read(char* buf, std::streamsize count);

        //- Read string as "%80s" or as binary
        Istream& read(string& value);

        //- Read integer as "%10d" or as binary
        Istream& read(label& value);

        //- Read float as "%12.5e" or as binary
        Istream& read(scalar& value);

        //- Read element keyword
        virtual Istream& readKeyword(string& key);

        //- Read "C Binary" for binary files (eg, geometry/measured)
        Istream& readBinaryHeader();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
