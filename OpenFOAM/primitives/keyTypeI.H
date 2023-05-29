/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2017-2021 OpenCFD Ltd.
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

// * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * * //


 namespace Foam{
inline bool keyType::valid(const char c)
{
    // Also accept '{' and '}' (for regex grouping?)
    return (word::valid(c) || c == '{' || c == '}');
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline keyType::keyType()
:
    word(),
    type_(option::LITERAL)
{}


inline keyType::keyType(const word& str)
:
    word(str),
    type_(option::LITERAL)
{}


inline keyType::keyType(word&& str)
:
    word(std::move(str)),
    type_(option::LITERAL)
{}


inline keyType::keyType(const string& str)
:
    word(str, false),  // No stripping
    type_(option::REGEX)
{}


inline keyType::keyType(string&& str)
:
    word(std::move(str), false),  // No stripping
    type_(option::REGEX)
{}


inline keyType::keyType(const std::string& str, option opt)
:
    word(str, false),  // No stripping
    type_(option(opt & 0x0F))
{}


inline keyType::keyType(std::string&& str, option opt)
:
    word(std::move(str), false),  // No stripping
    type_(option(opt & 0x0F))
{}


inline keyType::keyType(const char* str, option opt)
:
    word(str, false),  // No stripping
    type_(option(opt & 0x0F))
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline bool keyType::isLiteral() const noexcept
{
    return !(type_ & option::REGEX);
}


inline bool keyType::isPattern() const noexcept
{
    return (type_ & option::REGEX);
}


inline void keyType::setType(option opt, bool adjust)
{
    opt = option(opt & 0x0F);

    if (type_ != opt)
    {
        // Only strip when debug is active (potentially costly operation)
        if (adjust && isPattern() && word::debug)
        {
            string::stripInvalid<word>(*this);
        }

        type_ = opt;
    }
}


inline bool keyType::compile() noexcept
{
    type_ = option::REGEX;
    return true;
}


inline void keyType::uncompile() noexcept
{
    type_ = option::LITERAL;
}


inline void keyType::uncompile(bool adjust)
{
    // Only strip when debug is active (potentially costly operation)
    if (adjust && isPattern() && word::debug)
    {
        string::stripInvalid<word>(*this);
    }

    type_ = option::LITERAL;
}


inline void keyType::clear()
{
    word::clear();
    type_ = option::LITERAL;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline bool keyType::operator()(const std::string& text) const
{
    return match(text);  // Use smart match
}


inline void keyType::operator=(const keyType& str)
{
    if (this == &str)
    {
        return;  // Self-assignment is a no-op
    }

    assign(str); // Bypasses char checking
    type_ = str.type_;
}


inline void keyType::operator=(keyType&& str)
{
    if (this == &str)
    {
        return;  // Self-assignment is a no-op
    }

    clear();
    swap(str);
}


inline void keyType::operator=(const char* str)
{
    assign(str); // Bypasses char checking
    type_ = option::LITERAL;
}


inline void keyType::operator=(const word& str)
{
    assign(str); // Bypasses char checking
    type_ = option::LITERAL;
}


inline void keyType::operator=(const string& str)
{
    assign(str); // Bypasses char checking
    type_ = option::REGEX;
}


// ************************************************************************* //

 } // End namespace Foam
