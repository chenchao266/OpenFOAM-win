/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016-2019 OpenCFD Ltd.
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

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


 namespace Foam{
inline hashedWordList::hashedWordList(const hashedWordList& list)
:
    hashedWordList(static_cast<const wordUList&>(list), false)
{}


inline hashedWordList::hashedWordList(hashedWordList&& list)
:
    wordList(std::move(static_cast<wordList&>(list))),
    lookup_(std::move(list.lookup_))
{}


inline hashedWordList::hashedWordList(const wordUList& list)
:
    hashedWordList(list, false)
{}


inline hashedWordList::hashedWordList(const wordUList& list, bool unique)
:
    wordList(list)
{
    rehash(unique);
}


inline hashedWordList::hashedWordList(wordList&& list, bool unique)
:
    wordList(std::move(list))
{
    rehash(unique);
}


inline hashedWordList::hashedWordList(std::initializer_list<word> list)
:
    wordList(list)
{
    rehash();
}


template<class AnyType, class AnyHash>
inline hashedWordList::hashedWordList
(
    const HashTable<AnyType, word, AnyHash>& tbl
)
:
    wordList(tbl.size())
{
    wordList& list = *this;

    label count = 0;
    for (auto iter = tbl.cbegin(); iter != tbl.cend(); ++iter)
    {
        list[count++] = iter.key();
    }

    this->sort();
}


inline hashedWordList::hashedWordList(Istream& is)
{
    is  >> *this;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline void hashedWordList::clear()
{
    wordList::clear();
    lookup_.clear();
}


inline void hashedWordList::append(const word& name, bool unique)
{
    // name is either unique or we don't care about duplicates
    if (lookup_.insert(name, size()) || !unique)
    {
        wordList::append(name);
    }
}


inline const HashTable<label>&
hashedWordList::lookup() const
{
    const label lenList = wordList::size();
    const label lenHash = lookup_.size();

    if ((lenList < lenHash) || (lenList && !lenHash))
    {
        rehash();   // Was somehow out of sync
    }

    return lookup_;
}


// TBD (2019-01-07) - overload find() for consistency?
//
// inline Foam::label Foam::hashedWordList::find(const word& name) const
// {
//     return lookup().lookup(name, -1);  // -1 = not found or not hashed
// }


inline bool hashedWordList::found(const word& name) const
{
    return lookup().found(name);
}


inline void hashedWordList::swap(hashedWordList& list)
{
    if (this == &list)
    {
        return;  // Self-swap is a no-op
    }

    wordList::swap(static_cast<wordList&>(list));
    lookup_.swap(list.lookup_);
}


inline void hashedWordList::transfer(hashedWordList& list)
{
    wordList::transfer(static_cast<wordList&>(list));
    lookup_.transfer(list.lookup_);
}


inline void hashedWordList::transfer(wordList& list, bool unique)
{
    wordList::transfer(list);
    rehash(unique);
}


inline void hashedWordList::rehash(bool unique)
{
    if (unique)
    {
        uniq();
    }
    else
    {
        rehash();
    }
}


inline void hashedWordList::sort()
{
    ::Foam::sort(*this);
    rehash();
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline const word& hashedWordList::operator[]
(
    const label index
) const
{
    return wordList::operator[](index);
}


inline label hashedWordList::operator[](const word& name) const
{
    return lookup_.lookup(name, -1);  // -1 = not found or not hashed
}


inline bool hashedWordList::operator()(const word& name) const
{
    return lookup_.found(name);
}


inline void hashedWordList::operator=(const hashedWordList& list)
{
    wordList::operator=(list);
    rehash();
}


inline void hashedWordList::operator=(const wordUList& list)
{
    wordList::operator=(list);
    rehash();
}


inline void hashedWordList::operator=(std::initializer_list<word> list)
{
    wordList::operator=(list);
    rehash();
}


inline void hashedWordList::operator=(hashedWordList&& list)
{
    wordList::transfer(static_cast<wordList&>(list));
    lookup_ = std::move(list.lookup_);
}


inline void hashedWordList::operator=(wordList&& list)
{
    wordList::transfer(list);
    rehash();
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

inline Istream& operator>>(Istream& is, hashedWordList& list)
{
    is  >> static_cast<wordList&>(list);
    list.rehash();

    return is;
}


// ************************************************************************* //

 } // End namespace Foam
