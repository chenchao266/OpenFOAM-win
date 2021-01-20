/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2017 OpenFOAM Foundation
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

//#include "dictionary.H"
#include "primitiveEntry.H"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //
namespace Foam {
    template<class T>
    T dictionary::lookupType
    (
        const word& keyword,
        bool recursive,
        bool patternMatch
    ) const
    {
        const entry* entryPtr = lookupEntryPtr(keyword, recursive, patternMatch);

        if (entryPtr == nullptr)
        {
            FatalIOErrorInFunction
            (
                *this
            ) << "keyword " << keyword << " is undefined in dictionary "
                << name()
                << exit(FatalIOError);
        }

        return pTraits<T>(entryPtr->stream());
    }


    template<class T>
    T dictionary::lookupOrDefault
    (
        const word& keyword,
        const T& deflt,
        bool recursive,
        bool patternMatch
    ) const
    {
        const entry* entryPtr = lookupEntryPtr(keyword, recursive, patternMatch);

        if (entryPtr)
        {
            return pTraits<T>(entryPtr->stream());
        }
        else
        {
            if (writeOptionalEntries)
            {
                IOInfoInFunction(*this)
                    << "Optional entry '" << keyword << "' is not present,"
                    << " returning the default value '" << deflt << "'"
                    << endl;
            }

            return deflt;
        }
    }


    template<class T>
    T dictionary::lookupOrAddDefault
    (
        const word& keyword,
        const T& deflt,
        bool recursive,
        bool patternMatch
    )
    {
        const entry* entryPtr = lookupEntryPtr(keyword, recursive, patternMatch);

        if (entryPtr)
        {
            return pTraits<T>(entryPtr->stream());
        }
        else
        {
            if (writeOptionalEntries)
            {
                IOInfoInFunction(*this)
                    << "Optional entry '" << keyword << "' is not present,"
                    << " adding and returning the default value '" << deflt << "'"
                    << endl;
            }

            add(new primitiveEntry(keyword, deflt));
            return deflt;
        }
    }


    template<class T>
    bool dictionary::readIfPresent
    (
        const word& keyword,
        T& val,
        bool recursive,
        bool patternMatch
    ) const
    {
        const entry* entryPtr = lookupEntryPtr(keyword, recursive, patternMatch);

        if (entryPtr)
        {
            entryPtr->stream() >> val;
            return true;
        }
        else
        {
            if (writeOptionalEntries)
            {
                IOInfoInFunction(*this)
                    << "Optional entry '" << keyword << "' is not present,"
                    << " the default value '" << val << "' will be used."
                    << endl;
            }

            return false;
        }
    }


    template<class T>
    void dictionary::add(const keyType& k, const T& t, bool overwrite)
    {
        add(new primitiveEntry(k, t), overwrite);
    }


    template<class T>
    void dictionary::set(const keyType& k, const T& t)
    {
        set(new primitiveEntry(k, t));
    }

}
// ************************************************************************* //
