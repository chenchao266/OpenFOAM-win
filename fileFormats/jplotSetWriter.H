/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
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

Class
    Foam::jplotSetWriter

SourceFiles
    jplotSetWriter.C

\*---------------------------------------------------------------------------*/

#ifndef jplotSetWriter_H
#define jplotSetWriter_H

#include "writer.H"
#include "vector2.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                      Class jplotSetWriter Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class jplotSetWriter
:
    public writer<Type>
{

    // Private Member Functions

        //- Write header
        Ostream& writeHeader(Ostream&) const;

public:

    //- Runtime type information
    TypeName("jplot");


    // Constructors

        //- Default construct
        jplotSetWriter();

        //- Construct with dictionary
        explicit jplotSetWriter(const dictionary& dict);


    //- Destructor
    virtual ~jplotSetWriter() = default;


    // Member Functions

        virtual fileName getFileName
        (
            const coordSet&,
            const wordList&
        ) const;

        virtual void write
        (
            const coordSet&,
            const wordList&,
            const List<const Field<Type>*>&,
            Ostream&
        ) const;

        virtual void write
        (
            const bool writeTracks,
            const List<scalarField>& times,
            const PtrList<coordSet>& tracks,
            const wordList& valueSetNames,
            const List<List<Field<Type>>>& valueSets,
            Ostream&
        ) const
        {
            NotImplemented;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "jplotSetWriter.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
