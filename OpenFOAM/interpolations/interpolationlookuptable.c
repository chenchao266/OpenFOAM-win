﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
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


// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //


 namespace Foam{
template<class Type>
label interpolationLookUpTable<Type>::index
(
    const List<scalar>& indices,
    const bool lastDim
) const
{
    label totalIndex = 0;

    forAll(dim_, i)
    {
        label dim = 1;
        for (int j = i + 1; j < dim_.size(); j++)
        {
            dim *= dim_[j] + 1;
        }

        totalIndex +=
            dim
           *min
            (
                max(label((indices[i] - min_[i])/delta_[i]), 0),
                dim_[i]
            );
    }

    if (lastDim)
    {
        label iLastdim = dim_.size() - 1;
        totalIndex += min
        (
            max
            (
                label((indices[iLastdim] - min_[iLastdim])/delta_[iLastdim]),
                0
            ),
            dim_[iLastdim]
        );
    }

    return totalIndex;
}


template<class Type>
label interpolationLookUpTable<Type>::index
(
    const scalar indice
) const
{
    label i = 0;
    label totalIndex =
        ::Foam::min
        (
            ::Foam::max
            (
                label((indice - min_[i])/delta_[i]),
                0
            ),
            dim_[i]
        );

    return totalIndex;
}


template<class Type>
bool interpolationLookUpTable<Type>::checkRange
(
    const scalar lookUpValue,
    const label interfield
) const
{
    return lookUpValue >= min_[interfield] && lookUpValue <= max_[interfield];
}


template<class Type>
scalar interpolationLookUpTable<Type>::interpolate
(
    const label lo,
    const label hi,
    const scalar lookUpValue,
    const label ofield,
    const label interfield
) const
{
        if
        (
            List<scalarField>::operator[](interfield).operator[](hi)
         != List<scalarField>::operator[](interfield).operator[](lo)
        )
        {
            scalar output
            (
                List<scalarField>::operator[](ofield).operator[](lo)
              + (
                    List<scalarField>::operator[](ofield).operator[](hi)
                  - List<scalarField>::operator[](ofield).operator[](lo)
                )
               *(
                    lookUpValue
                  - List<scalarField>::operator[](interfield).operator[](lo)
                )
               /(
                    List<scalarField>::operator[](interfield).operator[](hi)
                  - List<scalarField>::operator[](interfield).operator[](lo)
                )
            );
            return output;
        }
        else
        {
            return List<scalarField>::operator[](ofield).operator[](lo);
        }
}


template<class Type>
void interpolationLookUpTable<Type>::dimensionTable()
{
    min_.setSize(entries_.size());
    dim_.setSize(entries_.size());
    delta_.setSize(entries_.size());
    max_.setSize(entries_.size());
    entryIndices_.setSize(entries_.size());
    outputIndices_.setSize(output_.size());
    label index = 0;
    label tableDim = 1;

    forAll(entries_,i)
    {
        dim_[i] = entries_[i].template get<label>("N");
        max_[i] = entries_[i].template get<scalar>("max");
        min_[i] = entries_[i].template get<scalar>("min");
        delta_[i] = (max_[i] - min_[i])/dim_[i];
        tableDim *= dim_[i] + 1;
        fieldIndices_.insert(entries_[i].template get<word>("name"), index);
        entryIndices_[i] = index;
        index++;
    }

    forAll(output_,i)
    {
        fieldIndices_.insert(output_[i].template get<word>("name"), index);
        outputIndices_[i] = index;
        index++;
    }

    List<scalarField>& internal = *this;

    internal.setSize(entries_.size() + output_.size());

    interpOutput_.setSize(entries_.size() + output_.size());

    forAll(internal, i)
    {
        internal[i].setSize(tableDim);
    }
}


template<class Type>
void interpolationLookUpTable<Type>::readTable
(
    const word& instance,
    const objectRegistry& obr
)
{
    IOdictionary control
    (
        IOobject
        (
            fileName_,
            instance,
            obr,
            IOobject::MUST_READ_IF_MODIFIED,
            IOobject::NO_WRITE
        )
    );

    control.readEntry("fields", entries_);
    control.readEntry("output", output_);
    control.readEntry("values", *this);

    dimensionTable();

    check();

    if (this->size() == 0)
    {
        FatalErrorInFunction
            << "table is empty" << nl << exit(FatalError);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
interpolationLookUpTable<Type>::interpolationLookUpTable()
:
    List<scalarField>(),
    fileName_("fileNameIsUndefined")
{}


template<class Type>
interpolationLookUpTable<Type>::interpolationLookUpTable
(
    const fileName& fn,
    const word& instance,
    const objectRegistry& obr
)
:
    List<scalarField>(),
    fileName_(fn),
    dim_(0),
    min_(0),
    delta_(0.0),
    max_(0.0),
    entries_(0),
    output_(0),
    entryIndices_(0),
    outputIndices_(0),
    interpOutput_(0)
{
    readTable(instance, obr);
}


template<class Type>
interpolationLookUpTable<Type>::interpolationLookUpTable
(
     const interpolationLookUpTable& interpTable
)
:
    List<scalarField>(interpTable),
    fileName_(interpTable.fileName_),
    entryIndices_(interpTable.entryIndices_),
    outputIndices_(interpTable.outputIndices_),
    dim_(interpTable.dim_),
    min_(interpTable.min_),
    delta_(interpTable.delta_),
    max_(interpTable.max_),
    entries_(0),
    output_(0),
    interpOutput_(interpTable.interpOutput_)
{}


template<class Type>
interpolationLookUpTable<Type>::interpolationLookUpTable
(
    const dictionary& dict
)
:
    List<scalarField>(),
    fileName_(dict.template get<fileName>("file").expand()),
    dim_(0),
    min_(0.0),
    delta_(0.0),
    max_(0.0),
    entries_(dict.lookup("fields")),
    output_(dict.lookup("output")),
    entryIndices_(0),
    outputIndices_(0),
    fieldIndices_(0),
    interpOutput_(0)
{
    dimensionTable();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void interpolationLookUpTable<Type>::check() const
{
    // check order in the first dimension.
    scalar prevValue = List<scalarField>::operator[](0).operator[](0);
    label dim = 1;
    for (int j = 1; j < dim_.size(); j++)
    {
        dim *= dim_[j] + 1;
    }

    for (label i = 1; i < dim_[0]; i++)
    {
        label index = i*dim;
        const scalar currValue =
            List<scalarField>::operator[](0).operator[](index);

        // avoid duplicate values (divide-by-zero error)
        if (currValue <= prevValue)
        {
            FatalErrorInFunction
                << "out-of-order value: " << currValue
                << " at index " << index << nl << exit(FatalError);
        }
        prevValue = currValue;
    }
}


template<class Type>
void interpolationLookUpTable<Type>::write
(
    Ostream& os,
    const fileName& fn,
    const word& instance,
    const objectRegistry& obr
) const
{
    IOdictionary control
    (
        IOobject
        (
            fn,
            instance,
            obr,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        )
    );

    control.writeHeader(os);

    os.writeEntry("fields", entries_);
    os.writeEntry("output", output_);

    if (this->size() == 0)
    {
        FatalErrorInFunction
            << "table is empty" << nl << exit(FatalError);
    }
    os.writeEntry("values", *this);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Type>
scalarField&
interpolationLookUpTable<Type>::operator[](const label i)
{
    const label n = this->size();

    if (n <= 1)
    {
        FatalErrorInFunction
            << "table has (" << n << ") columns" << nl << exit(FatalError);
    }
    else if (i < 0)
    {
        FatalErrorInFunction
            << "index (" << i << ") underflow" << nl << exit(FatalError);
    }
    else if (i >= n)
    {
        FatalErrorInFunction
            << "index (" << i << ") overflow" << nl << exit(FatalError);
    }

    return List<scalarField>::operator[](i);
}


template<class Type>
const scalarField&
interpolationLookUpTable<Type>::operator[](const label i) const
{
    const label n = this->size();

    if (n <= 1)
    {
        FatalErrorInFunction
            << "table has (" << n << ") columns" << nl << exit(FatalError);
    }
    else if (i < 0)
    {
        FatalErrorInFunction
            << "index (" << i << ") underflow" << nl << exit(FatalError);
    }
    else if (i >= n)
    {
        FatalErrorInFunction
            << "index (" << i << ") overflow" << nl
            << exit(FatalError);
    }

    return List<scalarField>::operator[](i);
}


template<class Type>
bool interpolationLookUpTable<Type>::found(const word& fieldName) const
{
    return fieldIndices_.found(fieldName);
}


template<class Type>
const scalarList&
interpolationLookUpTable<Type>::lookUp(const scalar retvals)
{
    const label lo = index(retvals);
    findHi(lo, retvals);
    return interpOutput_;
}


template<class Type>
void interpolationLookUpTable<Type>::findHi
(
    const label lo,
    const scalar retvals
)
{
    forAll(outputIndices_,j)
    {
        scalar tmp = 0;
        label ofield = outputIndices_[j];
        scalar baseValue = List<scalarField>::operator[](ofield).operator[](lo);

        forAll(entryIndices_,i)
        {
            if (checkRange(retvals, entryIndices_[i]))
            {
                label dim = 1;

                label hi = ::Foam::min(lo + dim, (*this)[0].size() - 1);

                tmp += interpolate(lo, hi, retvals, ofield, entryIndices_[i])
                     - baseValue;
            }
            interpOutput_[entryIndices_[i]] = retvals;
        }

        tmp += baseValue;
        interpOutput_[outputIndices_[j]] = tmp;
    }
}


// ************************************************************************* //

 } // End namespace Foam
