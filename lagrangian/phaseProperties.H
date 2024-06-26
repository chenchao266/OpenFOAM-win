/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2020 OpenCFD Ltd.
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
    Foam::phaseProperties

Description
    Helper class to manage multi-specie phase properties

SourceFiles
    phaseProperties.C
    phasePropertiesIO.C

\*---------------------------------------------------------------------------*/

#ifndef phaseProperties_H
#define phaseProperties_H

#include "Enum.H"
#include "Tuple2.H"
#include "PtrList.H"
#include "volFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward Declarations
class phaseProperties;
Istream& operator>>(Istream&, phaseProperties&);
Ostream& operator<<(Ostream&, const phaseProperties&);

/*---------------------------------------------------------------------------*\
                      Class phaseProperties Declaration
\*---------------------------------------------------------------------------*/

class phaseProperties
{
public:

    // Public Data

        //- Phase type enumeration
        enum phaseType
        {
            GAS,
            LIQUID,
            SOLID,
            UNKNOWN
        };

        //- Corresponding word representations for phase type enumerations
        static const Enum<phaseType> phaseTypeNames;


private:

   // Private Data

        //- Phase type
        phaseType phase_;

        //- State label (s), (l), (g) etc.
        word stateLabel_;

        //- List of specie names
        List<word> names_;

        //- List of specie mass fractions
        scalarField Y_;

        //- Map to carrier id
        labelList carrierIds_;


    // Private Member Functions

        //- Reorder species to be consistent with the given specie name list
        void reorder(const wordList& specieNames);

        //- Set carrier ids
        void setCarrierIds(const wordList& carrierNames);

        //- Check the total mass fraction
        void checkTotalMassFraction() const;

        //- Set the state label
        word phaseToStateLabel(const phaseType pt) const;


public:

    // Constructors

        //- Default construct, as 'UNKNOWN' state
        phaseProperties();

        //- Construct from Istream
        explicit phaseProperties(Istream& is);


    // Member Functions

        //- Reorder species to be consistent with the corresponding
        //  phase specie name list
        void reorder
        (
            const wordList& gasNames,
            const wordList& liquidNames,
            const wordList& solidNames
        );


        // Access

            //- Return const access to the phase type
            phaseType phase() const;

            //- Return const access to the phase state label
            const word& stateLabel() const;

            //- Return word representation of the phase type
            word phaseTypeName() const;

            //- Return the list of specie names
            const List<word>& names() const;

            //- Return const access to a specie name
            const word& name(const label speciei) const;

            //- Return const access to all specie mass fractions
            const scalarField& Y() const;

            //- Return non-const access to a specie mass fraction
            scalar& Y(const label speciei);

            //- Return const access to the map to the carrier ids
            const labelList& carrierIds() const;

            //- Return the id of a specie in the local list by name
            //  Returns -1 if not found
            label id(const word& specieName) const;


    // IOstream Operators

        friend Istream& operator>>(Istream&, phaseProperties&);
        friend Ostream& operator<<(Ostream&, const phaseProperties&);
        bool operator==(const phaseProperties& pp) const;
        bool operator!=(const phaseProperties& pp) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
