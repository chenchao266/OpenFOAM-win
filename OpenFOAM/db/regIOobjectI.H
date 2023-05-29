/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2015 OpenFOAM Foundation
    Copyright (C) 2018-2021 OpenCFD Ltd.
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

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


 namespace Foam{
inline bool regIOobject::ownedByRegistry() const
{
    return ownedByRegistry_;
}


inline bool regIOobject::store()
{
    if (checkIn())
    {
        ownedByRegistry_ = true;
    }
    else
    {
        WarningInFunction
            << "Refuse to store unregistered object: " << this->name() << nl;
    }

    return ownedByRegistry_;
}


template<class Type>
inline Type& regIOobject::store(Type* p)
{
    if (!p)
    {
        FatalErrorInFunction
            << "Object deallocated\n"
            << abort(FatalError);
    }

    const bool ok = p->regIOobject::store();

    if (!ok)
    {
        FatalErrorInFunction
            << "Failed to store pointer: " << p->regIOobject::name()
            << ". Risk of memory leakage\n"
            << abort(FatalError);
    }

    return *p;
}


template<class Type>
inline Type& regIOobject::store(autoPtr<Type>& ptr)
{
    // Pass management to objectRegistry
    return store(ptr.release());
}


template<class Type>
inline Type& regIOobject::store(autoPtr<Type>&& ptr)
{
    // Pass management to objectRegistry
    return store(ptr.release());
}


template<class Type>
inline Type& regIOobject::store(refPtr<Type>& ptr)
{
    Type* p = nullptr;

    if (ptr.is_pointer())
    {
        // Acquire ownership, pass management to objectRegistry
        p = ptr.release();

        store(p);

        // Change parameter to access the stored reference
        ptr.cref(p);
    }
    else
    {
        // Taking ownership of reference does not make much sense.
        // - Storing the object won't actually do so, it will be removed
        //   when the original object goes out of scope.
        // - Storing a clone may not be what we want.

        p = ptr.get();

        WarningInFunction
            << "Refuse to store reference: " << p->name()
            << ". Likely indicates a coding error\n";
    }

    return *p;
}


template<class Type>
inline Type& regIOobject::store(refPtr<Type>&& ptr)
{
    // Forward as named reference, which also does a move
    return store(ptr);
}


template<class Type>
inline Type& regIOobject::store(tmp<Type>& ptr)
{
    Type* p = nullptr;

    if (ptr.is_pointer())
    {
        // Acquire ownership, pass management to objectRegistry
        p = ptr.ptr();

        store(p);

        // Change parameter to access the stored reference
        ptr.cref(p);
    }
    else
    {
        // Taking ownership of reference does not make much sense.
        // - Storing the object won't actually do so, it will be removed
        //   when the original object goes out of scope.
        // - Storing a clone may not be what we want.

        p = ptr.get();

        WarningInFunction
            << "Refuse to store reference: " << p->name()
            << ". Likely indicates a coding error\n";
    }

    return *p;
}


template<class Type>
inline Type& regIOobject::store(tmp<Type>&& ptr)
{
    // Forward as named reference, which also does a move
    return store(ptr);
}


inline void regIOobject::release(const bool unregister)
{
    ownedByRegistry_ = false;
    if (unregister)
    {
        registered_ = false;
    }
}


inline label regIOobject::eventNo() const
{
    return eventNo_;
}

inline label& regIOobject::eventNo()
{
    return eventNo_;
}


inline const labelList& regIOobject::watchIndices() const
{
    return watchIndices_;
}


inline labelList& regIOobject::watchIndices()
{
    return watchIndices_;
}


// ************************************************************************* //

 } // End namespace Foam
