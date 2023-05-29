/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2020 OpenCFD Ltd.
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

// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //


 namespace Foam{
inline clockValue clockValue::now()
{
    return clockValue(true);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline clockValue::clockValue()
:
    value_(value_type::zero())
{}


inline clockValue::clockValue(bool)
:
    value_(std::chrono::high_resolution_clock::now().time_since_epoch())
{}


inline clockValue::clockValue(const value_type& value)
:
    value_(value)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline void clockValue::clear()
{
    value_ = value_type::zero();
}


inline void clockValue::update()
{
    value_ = std::chrono::high_resolution_clock::now().time_since_epoch();
}


inline long clockValue::seconds() const
{
    return std::chrono::duration_cast<std::chrono::seconds>(value_).count();
}


inline clockValue clockValue::elapsed() const
{
    return (clockValue::now() -= *this);
}


inline double clockValue::elapsedTime() const
{
    return static_cast<double>(this->elapsed());
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline clockValue::operator double() const
{
    return
    (
        (double(value_.count()) * value_type::period::num)
      / value_type::period::den
    );
}


inline clockValue& clockValue::operator-=(const clockValue& rhs)
{
    value_ -= rhs.value_;
    return *this;
}


inline clockValue& clockValue::operator+=(const clockValue& rhs)
{
    value_ += rhs.value_;
    return *this;
}


// ************************************************************************* //

 } // End namespace Foam
