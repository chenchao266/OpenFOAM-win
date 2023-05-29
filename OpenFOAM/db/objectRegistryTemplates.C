/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2015 OpenFOAM Foundation
    Copyright (C) 2016-2021 OpenCFD Ltd.
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

//#include "objectRegistry.H"
#include "predicates.H"
#include <type_traits>

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

// Templated implementation for classes()

 namespace Foam{
template<class MatchPredicate>
HashTable<wordHashSet> objectRegistry::classesImpl
(
    const objectRegistry& list,
    const MatchPredicate& matchName
)
{
    HashTable<wordHashSet> summary(2*list.size());

    // Summary (key,val) = (class-name, object-names)
    forAllConstIters(list, iter)
    {
        const regIOobject* obj = iter.val();

        if (matchName(obj->name()))
        {
            // Create entry (if needed) and insert
            summary(obj->type()).insert(obj->name());
        }
    }

    return summary;
}


// Templated implementation for count()
template<class MatchPredicate1, class MatchPredicate2>
label objectRegistry::countImpl
(
    const objectRegistry& list,
    const MatchPredicate1& matchClass,
    const MatchPredicate2& matchName
)
{
    label count = 0;

    forAllConstIters(list, iter)
    {
        const regIOobject* obj = iter.val();

        if (matchClass(obj->type()) && matchName(obj->name()))
        {
            ++count;
        }
    }

    return count;
}


// Templated implementation for count()
template<class Type, class MatchPredicate>
label objectRegistry::countTypeImpl
(
    const objectRegistry& list,
    const MatchPredicate& matchName
)
{
    label count = 0;

    forAllConstIters(list, iter)
    {
        const regIOobject* obj = iter.val();

        if
        (
            (std::is_void<Type>::value || isA<Type>(*obj))
         && matchName(obj->name())
        )
        {
            ++count;
        }
    }

    return count;
}


// Templated implementation for names(), sortedNames()
template<class MatchPredicate1, class MatchPredicate2>
wordList objectRegistry::namesImpl
(
    const objectRegistry& list,
    const MatchPredicate1& matchClass,
    const MatchPredicate2& matchName,
    const bool doSort
)
{
    wordList objNames(list.size());

    label count=0;
    forAllConstIters(list, iter)
    {
        const regIOobject* obj = iter.val();

        if (matchClass(obj->type()) && matchName(obj->name()))
        {
            objNames[count] = obj->name();
            ++count;
        }
    }

    objNames.resize(count);

    if (doSort)
    {
        sort(objNames);
    }

    return objNames;
}


// Templated implementation for names(), sortedNames()
template<class Type, class MatchPredicate>
wordList objectRegistry::namesTypeImpl
(
    const objectRegistry& list,
    const MatchPredicate& matchName,
    const bool doSort
)
{
    wordList objNames(list.size());

    label count = 0;
    forAllConstIters(list, iter)
    {
        const regIOobject* obj = iter.val();

        if
        (
            (std::is_void<Type>::value || isA<Type>(*obj))
         && matchName(obj->name())
        )
        {
            objNames[count] = obj->name();
            ++count;
        }
    }

    objNames.resize(count);

    if (doSort)
    {
        sort(objNames);
    }

    return objNames;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class MatchPredicate>
HashTable<wordHashSet>
objectRegistry::classes
(
    const MatchPredicate& matchName
) const
{
    return classesImpl(*this, matchName);
}


template<class MatchPredicate>
label objectRegistry::count
(
    const MatchPredicate& matchClass
) const
{
    return countImpl(*this, matchClass, predicates::always());
}


template<class MatchPredicate1, class MatchPredicate2>
label objectRegistry::count
(
    const MatchPredicate1& matchClass,
    const MatchPredicate2& matchName
) const
{
    return countImpl(*this, matchClass, matchName);
}


template<class Type, class MatchPredicate>
label objectRegistry::count
(
    const MatchPredicate& matchName
) const
{
    return countTypeImpl<Type>(*this, matchName);
}


template<class Type>
label objectRegistry::count
(
    const bool strict
) const
{
    label nObjects = 0;

    forAllConstIters(*this, iter)
    {
        const regIOobject* obj = iter.val();

        if
        (
            std::is_void<Type>::value
         || (strict ? isType<Type>(*obj) : bool(isA<Type>(*obj)))
        )
        {
            ++nObjects;
        }
    }

    return nObjects;
}


template<class MatchPredicate>
wordList objectRegistry::names
(
    const MatchPredicate& matchClass
) const
{
    return namesImpl(*this, matchClass, predicates::always(), false);
}


template<class MatchPredicate1, class MatchPredicate2>
wordList objectRegistry::names
(
    const MatchPredicate1& matchClass,
    const MatchPredicate2& matchName
) const
{
    return namesImpl(*this, matchClass, matchName, false);
}


template<class Type>
wordList objectRegistry::names() const
{
    return namesTypeImpl<Type>(*this, predicates::always(), false);
}


template<class Type, class MatchPredicate>
wordList objectRegistry::names
(
    const MatchPredicate& matchName
) const
{
    return namesTypeImpl<Type>(*this, matchName, false);
}


template<class MatchPredicate>
wordList objectRegistry::sortedNames
(
    const MatchPredicate& matchClass
) const
{
    return namesImpl(*this, matchClass, predicates::always(), true);
}


template<class MatchPredicate1, class MatchPredicate2>
wordList objectRegistry::sortedNames
(
    const MatchPredicate1& matchClass,
    const MatchPredicate2& matchName
) const
{
    return namesImpl(*this, matchClass, matchName, true);
}


template<class Type>
wordList objectRegistry::sortedNames() const
{
    return namesTypeImpl<Type>(*this, predicates::always(), true);
}


template<class Type, class MatchPredicate>
wordList objectRegistry::sortedNames
(
    const MatchPredicate& matchName
) const
{
    return namesTypeImpl<Type>(*this, matchName, true);
}


template<class Type>
HashTable<const Type*> objectRegistry::lookupClass
(
    const bool strict
) const
{
    HashTable<const Type*> objectsOfClass(size());

    forAllConstIters(*this, iter)
    {
        const regIOobject* obj = iter.val();

        if (strict ? isType<Type>(*obj) : bool(isA<Type>(*obj)))
        {
            objectsOfClass.insert(obj->name(), dynamic_cast<const Type*>(obj));
        }
    }

    return objectsOfClass;
}


template<class Type>
HashTable<Type*> objectRegistry::lookupClass
(
    const bool strict
)
{
    HashTable<Type*> objectsOfClass(size());

    forAllIters(*this, iter)
    {
        regIOobject* obj = iter.val();

        if (strict ? isType<Type>(*obj) : bool(isA<Type>(*obj)))
        {
            objectsOfClass.insert(obj->name(), dynamic_cast<Type*>(obj));
        }
    }

    return objectsOfClass;
}


template<class Type>
bool objectRegistry::foundObject
(
    const word& name,
    const bool recursive
) const
{
    return this->cfindObject<Type>(name, recursive);
}


template<class Type>
const Type* objectRegistry::cfindObject
(
    const word& name,
    const bool recursive
) const
{
    return dynamic_cast<const Type*>(this->cfindIOobject(name, recursive));
}


template<class Type>
const Type* objectRegistry::findObject
(
    const word& name,
    const bool recursive
) const
{
    return this->cfindObject<Type>(name, recursive);
}


template<class Type>
Type* objectRegistry::findObject
(
    const word& name,
    const bool recursive
)
{
    return const_cast<Type*>(this->cfindObject<Type>(name, recursive));
}


template<class Type>
Type* objectRegistry::getObjectPtr
(
    const word& name,
    const bool recursive
) const
{
    return const_cast<Type*>(this->cfindObject<Type>(name, recursive));
}


template<class Type>
const Type& objectRegistry::lookupObject
(
    const word& name,
    const bool recursive
) const
{
    const_iterator iter = cfind(name);

    if (iter.found())
    {
        const Type* ptr = dynamic_cast<const Type*>(iter());

        if (ptr)
        {
            return *ptr;
        }

        FatalErrorInFunction
            << nl
            << "    bad lookup of " << name << " (objectRegistry "
            << this->name()
            << ")\n    expected a " << Type::typeName
            << ", found a " << (*iter)->type() << nl
            << exit(FatalError);
    }
    else if (recursive && this->parentNotTime())
    {
        return parent_.lookupObject<Type>(name, recursive);
    }

    FatalErrorInFunction
        << nl
        << "    failed lookup of " << name << " (objectRegistry "
        << this->name()
        << ")\n    available objects of type " << Type::typeName
        << ':' << nl
        << names<Type>() << nl
        << exit(FatalError);

    return NullObjectRef<Type>();
}


template<class Type>
Type& objectRegistry::lookupObjectRef
(
    const word& name,
    const bool recursive
) const
{
    const Type& ref = this->lookupObject<Type>(name, recursive);
    // The above will already fail if things didn't work

    return const_cast<Type&>(ref);
}


// ************************************************************************* //

 } // End namespace Foam
