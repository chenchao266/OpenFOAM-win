﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2017 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
 2011 Symscape: Always access files in binary mode.
 2014-02-21 blueCAPE Lda: Modifications for blueCFD-Core 2.3
 2017-09-14 blueCAPE Lda: Modifications for blueCFD-Core 2017
------------------------------------------------------------------------------
License
    This file is a derivative work of OpenFOAM.

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

Modifications
    This file has been modified by blueCAPE's unofficial mingw patches for
    OpenFOAM.
    For more information about these patches, visit:
        http://bluecfd.com/Core

\*---------------------------------------------------------------------------*/

#include "OFstream.H"
#include "OSspecific.H"
#include "gzstream.h"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //
using namespace Foam;
namespace Foam
{
    defineTypeNameAndDebug(OFstream, 0);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

OFstreamAllocator::OFstreamAllocator
(
    const fileName& pathname,
    IOstream::compressionType compression,
    const bool append
) :    ofPtr_(nullptr)
{
    if (pathname.empty())
    {
        if (OFstream::debug)
        {
            InfoInFunction << "Cannot open null file " << endl;
        }
    }
    ofstream::openmode mode(ofstream::out);
    if (append)
    {
        mode |= ofstream::app;
    }

#if defined( WIN32 ) || defined( WIN64 )
    // Use binary mode in case we write binary.
    // Causes windows reading to fail if we don't
    mode |= ofstream::binary;
#endif

    if (compression == IOstream::COMPRESSED)
    {
        // Get identically named uncompressed version out of the way
        fileName::Type pathType = type(pathname, false);
        if (pathType == fileName::FILE || pathType == fileName::LINK)
        {
            rm(pathname);
        }
        fileName gzPathName(pathname + ".gz");

        if (!append && type(gzPathName) == fileName::LINK)
        {
            // Disallow writing into softlink to avoid any problems with
            // e.g. softlinked initial fields
            rm(gzPathName);
        }

        ofPtr_ = new ogzstream(gzPathName.c_str(), mode);
    }
    else
    {
        // get identically named compressed version out of the way
        fileName gzPathName(pathname + ".gz");
        fileName::Type gzType = type(gzPathName, false);
        if (gzType == fileName::FILE || gzType == fileName::LINK)
        {
            rm(gzPathName);
        }
        if (!append && type(pathname, false) == fileName::LINK)
        {
            // Disallow writing into softlink to avoid any problems with
            // e.g. softlinked initial fields
            rm(pathname);
        }

        ofPtr_ = new ofstream(pathname.c_str(), mode);
    }
}


OFstreamAllocator::~OFstreamAllocator()
{
    delete ofPtr_;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

OFstream::OFstream
(
    const fileName& pathname,
    streamFormat format,
    versionNumber version,
    compressionType compression,
    const bool append
) :    OFstreamAllocator(pathname, compression, append),
    OSstream(*ofPtr_, "OFstream.sinkFile_", format, version, compression),
    pathname_(pathname)
{
    setClosed();
    setState(ofPtr_->rdstate());

    if (!good())
    {
        if (debug)
        {
            InfoInFunction
                << "Could not open file " << pathname
                << "for input\n"
                   "in stream " << info() << ::Foam::endl;
        }

        setBad();
    }
    else
    {
        setOpened();
    }

    lineNumber_ = 1;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

OFstream::~OFstream()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

std::ostream& OFstream::stdStream()
{
    if (!ofPtr_)
    {
        FatalErrorInFunction
            << "No stream allocated." << abort(FatalError);
    }
    return *ofPtr_;
}


const std::ostream& OFstream::stdStream() const
{
    if (!ofPtr_)
    {
        FatalErrorInFunction
            << "No stream allocated." << abort(FatalError);
    }
    return *ofPtr_;
}


void OFstream::print(Ostream& os) const
{
    os  << "    OFstream: ";
    OSstream::print(os);
}


// ************************************************************************* //
