/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2016 OpenFOAM Foundation
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

#include "string.T.H"
#include "stringOps.H"


/* * * * * * * * * * * * * * * Static Member Data  * * * * * * * * * * * * * */
using namespace Foam;
const char* const string::typeName = "string";
int string::debug(debug::debugSwitch(string::typeName, 0));
const string string::null;


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

string::size_type string::count(const char c) const
{
    size_type cCount = 0;

    for (const_iterator iter = begin(); iter != end(); ++iter)
    {
        if (*iter == c)
        {
            ++cCount;
        }
    }

    return cCount;
}


string& string::replace
(
    const string& oldStr,
    const string& newStr,
    size_type start
)
{
    size_type newStart = start;

    if ((newStart = find(oldStr, newStart)) != npos)
    {
        std::string::replace(newStart, oldStr.size(), newStr);
    }

    return *this;
}


string& string::replaceAll
(
    const string& oldStr,
    const string& newStr,
    size_type start
)
{
    if (oldStr.size())
    {
        size_type newStart = start;

        while ((newStart = find(oldStr, newStart)) != npos)
        {
            std::string::replace(newStart, oldStr.size(), newStr);
            newStart += newStr.size();
        }
    }

    return *this;
}


string& string::expand(const bool allowEmpty)
{
    stringOps::inplaceExpand(*this, allowEmpty);
    return *this;
}


bool string::removeRepeated(const char character)
{
    bool changed = false;

    if (character && find(character) != npos)
    {
        string::size_type nChar=0;
        iterator iter2 = begin();

        char prev = 0;

        for
        (
            string::const_iterator iter1 = iter2;
            iter1 != end();
            iter1++
        )
        {
            char c = *iter1;

            if (prev == c && c == character)
            {
                changed = true;
            }
            else
            {
                *iter2 = prev = c;
                ++iter2;
                ++nChar;
            }
        }
        resize(nChar);
    }

    return changed;
}


string string::removeRepeated(const char character) const
{
    string str(*this);
    str.removeRepeated(character);
    return str;
}


bool string::removeTrailing(const char character)
{
    bool changed = false;

    string::size_type nChar = size();
    if (character && nChar > 1 && operator[](nChar-1) == character)
    {
        resize(nChar-1);
        changed = true;
    }

    return changed;
}


string string::removeTrailing(const char character) const
{
    string str(*this);
    str.removeTrailing(character);
    return str;
}


// ************************************************************************* //
