/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2012-2017 OpenFOAM Foundation
     \\/     M anipulation  |
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

#include "writeFile.H"
#include "Time.T.H"
#include "polyMesh.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //
using namespace Foam;
const word functionObjects::writeFile::outputPrefix
(
    "postProcessing"
);

label functionObjects::writeFile::addChars = 7;


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

void functionObjects::writeFile::initStream(Ostream& os) const
{
    os.setf(ios_base::scientific, ios_base::floatfield);
    os.width(charWidth());
}


fileName functionObjects::writeFile::baseFileDir() const
{
    fileName baseDir = fileObr_.time().path();

    if (Pstream::parRun())
    {
        // Put in undecomposed case (Note: gives problems for
        // distributed data running)
        baseDir = baseDir/".."/outputPrefix;
    }
    else
    {
        baseDir = baseDir/outputPrefix;
    }

    // Append mesh name if not default region
    if (isA<polyMesh>(fileObr_))
    {
        const polyMesh& mesh = refCast<const polyMesh>(fileObr_);
        if (mesh.name() != polyMesh::defaultRegion)
        {
            baseDir = baseDir/mesh.name();
        }
    }

    // Remove any ".."
    baseDir.clean();

    return baseDir;
}


fileName functionObjects::writeFile::baseTimeDir() const
{
    return baseFileDir()/prefix_/fileObr_.time().timeName();
}


Omanip<int> functionObjects::writeFile::valueWidth
(
    const label offset
) const
{
    return setw(IOstream::defaultPrecision() + addChars + offset);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

functionObjects::writeFile::writeFile
(
    const objectRegistry& obr,
    const word& prefix
) :    fileObr_(obr),
    prefix_(prefix)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

functionObjects::writeFile::~writeFile()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

label functionObjects::writeFile::charWidth() const
{
    return IOstream::defaultPrecision() + addChars;
}


void functionObjects::writeFile::writeCommented
(
    Ostream& os,
    const string& str
) const
{
    os  << setw(1) << "#" << setw(1) << ' '
        << setf(ios_base::left) << setw(charWidth() - 2) << str.c_str();
}


void functionObjects::writeFile::writeTabbed
(
    Ostream& os,
    const string& str
) const
{
    os  << tab << setw(charWidth()) << str.c_str();
}


void functionObjects::writeFile::writeHeader
(
    Ostream& os,
    const string& str
) const
{
    os  << setw(1) << "#" << setw(1) << ' '
        << setf(ios_base::left) << setw(charWidth() - 2) << str.c_str() << nl;
}


void functionObjects::writeFile::writeTime(Ostream& os) const
{
    os  << setw(charWidth()) << fileObr_.time().timeName();
}


// ************************************************************************* //
