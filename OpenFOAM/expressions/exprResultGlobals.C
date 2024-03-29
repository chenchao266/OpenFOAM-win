﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2012-2018 Bernhard Gschaider <bgschaid@hfd-research.com>
    Copyright (C) 2019-2020 OpenCFD Ltd.
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

#include "exprResultGlobals.H"
#include "Time1.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace expressions
{

    defineTypeName(exprResultGlobals);

} // End namespace expressions
} // End namespace Foam


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


 namespace Foam{
expressions::exprResultGlobals::exprResultGlobals
(
    const objectRegistry& obr
)
:
    regIOobject
    (
        IOobject
        (
            exprResultGlobals::typeName,
            obr.time().timeName(),  // instance
            "expressions",          // local
            obr.time(),
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE,
            true  // register
        )
    ),
    variables_(),
    timeIndex_(obr.time().timeIndex())
{
    if (headerOk())
    {
        readData
        (
            readStream(exprResultGlobals::typeName, true)
        );
    }
}


expressions::exprResultGlobals::Table::Table(const Table& tbl)
:
    HashPtrTable<exprResult>(tbl.capacity())
{
    for (auto iter = tbl.cbegin(); iter != tbl.cend(); ++iter)
    {
        this->set(iter.key(), (*iter)->clone());
    }
}


expressions::exprResultGlobals::Table::Table(Table&& tbl)
:
    HashPtrTable<exprResult>(std::move(tbl))
{}


expressions::exprResultGlobals::Table::Table(Istream& is)
:
    HashPtrTable<exprResult>(is)
{}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void expressions::exprResultGlobals::reset()
{
    forAllIters(variables_, tablesIter)
    {
        forAllIters((*tablesIter), iter)
        {
            (*iter)->reset();
        }
    }
}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

expressions::exprResultGlobals&
expressions::exprResultGlobals::New
(
    const objectRegistry& obr
)
{
    typedef expressions::exprResultGlobals Type;

    auto* ptr = obr.time().getObjectPtr<Type>(Type::typeName);

    if (!ptr)
    {
        ptr = new Type(obr);
        ptr->store();
    }
    else if (ptr->timeIndex_ != obr.time().timeIndex())
    {
        // If time changes, reset variables

        ptr->timeIndex_ = obr.time().timeIndex();
        ptr->reset();
    }

    return *ptr;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

bool expressions::exprResultGlobals::Delete(const objectRegistry& obr)
{
    typedef expressions::exprResultGlobals Type;

    auto* ptr = obr.time().getObjectPtr<Type>(Type::typeName);

    if (ptr)
    {
        return obr.time().checkOut(ptr);
    }

    return false;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool expressions::exprResultGlobals::writeData(Ostream& os) const
{
    // Enforce ASCII to avoid any potential binary issues
    const auto oldFmt = os.format(IOstream::ASCII);

    os << variables_;

    os.format(oldFmt);

    return os.good();
}


bool expressions::exprResultGlobals::readData(Istream& is)
{
    // Enforce ASCII to avoid any potential binary issues
    const auto oldFmt = is.format(IOstream::ASCII);

    is >> variables_;

    is.format(oldFmt);

    return !is.bad();
}


expressions::exprResultGlobals::Table&
expressions::exprResultGlobals::getNamespace(const word& name)
{
    return variables_[name];
}


const expressions::exprResult&
expressions::exprResultGlobals::get
(
    const word& name,
    const wordUList& scopes
) const
{
    for (const word& scopeName : scopes)
    {
        const auto tableIter = variables_.cfind(scopeName);

        if (tableIter.found())
        {
            const auto resultIter = (*tableIter).cfind(name);

            if (resultIter.found())
            {
                return *(*resultIter);
            }
        }
        #ifdef FULLDEBUG
        else
        {
            WarningInFunction
                << "No scope " << scopeName << " for " << name << nl
                << "Known global scopes: " << variables_.sortedToc() << nl;
        }
        #endif
    }

    return exprResult::null;
}


expressions::exprResult&
expressions::exprResultGlobals::addValue
(
    const word& name,
    const word& scope,
    const exprResult& value,
    const bool overwrite
)
{
    Table& tbl = getOrCreateScope(scope);

    auto iter = tbl.find(name);

    if (!iter.found())
    {
        tbl.set(name, new exprResult(value));
        iter = tbl.find(name);
    }
    else if (overwrite)
    {
        *(*iter) = value;
    }

    return *(*iter);
}


expressions::exprResult&
expressions::exprResultGlobals::addValue
(
    const word& name,
    const word& scope,
    autoPtr<exprResult>&& value,
    const bool overwrite
)
{
    Table& tbl = getOrCreateScope(scope);

    if (overwrite || !tbl.found(name))
    {
        tbl.set(name, std::move(value));
    }

    return *tbl[name];
}


expressions::exprResult&
expressions::exprResultGlobals::addValue
(
    const dictionary& dict,
    const word& scope,
    const bool overwrite
)
{
    word scopeName(scope);

    const word name(dict.get<word>("globalName"));

    if (scopeName.empty())
    {
        scopeName = dict.get<word>("globalScope");
    }

    if (dict.found("resultType"))
    {
        return addValue
        (
            name,
            scopeName,
            exprResult::New(dict),
            overwrite
        );
    }
    else
    {
        return addValue
        (
            name,
            scopeName,
            exprResult(dict, true),
            overwrite
        );
    }
}


bool expressions::exprResultGlobals::removeValue
(
    const word& name,
    const word& scope
)
{
    auto iter = variables_.find(scope);

    return (iter.found() && (*iter).erase(name));
}


// ************************************************************************* //

 } // End namespace Foam
