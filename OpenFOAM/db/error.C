/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2014 OpenFOAM Foundation
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

#include "error.H"
#include "OStringStream.H"
#include "fileName.H"
#include "dictionary.H"
#include "JobInfo.H"
#include "Pstream.T.H"
#include "OSspecific.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
using namespace Foam;
error::error(const string& title) :    std::exception(),
    messageStream(title, messageStream::FATAL),
    functionName_("unknown"),
    sourceFileName_("unknown"),
    sourceFileLineNumber_(0),
    abort_(env("FOAM_ABORT")),
    throwExceptions_(false),
    messageStreamPtr_(new OStringStream())
{
    if (!messageStreamPtr_->good())
    {
        Perr<< endl
            << "error::error(const string& title) : cannot open error stream"
            << endl;
        exit(1);
    }
}


error::error(const dictionary& errDict) :    std::exception(),
    messageStream(errDict),
    functionName_(errDict.lookup("functionName")),
    sourceFileName_(errDict.lookup("sourceFileName")),
    sourceFileLineNumber_(readLabel(errDict.lookup("sourceFileLineNumber"))),
    abort_(env("FOAM_ABORT")),
    throwExceptions_(false),
    messageStreamPtr_(new OStringStream())
{
    if (!messageStreamPtr_->good())
    {
        Perr<< endl
            << "error::error(const dictionary& errDict) : "
               "cannot open error stream"
            << endl;
        exit(1);
    }
}


error::error(const error& err) :    std::exception(),
    messageStream(err),
    functionName_(err.functionName_),
    sourceFileName_(err.sourceFileName_),
    sourceFileLineNumber_(err.sourceFileLineNumber_),
    abort_(err.abort_),
    throwExceptions_(err.throwExceptions_),
    messageStreamPtr_(new OStringStream(*err.messageStreamPtr_))
{
    //*messageStreamPtr_ << err.message();
}


error::~error() throw()
{
    delete messageStreamPtr_;
}


OSstream& error::operator()
(
    const char* functionName,
    const char* sourceFileName,
    const int sourceFileLineNumber
)
{
    functionName_ = functionName;
    sourceFileName_ = sourceFileName;
    sourceFileLineNumber_ = sourceFileLineNumber;

    return operator OSstream&();
}


OSstream& error::operator()
(
    const string& functionName,
    const char* sourceFileName,
    const int sourceFileLineNumber
)
{
    return operator()
    (
        functionName.c_str(),
        sourceFileName,
        sourceFileLineNumber
    );
}


error::operator OSstream&()
{
    if (!messageStreamPtr_->good())
    {
        Perr<< endl
            << "error::operator OSstream&() : error stream has failed"
            << endl;
        abort();
    }

    return *messageStreamPtr_;
}


error::operator dictionary() const
{
    dictionary errDict;

    string oneLineMessage(message());
    oneLineMessage.replaceAll('\n', ' ');

    errDict.add("type", word("error"));
    errDict.add("message", oneLineMessage);
    errDict.add("function", functionName());
    errDict.add("sourceFile", sourceFileName());
    errDict.add("sourceFileLineNumber", sourceFileLineNumber());

    return errDict;
}


string error::message() const
{
    return messageStreamPtr_->str();
}


void error::exit(const int errNo)
{
    if (!throwExceptions_ && JobInfo::constructed)
    {
        jobInfo.add("FatalError", operator dictionary());
        jobInfo.exit();
    }

    if (abort_)
    {
        abort();
    }

    if (Pstream::parRun())
    {
        Perr<< endl << *this << endl
            << "\nFOAM parallel run exiting\n" << endl;
        Pstream::exit(errNo);
    }
    else
    {
        if (throwExceptions_)
        {
            // Make a copy of the error to throw
            error errorException(*this);

            // Rewind the message buffer for the next error message
            messageStreamPtr_->rewind();

            throw errorException;
        }
        else
        {
            Perr<< endl << *this << endl
                << "\nFOAM exiting\n" << endl;
            ::exit(1);
        }
    }
}


void error::abort()
{
    if (!throwExceptions_ && JobInfo::constructed)
    {
        jobInfo.add("FatalError", operator dictionary());
        jobInfo.abort();
    }

    if (abort_)
    {
        Perr<< endl << *this << endl
            << "\nFOAM aborting (FOAM_ABORT set)\n" << endl;
        printStack(Perr);
        ::abort();
    }

    if (Pstream::parRun())
    {
        Perr<< endl << *this << endl
            << "\nFOAM parallel run aborting\n" << endl;
        printStack(Perr);
        Pstream::abort();
    }
    else
    {
        if (throwExceptions_)
        {
            // Make a copy of the error to throw
            error errorException(*this);

            // Rewind the message buffer for the next error message
            messageStreamPtr_->rewind();

            throw errorException;
        }
        else
        {
            Perr<< endl << *this << endl
                << "\nFOAM aborting\n" << endl;
            printStack(Perr);
            ::abort();
        }
    }
}


Ostream& operator<<(Ostream& os, const error& fErr)
{
    os  << endl
        << fErr.title().c_str() << endl
        << fErr.message().c_str();

    if (error::level >= 2 && fErr.sourceFileLineNumber())
    {
        os  << endl << endl
            << "    From function " << fErr.functionName().c_str() << endl
            << "    in file " << fErr.sourceFileName().c_str()
            << " at line " << fErr.sourceFileLineNumber() << '.';
    }

    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Global error definitions

error FatalError("--> FOAM FATAL ERROR: ");

// ************************************************************************* //
