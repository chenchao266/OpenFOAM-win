/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2015 OpenFOAM Foundation
    Copyright (C) 2017-2019 OpenCFD Ltd.
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

//#include "NamedEnum.H"
#include "dictionary2.H"
#include "stdFoam.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


 namespace Foam{
template<class EnumType, int nEnum>
NamedEnum<EnumType, nEnum>::NamedEnum()
:
    lookup_(2*nEnum)
{
    for (int enumi=0; enumi < nEnum; ++enumi)
    {
        if (names[enumi] && names[enumi][0])
        {
            lookup_.insert(names[enumi], enumi);
        }
        else
        {
            // Bad name - generate error message
            List<string> goodNames(enumi);

            for (int i = 0; i < enumi; ++i)
            {
                goodNames[i] = names[i];
            }

            FatalErrorInFunction
                << "Illegal enumeration name at position " << enumi << nl
                << "after entries " << goodNames << nl
                << "Possibly your NamedEnum<EnumType, nEnum>::names array"
                << " is not of size " << nEnum << endl
                << abort(FatalError);
        }
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class EnumType, int nEnum>
wordList NamedEnum<EnumType, nEnum>::words() const
{
    List<word> lst(nEnum);

    label count = 0;
    for (int enumi=0; enumi < nEnum; ++enumi)
    {
        if (names[enumi] && names[enumi][0])
        {
            lst[count++] = names[enumi];
        }
    }

    lst.setSize(count);
    return lst;
}


template<class EnumType, int nEnum>
List<int> NamedEnum<EnumType, nEnum>::values() const
{
    List<int> lst(nEnum);

    label count = 0;
    for (int enumi=0; enumi < nEnum; ++enumi)
    {
        if (names[enumi] && names[enumi][0])
        {
            auto iter = lookup_.cfind(names[enumi]);

            if (iter.found())
            {
                lst[count++] = iter.val();
            }
        }
    }

    lst.setSize(count);
    return lst;
}


template<class EnumType, int nEnum>
bool NamedEnum<EnumType, nEnum>::hasName(const EnumType e) const
{
    const int enumValue(e);

    forAllConstIters(lookup_, iter)
    {
        if (iter.val() == enumValue)
        {
            return true;
        }
    }
    return false;
}


template<class EnumType, int nEnum>
EnumType NamedEnum<EnumType, nEnum>::lookup
(
    const word& key,
    const dictionary& dict
) const
{
    const word enumName(dict.lookup(key));
    auto iter = lookup_.cfind(enumName);

    if (!iter.found())
    {
        FatalIOErrorInFunction(dict)
            << enumName << " is not in enumeration: "
            << lookup_.sortedToc() << nl
            << exit(FatalIOError);
    }

    return EnumType(iter.val());
}


template<class EnumType, int nEnum>
EnumType NamedEnum<EnumType, nEnum>::lookupOrDefault
(
    const word& key,
    const dictionary& dict,
    const EnumType deflt
) const
{
    if (dict.found(key))
    {
        return lookup(key, dict);
    }

    return deflt;
}


template<class EnumType, int nEnum>
EnumType NamedEnum<EnumType, nEnum>::read(Istream& is) const
{
    const word enumName(is);
    auto iter = lookup_.cfind(enumName);

    if (!iter.found())
    {
        FatalIOErrorInFunction(is)
            << enumName << " is not in enumeration: "
            << lookup_.sortedToc() << nl
            << exit(FatalIOError);
    }

    return EnumType(iter.val());
}


template<class EnumType, int nEnum>
void NamedEnum<EnumType, nEnum>::write
(
    const EnumType e,
    Ostream& os
) const
{
    const int idx = int(e);
    if (idx >= 0 && idx < nEnum)
    {
        os  << names[idx];
    }
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class EnumType, int nEnum>
Ostream& operator<<
(
    Ostream& os,
    const NamedEnum<EnumType, nEnum>& wrapped
)
{
    return wrapped.lookup_.writeKeys(os, 10);
}


// ************************************************************************* //

 } // End namespace Foam
