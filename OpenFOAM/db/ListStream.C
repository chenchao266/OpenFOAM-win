﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2017 OpenCFD Ltd.
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

#include "UListStream.H"
#include "ListStream.H"
#include "OCountStream.H"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


 namespace Foam{
void IListStream::print(Ostream& os) const
{
    os  << "IListStream: ";
    printBufInfo(os);
    os  << ::Foam::endl;
}


void UIListStream::print(Ostream& os) const
{
    os  << "UIListStream: ";
    printBufInfo(os);
    os  << ::Foam::endl;
}


void OListStream::print(Ostream& os) const
{
    os  << "OListStream: ";
    printBufInfo(os);
    os  << ::Foam::endl;
}


void UOListStream::print(Ostream& os) const
{
    os  << "UOListStream: ";
    printBufInfo(os);
    os  << ::Foam::endl;
}


void OCountStream::print(Ostream& os) const
{
    os  << "OCountStream: ";
    printBufInfo(os);
    os  << ::Foam::endl;
}


// ************************************************************************* //

 } // End namespace Foam
