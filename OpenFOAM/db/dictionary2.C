﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
    Copyright (C) 2015-2021 OpenCFD Ltd.
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

#include "dictionary2.H"
#include "error.H"
#include "JobInfo.H"
#include "primitiveEntry.H"
#include "dictionaryEntry.H"
#include "regExp.H"
#include "OSHA1stream.H"
#include "OSstream.H"
#include "argList.H"
#include "registerSwitch.H"

/* * * * * * * * * * * * * * * Static Member Data  * * * * * * * * * * * * * */

namespace Foam
{
    defineTypeNameAndDebug(dictionary, 0);


    refPtr<OSstream> dictionary::reportingOutput(nullptr);

    const dictionary dictionary::null;

    int dictionary::writeOptionalEntries
    (
        debug::infoSwitch("writeOptionalEntries", 0)
    );


    registerInfoSwitch
    (
        "writeOptionalEntries",
        int,
        dictionary::writeOptionalEntries
    );


    // * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

    word dictionary::executableName()
    {
        return argList::envExecutable();
    }


    // * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

    dictionary::dictionary()
        :
        name_(),
        parent_(dictionary::null)
    {}


    dictionary::dictionary(const fileName& name)
        :
        name_(name),
        parent_(dictionary::null)
    {}


    dictionary::dictionary
    (
        const dictionary& parentDict,
        const dictionary& dict
    )
        :
        parent_type(dict, *this),
        name_(dict.name()),
        parent_(parentDict)
    {
        for (entry& e : *this)
        {
            hashedEntries_.insert(e.keyword(), &e);

            if (e.keyword().isPattern())
            {
                patterns_.insert(&e);
                regexps_.insert(autoPtr<regExp>::New(e.keyword()));
            }
        }
    }


    dictionary::dictionary
    (
        const dictionary& dict
    )
        :
        parent_type(dict, *this),
        name_(dict.name()),
        parent_(dictionary::null)
    {
        for (entry& e : *this)
        {
            hashedEntries_.insert(e.keyword(), &e);

            if (e.keyword().isPattern())
            {
                patterns_.insert(&e);
                regexps_.insert(autoPtr<regExp>::New(e.keyword()));
            }
        }
    }


    dictionary::dictionary(const dictionary* dict)
        :
        name_(),
        parent_(dictionary::null)
    {
        if (dict)
        {
            operator=(*dict);
        }
    }


    dictionary::dictionary
    (
        const dictionary& parentDict,
        dictionary&& dict
    )
        :
        name_(),
        parent_(parentDict)
    {
        transfer(dict);
        name() = fileName::concat(parentDict.name(), name(), '.');
    }


    dictionary::dictionary
    (
        dictionary&& dict
    )
        :
        name_(),
        parent_(dictionary::null)
    {
        transfer(dict);
    }


    autoPtr<dictionary> dictionary::clone() const
    {
        return autoPtr<dictionary>::New(*this);
    }


    // * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

    dictionary::~dictionary()
    {}


    // * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

    fileName dictionary::relativeName(const bool caseTag) const
    {
        return argList::envRelativePath(name(), caseTag);
    }


    const dictionary& dictionary::topDict() const
    {
        const dictionary& p = parent();

        if (&p != this && !p.name().empty())
        {
            return p.topDict();
        }

        return *this;
    }


    label dictionary::startLineNumber() const
    {
        if (size())
        {
            return first()->startLineNumber();
        }

        return -1;
    }


    label dictionary::endLineNumber() const
    {
        if (size())
        {
            return last()->endLineNumber();
        }

        return -1;
    }


    SHA1Digest dictionary::digest() const
    {
        OSHA1stream os;

        // Process entries
        for (const entry& e : *this)
        {
            os << e;
        }

        return os.digest();
    }


    tokenList dictionary::tokens() const
    {
        // Serialize dictionary entries into a string
        OStringStream os;

        // Process entries
        for (const entry& e : *this)
        {
            os << e;
        }

        // String re-parsed as a list of tokens
        return ITstream::parse(os.str());
    }


    void dictionary::checkITstream
    (
        const ITstream& is,
        const word& keyword
    ) const
    {
        if (is.nRemainingTokens())
        {
            const label remaining = is.nRemainingTokens();

            // Similar to SafeFatalIOError
            if (JobInfo::constructed)
            {
                OSstream& err =
                    FatalIOError
                    (
                        "",                 // functionName
                        "",                 // sourceFileName
                        0,                  // sourceFileLineNumber
                        relativeName(),     // ioFileName == dictionary name
                        is.lineNumber()     // ioStartLineNumber
                    );

                err << "Entry '" << keyword << "' has "
                    << remaining << " excess tokens in stream" << nl << nl
                    << "    ";
                is.writeList(err, 0);

                err << exit(FatalIOError);
            }
            else
            {
                std::cerr
                    << nl
                    << "--> FOAM FATAL IO ERROR:" << nl;

                std::cerr
                    << "Entry '" << keyword << "' has "
                    << remaining << " excess tokens in stream" << nl << nl;

                std::cerr
                    // ioFileName == dictionary name
                    << "file: " << relativeName()
                    << " at line " << is.lineNumber() << '.' << nl
                    << std::endl;

                std::exit(1);
            }
        }
        else if (!is.size())
        {
            // Similar to SafeFatalIOError
            if (JobInfo::constructed)
            {
                FatalIOError
                (
                    "",                 // functionName
                    "",                 // sourceFileName
                    0,                  // sourceFileLineNumber
                    relativeName(),     // ioFileName == dictionary name
                    is.lineNumber()     // ioStartLineNumber
                )
                    << "Entry '" << keyword
                    << "' had no tokens in stream" << nl << nl
                    << exit(FatalIOError);
            }
            else
            {
                std::cerr
                    << nl
                    << "--> FOAM FATAL IO ERROR:" << nl
                    << "Entry '" << keyword
                    << "' had no tokens in stream" << nl << nl;

                std::cerr
                    // ioFileName == dictionary name
                    << "file: " << relativeName()
                    << " at line " << is.lineNumber() << '.' << nl
                    << std::endl;

                std::exit(1);
            }
        }
    }


    void dictionary::raiseBadInput
    (
        const ITstream& is,
        const word& keyword
    ) const
    {
        // Can use FatalIOError instead of SafeFatalIOError
        // since predicate checks are not used at the earliest stages
        FatalIOError
        (
            "",                 // functionName
            "",                 // sourceFileName
            0,                  // sourceFileLineNumber
            relativeName(),     // ioFileName == dictionary name
            is.lineNumber(),    // ioStartLineNumber
            -1                  // ioEndLineNumber
        )
            << "Entry '" << keyword << "' with invalid input" << nl
            << exit(FatalIOError);
    }


    const entry& dictionary::lookupEntry
    (
        const word& keyword,
        enum keyType::option matchOpt
    ) const
    {
        const const_searcher finder(csearch(keyword, matchOpt));

        if (!finder.good())
        {
            FatalIOErrorInFunction(*this)
                << "Entry '" << keyword << "' not found in dictionary "
                << relativeName() << nl
                << exit(FatalIOError);
        }

        return finder.ref();
    }


    ITstream& dictionary::lookup
    (
        const word& keyword,
        enum keyType::option matchOpt
    ) const
    {
        return lookupEntry(keyword, matchOpt).stream();
    }


    bool dictionary::substituteKeyword
    (
        const word& keyword,
        bool mergeEntry
    )
    {
        if (keyword.size() < 2)
        {
            return false;
        }

        // Drop leading '$' to get the var-name, already validated as word.
        const word varName(keyword.substr(1), false);

        // Lookup the variable name in the given dictionary
        const const_searcher finder(csearch(varName, keyType::REGEX_RECURSIVE));

        // If defined insert its entries into this dictionary
        if (finder.good())
        {
            for (const entry& e : finder.dict())
            {
                add(e, mergeEntry);
            }

            return true;
        }

        return false;
    }


    bool dictionary::substituteScopedKeyword
    (
        const word& keyword,
        bool mergeEntry
    )
    {
        if (keyword.size() < 2)
        {
            return false;
        }

        // Drop leading '$' to get the var-name, already validated as word.
        const word varName(keyword.substr(1), false);

        // Lookup the variable name in the given dictionary
        const auto finder(csearchScoped(varName, keyType::REGEX_RECURSIVE));

        // If defined insert its entries into this dictionary
        if (finder.good())
        {
            for (const entry& e : finder.dict())
            {
                add(e, mergeEntry);
            }

            return true;
        }

        return false;
    }


    const dictionary& dictionary::subDict
    (
        const word& keyword,
        enum keyType::option matchOpt
    ) const
    {
        const const_searcher finder(csearch(keyword, matchOpt));

        if (!finder.good())
        {
            FatalIOErrorInFunction(*this)
                << "Entry '" << keyword << "' not found in dictionary "
                << relativeName() << nl
                << exit(FatalIOError);
        }

        return finder.dict();
    }


    dictionary& dictionary::subDict
    (
        const word& keyword,
        enum keyType::option matchOpt
    )
    {
        searcher finder(search(keyword, matchOpt));

        if (!finder.good())
        {
            FatalIOErrorInFunction(*this)
                << "Entry '" << keyword << "' not found in dictionary "
                << relativeName() << nl
                << exit(FatalIOError);
        }

        return finder.dict();
    }


    dictionary& dictionary::subDictOrAdd
    (
        const word& keyword,
        enum keyType::option matchOpt
    )
    {
        searcher finder(search(keyword, matchOpt));

        dictionary* ptr = finder.dictPtr();

        if (ptr)
        {
            // Found and a sub-dictionary
            return *ptr;
        }

        if (finder.good())
        {
            FatalIOErrorInFunction(*this)
                << "Entry '" << keyword
                << "' is not a sub-dictionary in dictionary "
                << relativeName() << nl
                << exit(FatalIOError);
        }

        ptr = this->set(keyword, dictionary())->dictPtr();

        if (!ptr)
        {
            FatalIOErrorInFunction(*this)
                << "Failed to insert sub-dictionary '" << keyword
                << "' in dictionary "
                << relativeName() << nl
                << exit(FatalIOError);
        }

        return *ptr;
    }


    dictionary dictionary::subOrEmptyDict
    (
        const word& keyword,
        enum keyType::option matchOpt,
        const bool mandatory
    ) const
    {
        const const_searcher finder(csearch(keyword, matchOpt));

        if (finder.isDict())
        {
            // Found and a sub-dictionary
            return finder.dict();
        }

        if (mandatory)
        {
            FatalIOErrorInFunction(*this)
                << "Entry '" << keyword
                << "' is not a sub-dictionary in dictionary "
                << relativeName() << nl
                << exit(FatalIOError);
        }

        if (finder.good())
        {
            IOWarningInFunction(*this)
                << "Entry '" << keyword
                << "' found but not a sub-dictionary in dictionary "
                << relativeName() << endl;
        }

        // The move constructor properly qualifies the dictionary name
        return dictionary(*this, dictionary(fileName(keyword)));
    }


    const dictionary& dictionary::optionalSubDict
    (
        const word& keyword,
        enum keyType::option matchOpt
    ) const
    {
        const const_searcher finder(csearch(keyword, matchOpt));

        if (finder.isDict())
        {
            // Found and a sub-dictionary
            return finder.dict();
        }

        if (finder.good())
        {
            IOWarningInFunction(*this)
                << "Entry '" << keyword
                << "' found but not a sub-dictionary in dictionary "
                << relativeName() << endl;
        }

        return *this;
    }


    wordList dictionary::toc() const
    {
        wordList list(size());

        label n = 0;
        for (const entry& e : *this)
        {
            list[n++] = e.keyword();
        }

        return list;
    }


    wordList dictionary::sortedToc() const
    {
        return hashedEntries_.sortedToc();
    }


    List<keyType> dictionary::keys(bool patterns) const
    {
        List<keyType> list(size());

        label n = 0;
        for (const entry& e : *this)
        {
            if (e.keyword().isPattern() ? patterns : !patterns)
            {
                list[n++] = e.keyword();
            }
        }
        list.resize(n);

        return list;
    }


    entry* dictionary::add(entry* entryPtr, bool mergeEntry)
    {
        if (!entryPtr)
        {
            return nullptr;
        }

        auto iter = hashedEntries_.find(entryPtr->keyword());

        if (mergeEntry && iter.good())
        {
            // Merge dictionary with dictionary
            if (iter()->isDict() && entryPtr->isDict())
            {
                iter()->dict().merge(entryPtr->dict());

                delete entryPtr;
                return iter();   // pointer to existing dictionary
            }


            // Replace existing dictionary with entry or vice versa
            parent_type::replace(iter(), entryPtr);
            delete iter();
            hashedEntries_.erase(iter);

            if (hashedEntries_.insert(entryPtr->keyword(), entryPtr))
            {
                entryPtr->name() =
                    fileName::concat(name(), entryPtr->keyword(), '.');

                if (entryPtr->keyword().isPattern())
                {
                    patterns_.insert(entryPtr);
                    regexps_.insert(autoPtr<regExp>::New(entryPtr->keyword()));
                }

                return entryPtr;  // now an entry in the dictionary
            }


            IOWarningInFunction(*this)
                << "Problem replacing entry " << entryPtr->keyword()
                << " in dictionary " << relativeName() << endl;

            parent_type::remove(entryPtr);

            delete entryPtr;
            return nullptr;
        }


        if (hashedEntries_.insert(entryPtr->keyword(), entryPtr))
        {
            entryPtr->name() =
                fileName::concat(name(), entryPtr->keyword(), '.');

            parent_type::append(entryPtr);

            if (entryPtr->keyword().isPattern())
            {
                patterns_.insert(entryPtr);
                regexps_.insert(autoPtr<regExp>::New(entryPtr->keyword()));
            }

            return entryPtr;  // now an entry in the dictionary
        }


        IOWarningInFunction(*this)
            << "Attempt to add entry " << entryPtr->keyword()
            << " which already exists in dictionary "
            << relativeName() << endl;

        delete entryPtr;
        return nullptr;
    }


    entry* dictionary::add(const entry& e, bool mergeEntry)
    {
        return add(e.clone(*this).ptr(), mergeEntry);
    }


    entry* dictionary::add
    (
        const keyType& k,
        const word& v,
        bool overwrite
    )
    {
        return add(new primitiveEntry(k, token(v)), overwrite);
    }


    entry* dictionary::add
    (
        const keyType& k,
        const string& v,
        bool overwrite
    )
    {
        return add(new primitiveEntry(k, token(v)), overwrite);
    }


    entry* dictionary::add
    (
        const keyType& k,
        const label v,
        bool overwrite
    )
    {
        return add(new primitiveEntry(k, token(v)), overwrite);
    }


    entry* dictionary::add
    (
        const keyType& k,
        const scalar v,
        bool overwrite
    )
    {
        return add(new primitiveEntry(k, token(v)), overwrite);
    }


    entry* dictionary::add
    (
        const keyType& k,
        const dictionary& v,
        bool mergeEntry
    )
    {
        return add(new dictionaryEntry(k, *this, v), mergeEntry);
    }


    entry* dictionary::set(entry* entryPtr)
    {
        if (!entryPtr)
        {
            return nullptr;
        }

        // Find non-recursive with patterns
        searcher finder(search(entryPtr->keyword(), keyType::REGEX));

        // Clear dictionary so merge acts like overwrite
        if (finder.isDict())
        {
            finder.dict().clear();
        }

        return add(entryPtr, true);
    }


    entry* dictionary::set(const entry& e)
    {
        return set(e.clone(*this).ptr());
    }


    entry* dictionary::set(const keyType& k, const dictionary& v)
    {
        return set(new dictionaryEntry(k, *this, v));
    }


    bool dictionary::merge(const dictionary& dict)
    {
        if (this == &dict)
        {
            FatalIOErrorInFunction(*this)
                << "Attempted merge to self, for dictionary "
                << relativeName() << nl
                << abort(FatalIOError);
        }

        bool changed = false;

        for (const entry& e : dict)
        {
            auto fnd = hashedEntries_.find(e.keyword());

            if (fnd.good())
            {
                // Recursively merge sub-dictionaries
                // TODO: merge without copying
                if (fnd()->isDict() && e.isDict())
                {
                    if (fnd()->dict().merge(e.dict()))
                    {
                        changed = true;
                    }
                }
                else
                {
                    add(e.clone(*this).ptr(), true);
                    changed = true;
                }
            }
            else
            {
                // Not found - just add
                add(e.clone(*this).ptr());
                changed = true;
            }
        }

        return changed;
    }


    void dictionary::clear()
    {
        parent_type::clear();
        hashedEntries_.clear();
        patterns_.clear();
        regexps_.clear();
    }


    void dictionary::transfer(dictionary& dict)
    {
        // Changing parents probably doesn't make much sense,
        // but what about the names?
        name() = dict.name();

        parent_type::transfer(dict);
        hashedEntries_.transfer(dict.hashedEntries_);
        patterns_.transfer(dict.patterns_);
        regexps_.transfer(dict.regexps_);
    }


    // * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

    void dictionary::operator=(const dictionary& rhs)
    {
        if (this == &rhs)
        {
            return;  // Self-assignment is a no-op
        }

        name() = rhs.name();
        clear();

        // Create clones of the entries in the given dictionary
        // resetting the parentDict to this dictionary

        for (const entry& e : rhs)
        {
            add(e.clone(*this).ptr());
        }
    }


    void dictionary::operator+=(const dictionary& rhs)
    {
        if (this == &rhs)
        {
            FatalIOErrorInFunction(*this)
                << "Attempted addition to self, for dictionary "
                << relativeName() << nl
                << abort(FatalIOError);
        }

        for (const entry& e : rhs)
        {
            add(e.clone(*this).ptr());
        }
    }


    void dictionary::operator|=(const dictionary& rhs)
    {
        if (this == &rhs)
        {
            FatalIOErrorInFunction(*this)
                << "Attempted |= merging to self, for dictionary "
                << relativeName() << nl
                << abort(FatalIOError);
        }

        for (const entry& e : rhs)
        {
            if (!found(e.keyword()))
            {
                add(e.clone(*this).ptr());
            }
        }
    }


    void dictionary::operator<<=(const dictionary& rhs)
    {
        if (this == &rhs)
        {
            FatalIOErrorInFunction(*this)
                << "Attempted addition to self, for dictionary "
                << relativeName() << nl
                << abort(FatalIOError);
        }

        for (const entry& e : rhs)
        {
            set(e.clone(*this).ptr());
        }
    }


    /* * * * * * * * * * * * * * * * Global operators  * * * * * * * * * * * * * */

    dictionary operator+
        (
            const dictionary& dict1,
            const dictionary& dict2
            )
    {
        dictionary result(dict1);
        result += dict2;
        return result;
    }


    dictionary operator|
        (
            const dictionary& dict1,
            const dictionary& dict2
            )
    {
        dictionary result(dict1);
        result |= dict2;
        return result;
    }

}
// ************************************************************************* //
