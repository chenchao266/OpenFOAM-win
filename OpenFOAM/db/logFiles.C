/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2012-2016 OpenFOAM Foundation
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

#include "logFiles.H"
#include "Time.T.H"
#include "IFstream.H"

// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //
using namespace Foam;
void functionObjects::logFiles::createFiles()
{
    if (Pstream::master())
    {
        const word startTimeName =
            fileObr_.time().timeName(fileObr_.time().startTime().value());

        forAll(names_, i)
        {
            if (!filePtrs_.set(i))
            {
                fileName outputDir(baseFileDir()/prefix_/startTimeName);
                mkDir(outputDir);

                word fName(names_[i]);

                // Check if file already exists
                IFstream is(outputDir/(fName + ".dat"));
                if (is.good())
                {
                    fName = fName + "_" + fileObr_.time().timeName();
                }

                filePtrs_.set(i, new OFstream(outputDir/(fName + ".dat")));

                initStream(filePtrs_[i]);

                writeFileHeader(i);
            }
        }
    }
}


void functionObjects::logFiles::resetNames(const wordList& names)
{
    names_.clear();
    names_.append(names);

    if (Pstream::master())
    {
        filePtrs_.clear();
        filePtrs_.setSize(names_.size());
    }
}


void functionObjects::logFiles::resetName(const word& name)
{
    names_.clear();
    names_.append(name);

    if (Pstream::master())
    {
        filePtrs_.clear();
        filePtrs_.setSize(1);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

functionObjects::logFiles::logFiles
(
    const objectRegistry& obr,
    const word& prefix
) :    writeFile(obr, prefix),
    names_(),
    filePtrs_()
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

functionObjects::logFiles::~logFiles()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const wordList& functionObjects::logFiles::names() const
{
    return names_;
}


OFstream& functionObjects::logFiles::file()
{
    if (!Pstream::master())
    {
        FatalErrorInFunction
            << "Request for file() can only be done by the master process"
            << abort(FatalError);
    }

    if (filePtrs_.size() != 1)
    {
        WarningInFunction
            << "Requested single file, but multiple files are present"
            << endl;
    }

    if (!filePtrs_.set(0))
    {
        FatalErrorInFunction
            << "File pointer at index " << 0 << " not allocated"
            << abort(FatalError);
    }

    return filePtrs_[0];
}


PtrList<OFstream>& functionObjects::logFiles::files()
{
    if (!Pstream::master())
    {
        FatalErrorInFunction
            << "Request for files() can only be done by the master process"
            << abort(FatalError);
    }

    return filePtrs_;
}


OFstream& functionObjects::logFiles::file(const label i)
{
    if (!Pstream::master())
    {
        FatalErrorInFunction
            << "Request for file(i) can only be done by the master process"
            << abort(FatalError);
    }

    if (!filePtrs_.set(i))
    {
        FatalErrorInFunction
            << "File pointer at index " << i << " not allocated"
            << abort(FatalError);
    }

    return filePtrs_[i];
}


bool functionObjects::logFiles::write()
{
    createFiles();

    return true;
}


// ************************************************************************* //
