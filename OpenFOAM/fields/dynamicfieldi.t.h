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

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //
namespace Foam {
    template<class Ty, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
    inline DynamicField<Ty, SizeInc, SizeMult, SizeDiv>::DynamicField()
        :
        Field<Ty>(0),
        capacity_(Field<Ty>::size())
    {}


    template<class Ty, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
    inline DynamicField<Ty, SizeInc, SizeMult, SizeDiv>::DynamicField
    (
        const label nElem
    )
        :
        Field<Ty>(nElem),
        capacity_(Field<Ty>::size())
    {
        // we could also enforce SizeInc granularity when (!SizeMult || !SizeDiv)
        Field<Ty>::size(0);
    }


    template<class Ty, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
    inline DynamicField<Ty, SizeInc, SizeMult, SizeDiv>::DynamicField
    (
        const UList<Ty>& lst
    )
        :
        Field<Ty>(lst),
        capacity_(Field<Ty>::size())
    {}


    template<class Ty, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
    inline DynamicField<Ty, SizeInc, SizeMult, SizeDiv>::DynamicField
    (
        const Xfer<List<Ty>>& lst
    )
        :
        Field<Ty>(lst),
        capacity_(Field<Ty>::size())
    {}


    template<class Ty, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
    inline DynamicField<Ty, SizeInc, SizeMult, SizeDiv>::DynamicField
    (
        const UList<Ty>& mapF,
        const labelList& mapAddressing
    )
        :
        Field<Ty>(mapF, mapAddressing),
        capacity_(Field<Ty>::size())
    {}


    template<class Ty, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
    inline DynamicField<Ty, SizeInc, SizeMult, SizeDiv>::DynamicField
    (
        const UList<Ty>& mapF,
        const labelListList& mapAddressing,
        const scalarListList& weights
    )
        :
        Field<Ty>(mapF, mapAddressing, weights),
        capacity_(Field<Ty>::size())
    {}


    //- Construct by mapping from the given field
    template<class Ty, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
    inline DynamicField<Ty, SizeInc, SizeMult, SizeDiv>::DynamicField
    (
        const UList<Ty>& mapF,
        const FieldMapper& map
    )
        :
        Field<Ty>(mapF, map),
        capacity_(Field<Ty>::size())
    {}


    template<class Ty, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
    inline DynamicField<Ty, SizeInc, SizeMult, SizeDiv>::DynamicField
    (
        const DynamicField<Ty, SizeInc, SizeMult, SizeDiv>& lst
    )
        :
        Field<Ty>(lst),
        capacity_(lst.capacity())
    {}


    template<class Ty, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
    inline DynamicField<Ty, SizeInc, SizeMult, SizeDiv>::DynamicField
    (
        const Xfer<DynamicField<Ty, SizeInc, SizeMult, SizeDiv>>& lst
    )
        :
        Field<Ty>(lst),
        capacity_(Field<Ty>::size())
    {}


    // * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

    template<class Ty, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
    inline label DynamicField<Ty, SizeInc, SizeMult, SizeDiv>::capacity()
        const
    {
        return capacity_;
    }


    template<class Ty, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
    inline void DynamicField<Ty, SizeInc, SizeMult, SizeDiv>::setCapacity
    (
        const label nElem
    )
    {
        label nextFree = Field<Ty>::size();
        capacity_ = nElem;

        if (nextFree > capacity_)
        {
            // truncate addressed sizes too
            nextFree = capacity_;
        }
        // we could also enforce SizeInc granularity when (!SizeMult || !SizeDiv)

        Field<Ty>::setSize(capacity_);
        Field<Ty>::size(nextFree);
    }


    template<class Ty, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
    inline void DynamicField<Ty, SizeInc, SizeMult, SizeDiv>::reserve
    (
        const label nElem
    )
    {
        // allocate more capacity?
        if (nElem > capacity_)
        {
            // TODO: convince the compiler that division by zero does not occur
            //        if (SizeInc && (!SizeMult || !SizeDiv))
            //        {
            //            // resize with SizeInc as the granularity
            //            capacity_ = nElem;
            //            unsigned pad = SizeInc - (capacity_ % SizeInc);
            //            if (pad != SizeInc)
            //            {
            //                capacity_ += pad;
            //            }
            //        }
            //        else
            {
                capacity_ = max
                (
                    nElem,
                    label(SizeInc + capacity_ * SizeMult / SizeDiv)
                );
            }

            // adjust allocated size, leave addressed size untouched
            label nextFree = Field<Ty>::size();
            Field<Ty>::setSize(capacity_);
            Field<Ty>::size(nextFree);
        }
    }


    template<class Ty, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
    inline void DynamicField<Ty, SizeInc, SizeMult, SizeDiv>::setSize
    (
        const label nElem
    )
    {
        // allocate more capacity?
        if (nElem > capacity_)
        {
            // TODO: convince the compiler that division by zero does not occur
            //        if (SizeInc && (!SizeMult || !SizeDiv))
            //        {
            //            // resize with SizeInc as the granularity
            //            capacity_ = nElem;
            //            unsigned pad = SizeInc - (capacity_ % SizeInc);
            //            if (pad != SizeInc)
            //            {
            //                capacity_ += pad;
            //            }
            //        }
            //        else
            {
                capacity_ = max
                (
                    nElem,
                    label(SizeInc + capacity_ * SizeMult / SizeDiv)
                );
            }

            Field<Ty>::setSize(capacity_);
        }

        // adjust addressed size
        Field<Ty>::size(nElem);
    }


    template<class Ty, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
    inline void DynamicField<Ty, SizeInc, SizeMult, SizeDiv>::setSize
    (
        const label nElem,
        const Ty& t
    )
    {
        label nextFree = Field<Ty>::size();
        setSize(nElem);

        // set new elements to constant value
        while (nextFree < nElem)
        {
            this->operator[](nextFree++) = t;
        }
    }


    template<class Ty, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
    inline void DynamicField<Ty, SizeInc, SizeMult, SizeDiv>::resize
    (
        const label nElem
    )
    {
        this->setSize(nElem);
    }


    template<class Ty, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
    inline void DynamicField<Ty, SizeInc, SizeMult, SizeDiv>::resize
    (
        const label nElem,
        const Ty& t
    )
    {
        this->setSize(nElem, t);
    }


    template<class Ty, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
    inline void DynamicField<Ty, SizeInc, SizeMult, SizeDiv>::clear()
    {
        Field<Ty>::size(0);
    }


    template<class Ty, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
    inline void DynamicField<Ty, SizeInc, SizeMult, SizeDiv>::clearStorage()
    {
        Field<Ty>::clear();
        capacity_ = 0;
    }


    template<class Ty, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
    inline DynamicField<Ty, SizeInc, SizeMult, SizeDiv>&
        DynamicField<Ty, SizeInc, SizeMult, SizeDiv>::shrink()
    {
        label nextFree = Field<Ty>::size();
        if (capacity_ > nextFree)
        {
            // use the full list when resizing
            Field<Ty>::size(capacity_);

            // the new size
            capacity_ = nextFree;
            Field<Ty>::setSize(capacity_);
            Field<Ty>::size(nextFree);
        }
        return *this;
    }


    template<class Ty, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
    inline Xfer<List<Ty>>
        DynamicField<Ty, SizeInc, SizeMult, SizeDiv>::xfer()
    {
        return xferMoveTo<List<Ty>>(*this);
    }


    template<class Ty, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
    inline DynamicField<Ty, SizeInc, SizeMult, SizeDiv>&
        DynamicField<Ty, SizeInc, SizeMult, SizeDiv>::append
        (
            const Ty& t
        )
    {
        const label elemI = List<Ty>::size();
        setSize(elemI + 1);

        this->operator[](elemI) = t;
        return *this;
    }


    template<class Ty, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
    inline DynamicField<Ty, SizeInc, SizeMult, SizeDiv>&
        DynamicField<Ty, SizeInc, SizeMult, SizeDiv>::append
        (
            const UList<Ty>& lst
        )
    {
        if (this == &lst)
        {
            FatalErrorInFunction
                << "attempted appending to self" << abort(FatalError);
        }

        label nextFree = List<Ty>::size();
        setSize(nextFree + lst.size());

        forAll(lst, elemI)
        {
            this->operator[](nextFree++) = lst[elemI];
        }
        return *this;
    }


    template<class Ty, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
    inline Ty DynamicField<Ty, SizeInc, SizeMult, SizeDiv>::remove()
    {
        const label elemI = List<Ty>::size() - 1;

        if (elemI < 0)
        {
            FatalErrorInFunction
                << "List is empty" << abort(FatalError);
        }

        const Ty& val = List<Ty>::operator[](elemI);

        List<Ty>::size(elemI);

        return val;
    }


    // * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

    template<class Ty, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
    inline Ty& DynamicField<Ty, SizeInc, SizeMult, SizeDiv>::operator()
        (
            const label elemI
            )
    {
        if (elemI >= Field<Ty>::size())
        {
            setSize(elemI + 1);
        }

        return this->operator[](elemI);
    }


    template<class Ty, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
    inline void DynamicField<Ty, SizeInc, SizeMult, SizeDiv>::operator=
        (
            const Ty& t
            )
    {
        UList<Ty>::operator=(t);
    }


    template<class Ty, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
    inline void DynamicField<Ty, SizeInc, SizeMult, SizeDiv>::operator=
        (
            const DynamicField<Ty, SizeInc, SizeMult, SizeDiv>& lst
            )
    {
        if (this == &lst)
        {
            FatalErrorInFunction
                << "attempted assignment to self" << abort(FatalError);
        }

        if (capacity_ >= lst.size())
        {
            // can copy w/o reallocating, match initial size to avoid reallocation
            Field<Ty>::size(lst.size());
            Field<Ty>::operator=(lst);
        }
        else
        {
            // make everything available for the copy operation
            Field<Ty>::size(capacity_);

            Field<Ty>::operator=(lst);
            capacity_ = Field<Ty>::size();
        }
    }


    template<class Ty, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
    inline void DynamicField<Ty, SizeInc, SizeMult, SizeDiv>::operator=
        (
            const UList<Ty>& lst
            )
    {
        if (capacity_ >= lst.size())
        {
            // can copy w/o reallocating, match initial size to avoid reallocation
            Field<Ty>::size(lst.size());
            Field<Ty>::operator=(lst);
        }
        else
        {
            // make everything available for the copy operation
            Field<Ty>::size(capacity_);

            Field<Ty>::operator=(lst);
            capacity_ = Field<Ty>::size();
        }
    }

}
// ************************************************************************* //
