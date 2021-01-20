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

//#include "CompactIOField.T.H"
#include "labelList.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //
namespace Foam {
    template<class Ty, class BaseType>
    void CompactIOField<Ty, BaseType>::readFromStream(const bool valid)
    {
        Istream& is = readStream(word::null, valid);

        if (valid)
        {
            if (headerClassName() == IOField<Ty>::typeName)
            {
                is >> static_cast<Field<Ty>&>(*this);
                close();
            }
            else if (headerClassName() == typeName)
            {
                is >> *this;
                close();
            }
            else
            {
                FatalIOErrorInFunction
                (
                    is
                ) << "unexpected class name " << headerClassName()
                    << " expected " << typeName << " or " << IOField<Ty>::typeName
                    << endl
                    << "    while reading object " << name()
                    << exit(FatalIOError);
            }
        }
    }


    // * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * //

    template<class Ty, class BaseType>
    CompactIOField<Ty, BaseType>::CompactIOField(const IOobject& io)
        :
        regIOobject(io)
    {
        if
            (
                io.readOpt() == IOobject::MUST_READ
                || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())
                )
        {
            readFromStream();
        }
    }


    template<class Ty, class BaseType>
    CompactIOField<Ty, BaseType>::CompactIOField
    (
        const IOobject& io,
        const bool valid
    )
        :
        regIOobject(io)
    {
        if (io.readOpt() == IOobject::MUST_READ)
        {
            readFromStream(valid);
        }
        else if (io.readOpt() == IOobject::READ_IF_PRESENT)
        {
            bool haveFile = headerOk();
            readFromStream(valid && haveFile);
        }
    }


    template<class Ty, class BaseType>
    CompactIOField<Ty, BaseType>::CompactIOField
    (
        const IOobject& io,
        const label size
    )
        :
        regIOobject(io)
    {
        if
            (
                io.readOpt() == IOobject::MUST_READ
                || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())
                )
        {
            readFromStream();
        }
        else
        {
            Field<Ty>::setSize(size);
        }
    }


    template<class Ty, class BaseType>
    CompactIOField<Ty, BaseType>::CompactIOField
    (
        const IOobject& io,
        const Field<Ty>& list
    )
        :
        regIOobject(io)
    {
        if
            (
                io.readOpt() == IOobject::MUST_READ
                || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())
                )
        {
            readFromStream();
        }
        else
        {
            Field<Ty>::operator=(list);
        }
    }


    template<class Ty, class BaseType>
    CompactIOField<Ty, BaseType>::CompactIOField
    (
        const IOobject& io,
        const Xfer<Field<Ty>>& list
    )
        :
        regIOobject(io)
    {
        Field<Ty>::transfer(list());

        if
            (
                io.readOpt() == IOobject::MUST_READ
                || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())
                )
        {
            readFromStream();
        }
    }


    // * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * * //

    template<class Ty, class BaseType>
    CompactIOField<Ty, BaseType>::~CompactIOField()
    {}



    // * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

    template<class Ty, class BaseType>
    bool CompactIOField<Ty, BaseType>::writeObject
    (
        IOstream::streamFormat fmt,
        IOstream::versionNumber ver,
        IOstream::compressionType cmp,
        const bool valid
    ) const
    {
        if (fmt == IOstream::ASCII)
        {
            // Change type to be non-compact format type
            const word oldTypeName = typeName;

            const_cast<word&>(typeName) = IOField<Ty>::typeName;

            bool good = regIOobject::writeObject(fmt, ver, cmp, valid);

            // Change type back
            const_cast<word&>(typeName) = oldTypeName;

            return good;
        }
        else
        {
            return regIOobject::writeObject(fmt, ver, cmp, valid);
        }
    }


    template<class Ty, class BaseType>
    bool CompactIOField<Ty, BaseType>::writeData(Ostream& os) const
    {
        return (os << *this).good();
    }


    // * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

    template<class Ty, class BaseType>
    void CompactIOField<Ty, BaseType>::operator=
        (
            const CompactIOField<Ty, BaseType>& rhs
            )
    {
        Field<Ty>::operator=(rhs);
    }


    template<class Ty, class BaseType>
    void CompactIOField<Ty, BaseType>::operator=(const Field<Ty>& rhs)
    {
        Field<Ty>::operator=(rhs);
    }


    // * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

    template<class Ty, class BaseType>
    Istream& operator>>
        (
            Istream& is,
            CompactIOField<Ty, BaseType>& L
            )
    {
        // Read compact
        const labelList start(is);
        const Field<BaseType> elems(is);

        // Convert
        L.setSize(start.size() - 1);

        forAll(L, i)
        {
            Ty& subField = L[i];

            label index = start[i];
            subField.setSize(start[i + 1] - index);

            forAll(subField, j)
            {
                subField[j] = elems[index++];
            }
        }

        return is;
    }


    template<class Ty, class BaseType>
    Ostream& operator<<
        (
            Ostream& os,
            const CompactIOField<Ty, BaseType>& L
            )
    {
        // Keep ascii writing same.
        if (os.format() == IOstream::ASCII)
        {
            os << static_cast<const Field<Ty>&>(L);
        }
        else
        {
            // Convert to compact format
            labelList start(L.size() + 1);

            start[0] = 0;
            for (label i = 1; i < start.size(); i++)
            {
                start[i] = start[i - 1] + L[i - 1].size();
            }

            Field<BaseType> elems(start[start.size() - 1]);

            label elemI = 0;
            forAll(L, i)
            {
                const Ty& subField = L[i];

                forAll(subField, j)
                {
                    elems[elemI++] = subField[j];
                }
            }
            os << start << elems;
        }

        return os;
    }

}
// ************************************************************************* //
