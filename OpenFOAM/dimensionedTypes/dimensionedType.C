/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2016-2020 OpenCFD Ltd.
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

//#include "dimensionedType.H"
#include "pTraits.H"
#include "dictionary2.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


 namespace Foam{
template<class Type>
void dimensioned<Type>::initialize(Istream& is, const bool checkDims)
{
    token nextToken(is);
    is.putBack(nextToken);

    // Optional name found - use it
    if (nextToken.isWord())
    {
        is >> name_;
        is >> nextToken;
        is.putBack(nextToken);
    }

    scalar mult{1};

    if (nextToken == token::BEGIN_SQR)
    {
        // Optional dimensions found - use them
        const dimensionSet curr(dimensions_);
        dimensions_.read(is, mult);

        if (checkDims && curr != dimensions_)
        {
            FatalIOErrorInFunction(is)
                << "The dimensions " << dimensions_
                << " provided do not match the expected dimensions "
                << curr << endl
                << abort(FatalIOError);
        }
    }

    // Read value
    is >> value_;
    value_ *= mult;
}


template<class Type>
bool dimensioned<Type>::readEntry
(
    const word& key,
    const dictionary& dict,
    const bool mandatory,
    const bool checkDims,
    enum keyType::option matchOpt
)
{
    // Largely identical to dictionary::readEntry(),
    // but with optional handling of checkDims

    const entry* eptr = dict.findEntry(key, matchOpt);

    if (eptr)
    {
        ITstream& is = eptr->stream();

        initialize(is, checkDims);

        dict.checkITstream(is, key);

        return true;
    }
    else if (mandatory)
    {
        FatalIOErrorInFunction(dict)
            << "Entry '" << key << "' not found in dictionary "
            << dict.name()
            << exit(FatalIOError);
    }

    return false;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
dimensioned<Type>::dimensioned()
:
    name_("0"),
    dimensions_(),
    value_(Zero)
{}


template<class Type>
dimensioned<Type>::dimensioned(const dimensionSet& dims)
:
    name_("0"),
    dimensions_(dims),
    value_(Zero)
{}


template<class Type>
dimensioned<Type>::dimensioned
(
    const dimensionSet& dims,
    const zero
)
:
    name_("0"),
    dimensions_(dims),
    value_(Zero)
{}


template<class Type>
dimensioned<Type>::dimensioned
(
    const dimensionSet& dims,
    const one
)
:
    name_("1"),
    dimensions_(dims),
    value_(pTraits<Type>::one)
{}


template<class Type>
dimensioned<Type>::dimensioned
(
    const dimensionSet& dims,
    const Type& val
)
:
    name_(::Foam::name(val)),
    dimensions_(dims),
    value_(val)
{}


template<class Type>
dimensioned<Type>::dimensioned
(
    const word& name,
    const dimensionSet& dims,
    const Type& val
)
:
    name_(name),
    dimensions_(dims),
    value_(val)
{}


template<class Type>
dimensioned<Type>::dimensioned
(
    const word& name,
    const dimensioned<Type>& dt
)
:
    name_(name),
    dimensions_(dt.dimensions_),
    value_(dt.value_)
{}


template<class Type>
dimensioned<Type>::dimensioned
(
    const primitiveEntry& e
)
:
    name_(e.name()),
    dimensions_(),
    value_(Zero)
{
    ITstream& is = e.stream();

    // no checkDims
    initialize(is, false);

    e.checkITstream(is);
}


template<class Type>
dimensioned<Type>::dimensioned
(
    const primitiveEntry& e,
    const dimensionSet& dims
)
:
    name_(e.name()),
    dimensions_(dims),
    value_(Zero)
{
    ITstream& is = e.stream();

    // checkDims
    initialize(is, true);

    e.checkITstream(is);
}


template<class Type>
dimensioned<Type>::dimensioned
(
    const word& name,
    const dictionary& dict
)
:
    name_(name),
    dimensions_(),
    value_(Zero)
{
    // mandatory, no checkDims
    readEntry(name, dict, true, false);
}


template<class Type>
dimensioned<Type>::dimensioned
(
    const word& name,
    const dimensionSet& dims,
    const dictionary& dict
)
:
    name_(name),
    dimensions_(dims),
    value_(Zero)
{
    // mandatory, checkDims
    readEntry(name, dict);
}


template<class Type>
dimensioned<Type>::dimensioned
(
    const word& name,
    const dimensionSet& dims,
    const dictionary& dict,
    const word& entryName
)
:
    name_(name),
    dimensions_(dims),
    value_(Zero)
{
    // mandatory, checkDims
    readEntry(entryName, dict);
}


template<class Type>
dimensioned<Type>::dimensioned
(
    const word& name,
    const dimensionSet& dims,
    const Type& val,
    const dictionary& dict
)
:
    name_(name),
    dimensions_(dims),
    value_(val)
{
    // non-mandatory, checkDims
    readEntry(name, dict, false);
}


template<class Type>
dimensioned<Type>::dimensioned(Istream& is)
:
    dimensions_()
{
    read(is);
}


template<class Type>
dimensioned<Type>::dimensioned
(
    const word& name,
    Istream& is
)
:
    name_(name),
    dimensions_()
{
    read(is, false);  // Don't read name. Read dimensionSet + multiplier only.
}


template<class Type>
dimensioned<Type>::dimensioned
(
    const word& name,
    const dimensionSet& dims,
    Istream& is
)
:
    name_(name),
    dimensions_(dims),
    value_(Zero)
{
    // checkDims
    initialize(is, true);
}


// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

template<class Type>
dimensioned<Type> dimensioned<Type>::getOrDefault
(
    const word& name,
    const dictionary& dict,
    const dimensionSet& dims,
    const Type& deflt
)
{
    // checkDims = true
    return dimensioned<Type>(name, dims, deflt, dict);
}


template<class Type>
dimensioned<Type> dimensioned<Type>::getOrDefault
(
    const word& name,
    const dictionary& dict,
    const Type& deflt
)
{
    return dimensioned<Type>(name, dimless, deflt, dict);
}


template<class Type>
dimensioned<Type> dimensioned<Type>::getOrAddToDict
(
    const word& name,
    dictionary& dict,
    const dimensionSet& dims,
    const Type& deflt
)
{
    if (dict.found(name))
    {
        return dimensioned<Type>(name, dims, dict);
    }

    (void) dict.add(name, deflt);
    return dimensioned<Type>(name, dims, deflt);
}


template<class Type>
dimensioned<Type> dimensioned<Type>::getOrAddToDict
(
    const word& name,
    dictionary& dict,
    const Type& deflt
)
{
    return getOrAddToDict(name, dict, dimless, deflt);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
const word& dimensioned<Type>::name() const
{
    return name_;
}


template<class Type>
word& dimensioned<Type>::name()
{
    return name_;
}


template<class Type>
const dimensionSet& dimensioned<Type>::dimensions() const
{
    return dimensions_;
}


template<class Type>
dimensionSet& dimensioned<Type>::dimensions()
{
    return dimensions_;
}


template<class Type>
const Type& dimensioned<Type>::value() const
{
    return value_;
}


template<class Type>
Type& dimensioned<Type>::value()
{
    return value_;
}


template<class Type>
dimensioned<typename dimensioned<Type>::cmptType>
dimensioned<Type>::component
(
    const direction d
) const
{
    return dimensioned<cmptType>
    (
        name_ + ".component(" + name(d) + ')',
        dimensions_,
        value_.component(d)
    );
}


template<class Type>
void dimensioned<Type>::replace
(
    const direction d,
    const dimensioned<typename dimensioned<Type>::cmptType>& dc
)
{
    dimensions_ = dc.dimensions();
    value_.replace(d, dc.value());
}


template<class Type>
bool dimensioned<Type>::read(const dictionary& dict)
{
    return read(name_, dict);
}


template<class Type>
bool dimensioned<Type>::readIfPresent(const dictionary& dict)
{
    return readIfPresent(name_, dict);
}


template<class Type>
bool dimensioned<Type>::read
(
    const word& entryName,
    const dictionary& dict
)
{
    // mandatory, checkDims
    return readEntry(entryName, dict);
}


template<class Type>
bool dimensioned<Type>::readIfPresent
(
    const word& entryName,
    const dictionary& dict
)
{
    // non-mandatory, checkDims
    return readEntry(entryName, dict, false);
}


template<class Type>
Istream& dimensioned<Type>::read(Istream& is, const bool readName)
{
    if (readName)
    {
        // Read name
        is >> name_;
    }

    // Read dimensionSet + multiplier
    scalar mult{1};
    dimensions_.read(is, mult);

    // Read value
    is >> value_;
    value_ *= mult;

    is.check(FUNCTION_NAME);
    return is;
}


template<class Type>
Istream&
dimensioned<Type>::read(Istream& is, const dictionary& readSet)
{
    // Read name
    is >> name_;

    // Read dimensionSet + multiplier
    scalar mult{1};
    dimensions_.read(is, mult, readSet);

    // Read value
    is >> value_;
    value_ *= mult;

    is.check(FUNCTION_NAME);
    return is;
}


template<class Type>
Istream& dimensioned<Type>::read
(
    Istream& is,
    const HashTable<dimensionedScalar>& readSet
)
{
    // Read name
    is >> name_;

    // Read dimensionSet + multiplier
    scalar mult{1};
    dimensions_.read(is, mult, readSet);

    // Read value
    is >> value_;
    value_ *= mult;

    is.check(FUNCTION_NAME);
    return is;
}


template<class Type>
void dimensioned<Type>::writeEntry
(
    const word& keyword,
    Ostream& os
) const
{
    os.writeKeyword(keyword);

    if (keyword != name_)
    {
        // The name, only if different from keyword
        os << name_ << token::SPACE;
    }

    // The dimensions
    scalar mult{1};
    dimensions_.write(os, mult);

    // The value
    os << token::SPACE << value_/mult << token::END_STATEMENT << endl;

    os.check(FUNCTION_NAME);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Type>
dimensioned<typename dimensioned<Type>::cmptType>
dimensioned<Type>::operator[]
(
    const direction d
) const
{
    return component(d);
}


template<class Type>
void dimensioned<Type>::operator+=
(
    const dimensioned<Type>& dt
)
{
    dimensions_ += dt.dimensions_;
    value_ += dt.value_;
}
 
template<class Type>
void dimensioned<Type>::operator-=
(
    const dimensioned<Type>& dt
)
{
    dimensions_ -= dt.dimensions_;
    value_ -= dt.value_;
}


template<class Type>
void dimensioned<Type>::operator*=
(
    const scalar s
)
{
    value_ *= s;
}


template<class Type>
void dimensioned<Type>::operator/=
(
    const scalar s
)
{
    value_ /= s;
}

template<class Type>
bool dimensioned<Type>::operator==
(
    const dimensioned<Type>& dt
    ) const
{
    return name_ == dt.name_ && dimensions_ == dt.dimensions_ && value_ == dt.value_;
}

template<class Type>
bool dimensioned<Type>::operator!=
(
    const dimensioned<Type>& dt
    ) const
{
    return !(name_ == dt.name_ && dimensions_ == dt.dimensions_ && value_ == dt.value_);
}


// * * * * * * * * * * * * * * * Friend Functions  * * * * * * * * * * * * * //

template<class Type, direction r>
dimensioned<typename powProduct<Type, r>::type>
pow(const dimensioned<Type>& dt, typename powProduct<Type, r>::type)
{
    return dimensioned<typename powProduct<Type, r>::type>
    (
        "pow(" + dt.name() + ',' + name(r) + ')',
        pow(dt.dimensions(), r),
        pow(dt.value(), 2)
    );
}


template<class Type>
dimensioned<typename outerProduct<Type, Type>::type>
sqr(const dimensioned<Type>& dt)
{
    return dimensioned<typename outerProduct<Type, Type>::type>
    (
        "sqr(" + dt.name() + ')',
        sqr(dt.dimensions()),
        sqr(dt.value())
    );
}

template<class Type>
dimensioned<typename typeOfMag<Type>::type>
magSqr(const dimensioned<Type>& dt)
{
    typedef typename typeOfMag<Type>::type magType;

    return dimensioned<magType>
    (
        "magSqr(" + dt.name() + ')',
        magSqr(dt.dimensions()),
        magSqr(dt.value())
    );
}

template<class Type>
dimensioned<typename typeOfMag<Type>::type>
mag(const dimensioned<Type>& dt)
{
    typedef typename typeOfMag<Type>::type magType;

    return dimensioned<magType>
    (
        "mag(" + dt.name() + ')',
        dt.dimensions(),
        mag(dt.value())
    );
}


template<class Type>
dimensioned<Type> cmptMultiply
(
    const dimensioned<Type>& dt1,
    const dimensioned<Type>& dt2
)
{
    return dimensioned<Type>
    (
        "cmptMultiply(" + dt1.name() + ',' + dt2.name() + ')',
        cmptMultiply(dt1.dimensions(), dt2.dimensions()),
        cmptMultiply(dt1.value(), dt2.value())
    );
}

template<class Type>
dimensioned<Type> cmptDivide
(
    const dimensioned<Type>& dt1,
    const dimensioned<Type>& dt2
)
{
    return dimensioned<Type>
    (
        "cmptDivide(" + dt1.name() + ',' + dt2.name() + ')',
        cmptDivide(dt1.dimensions(), dt2.dimensions()),
        cmptDivide(dt1.value(), dt2.value())
    );
}


template<class Type>
dimensioned<Type> max
(
    const dimensioned<Type>& dt1,
    const dimensioned<Type>& dt2
)
{
    if (dt1.dimensions() != dt2.dimensions())
    {
        FatalErrorInFunction
            << "dimensions of arguments are not equal"
            << abort(FatalError);
    }

    return dimensioned<Type>
    (
        "max(" + dt1.name() + ',' + dt2.name() + ')',
        dt1.dimensions(),
        max(dt1.value(), dt2.value())
    );
}


template<class Type>
dimensioned<Type> min
(
    const dimensioned<Type>& dt1,
    const dimensioned<Type>& dt2
)
{
    if (dt1.dimensions() != dt2.dimensions())
    {
        FatalErrorInFunction
            << "dimensions of arguments are not equal"
            << abort(FatalError);
    }

    return dimensioned<Type>
    (
        "min(" + dt1.name() + ',' + dt2.name() + ')',
        dt1.dimensions(),
        min(dt1.value(), dt2.value())
    );
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Type>
Istream& operator>>(Istream& is, dimensioned<Type>& dt)
{
    dt.initialize(is, false);  // no checkDims
    is.check(FUNCTION_NAME);
    return is;
}


template<class Type>
Ostream& operator<<(Ostream& os, const dimensioned<Type>& dt)
{
    // The name
    os << dt.name() << token::SPACE;

    // The dimensions
    scalar mult{1};
    dt.dimensions().write(os, mult);

    // The value
    os << token::SPACE << dt.value()/mult;

    os.check(FUNCTION_NAME);
    return os;
}


// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

template<class Type>
bool operator<
(
    const dimensioned<Type>& dt1,
    const dimensioned<Type>& dt2
)
{
    return dt1.value() < dt2.value();
}


template<class Type>
bool operator>
(
    const dimensioned<Type>& dt1,
    const dimensioned<Type>& dt2
)
{
    return dt2.value() < dt1.value();
}


template<class Type>
dimensioned<Type> operator+
(
    const dimensioned<Type>& dt1,
    const dimensioned<Type>& dt2
)
{
    return dimensioned<Type>
    (
        '(' + dt1.name() + '+' + dt2.name() + ')',
        dt1.dimensions() + dt2.dimensions(),
        dt1.value() + dt2.value()
    );
}


template<class Type>
dimensioned<Type> operator-(const dimensioned<Type>& dt)
{
    return dimensioned<Type>
    (
        '-' + dt.name(),
        dt.dimensions(),
        -dt.value()
    );
}


template<class Type>
dimensioned<Type> operator-
(
    const dimensioned<Type>& dt1,
    const dimensioned<Type>& dt2
)
{
    return dimensioned<Type>
    (
        '(' + dt1.name() + '-' + dt2.name() + ')',
        dt1.dimensions() - dt2.dimensions(),
        dt1.value() - dt2.value()
    );
}


template<class Type>
dimensioned<Type> operator*
(
    const dimensioned<scalar>& ds,
    const dimensioned<Type>& dt
)
{
    return dimensioned<Type>
    (
        '(' + ds.name() + '*' + dt.name() + ')',
        ds.dimensions() * dt.dimensions(),
        ds.value() * dt.value()
    );
}


template<class Type>
dimensioned<Type> operator/
(
    const dimensioned<Type>& dt,
    const dimensioned<scalar>& ds
)
{
    return dimensioned<Type>
    (
        '(' + dt.name() + '|' + ds.name() + ')',
        dt.dimensions() / ds.dimensions(),
        dt.value() / ds.value()
    );
}


#define PRODUCT_OPERATOR(product, op, opFunc)                                  \
                                                                               \
template<class Type1, class Type2>                                             \
dimensioned<typename product<Type1, Type2>::type>                  \
operator op                                                              \
(                                                                              \
    const dimensioned<Type1>& dt1,                                             \
    const dimensioned<Type2>& dt2                                              \
)                                                                              \
{                                                                              \
    return dimensioned<typename product<Type1, Type2>::type>                   \
    (                                                                          \
        '(' + dt1.name() + #op + dt2.name() + ')',                             \
        dt1.dimensions() op dt2.dimensions(),                                  \
        dt1.value() op dt2.value()                                             \
    );                                                                         \
}                                                                              \
                                                                               \
template<class Type, class Form, class Cmpt, direction nCmpt>            \
dimensioned<typename product<Type, Form>::type>                    \
operator op                                                              \
(                                                                              \
    const dimensioned<Type>& dt1,                                              \
    const VectorSpace<Form,Cmpt,nCmpt>& t2                                     \
)                                                                              \
{                                                                              \
    return dimensioned<typename product<Type, Form>::type>                     \
    (                                                                          \
        '(' + dt1.name() + #op + name(t2) + ')',                               \
        dt1.dimensions(),                                                      \
        dt1.value() op static_cast<const Form&>(t2)                            \
    );                                                                         \
}                                                                              \
                                                                               \
template<class Type, class Form, class Cmpt, direction nCmpt>            \
dimensioned<typename product<Form, Type>::type>                    \
operator op                                                              \
(                                                                              \
    const VectorSpace<Form,Cmpt,nCmpt>& t1,                                    \
    const dimensioned<Type>& dt2                                               \
)                                                                              \
{                                                                              \
    return dimensioned<typename product<Form, Type>::type>                     \
    (                                                                          \
        '(' + name(t1) + #op + dt2.name() + ')',                               \
        dt2.dimensions(),                                                      \
        static_cast<const Form&>(t1) op dt2.value()                            \
    );                                                                         \
}


PRODUCT_OPERATOR(outerProduct, *, outer)
PRODUCT_OPERATOR(crossProduct, ^, cross)
PRODUCT_OPERATOR(innerProduct, &, dot)
PRODUCT_OPERATOR(scalarProduct, &&, dotdot)

#undef PRODUCT_OPERATOR


// ************************************************************************* //

 } // End namespace Foam
