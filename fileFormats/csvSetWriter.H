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
    Foam::csvSetWriter

Description
    Write set in csv format

SourceFiles
    csvSetWriter.C

\*---------------------------------------------------------------------------*/

#ifndef csvSetWriter_H
#define csvSetWriter_H

#include "writer.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                       Class csvSetWriter Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class csvSetWriter
:
    public writer<Type>
{

    // Private Member Functions

        void writeCoordHeader(const coordSet&, Ostream&) const;

        void writeHeader(const coordSet&, const wordList&, Ostream&) const;


protected:

    virtual void writeSeparator(Ostream&) const;


public:

    //- Runtime type information
    TypeName("csv");


    // Constructors

        //- Default construct
        csvSetWriter();

        //- Construct with dictionary
        explicit csvSetWriter(const dictionary& dict);


    //- Destructor
    virtual ~csvSetWriter() = default;


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
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "csvSetWriter.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
