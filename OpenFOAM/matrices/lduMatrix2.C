﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
    Copyright (C) 2019-2021 OpenCFD Ltd.
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

#include "lduMatrix2.H"
#include "IOstreams.H"
#include "Switch.H"
#include "objectRegistry.H"
#include "scalarIOField.H"
#include "Time1.h"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(lduMatrix, 1);


    const label lduMatrix::solver::defaultMaxIter_ = 1000;


    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    lduMatrix::lduMatrix(const lduMesh& mesh)
        :
        lduMesh_(mesh),
        lowerPtr_(nullptr),
        diagPtr_(nullptr),
        upperPtr_(nullptr)
    {}


    lduMatrix::lduMatrix(const lduMatrix& A)
        :
        lduMesh_(A.lduMesh_),
        lowerPtr_(nullptr),
        diagPtr_(nullptr),
        upperPtr_(nullptr)
    {
        if (A.lowerPtr_)
        {
            lowerPtr_ = new scalarField(*(A.lowerPtr_));
        }

        if (A.diagPtr_)
        {
            diagPtr_ = new scalarField(*(A.diagPtr_));
        }

        if (A.upperPtr_)
        {
            upperPtr_ = new scalarField(*(A.upperPtr_));
        }
    }


    lduMatrix::lduMatrix(lduMatrix& A, bool reuse)
        :
        lduMesh_(A.lduMesh_),
        lowerPtr_(nullptr),
        diagPtr_(nullptr),
        upperPtr_(nullptr)
    {
        if (reuse)
        {
            if (A.lowerPtr_)
            {
                lowerPtr_ = A.lowerPtr_;
                A.lowerPtr_ = nullptr;
            }

            if (A.diagPtr_)
            {
                diagPtr_ = A.diagPtr_;
                A.diagPtr_ = nullptr;
            }

            if (A.upperPtr_)
            {
                upperPtr_ = A.upperPtr_;
                A.upperPtr_ = nullptr;
            }
        }
        else
        {
            if (A.lowerPtr_)
            {
                lowerPtr_ = new scalarField(*(A.lowerPtr_));
            }

            if (A.diagPtr_)
            {
                diagPtr_ = new scalarField(*(A.diagPtr_));
            }

            if (A.upperPtr_)
            {
                upperPtr_ = new scalarField(*(A.upperPtr_));
            }
        }
    }


    lduMatrix::lduMatrix(const lduMesh& mesh, Istream& is)
        :
        lduMesh_(mesh),
        lowerPtr_(nullptr),
        diagPtr_(nullptr),
        upperPtr_(nullptr)
    {
        Switch hasLow(is);
        Switch hasDiag(is);
        Switch hasUp(is);

        if (hasLow)
        {
            lowerPtr_ = new scalarField(is);
        }
        if (hasDiag)
        {
            diagPtr_ = new scalarField(is);
        }
        if (hasUp)
        {
            upperPtr_ = new scalarField(is);
        }
    }


    lduMatrix::~lduMatrix()
    {
        if (lowerPtr_)
        {
            delete lowerPtr_;
        }

        if (diagPtr_)
        {
            delete diagPtr_;
        }

        if (upperPtr_)
        {
            delete upperPtr_;
        }
    }


    scalarField& lduMatrix::lower()
    {
        if (!lowerPtr_)
        {
            if (upperPtr_)
            {
                lowerPtr_ = new scalarField(*upperPtr_);
            }
            else
            {
                lowerPtr_ = new scalarField(lduAddr().lowerAddr().size(), Zero);
            }
        }

        return *lowerPtr_;
    }


    scalarField& lduMatrix::diag()
    {
        if (!diagPtr_)
        {
            diagPtr_ = new scalarField(lduAddr().size(), Zero);
        }

        return *diagPtr_;
    }


    scalarField& lduMatrix::upper()
    {
        if (!upperPtr_)
        {
            if (lowerPtr_)
            {
                upperPtr_ = new scalarField(*lowerPtr_);
            }
            else
            {
                upperPtr_ = new scalarField(lduAddr().lowerAddr().size(), Zero);
            }
        }

        return *upperPtr_;
    }


    scalarField& lduMatrix::lower(const label nCoeffs)
    {
        if (!lowerPtr_)
        {
            if (upperPtr_)
            {
                lowerPtr_ = new scalarField(*upperPtr_);
            }
            else
            {
                lowerPtr_ = new scalarField(nCoeffs, Zero);
            }
        }

        return *lowerPtr_;
    }


    scalarField& lduMatrix::diag(const label size)
    {
        if (!diagPtr_)
        {
            diagPtr_ = new scalarField(size, Zero);
        }

        return *diagPtr_;
    }


    scalarField& lduMatrix::upper(const label nCoeffs)
    {
        if (!upperPtr_)
        {
            if (lowerPtr_)
            {
                upperPtr_ = new scalarField(*lowerPtr_);
            }
            else
            {
                upperPtr_ = new scalarField(nCoeffs, Zero);
            }
        }

        return *upperPtr_;
    }


    const scalarField& lduMatrix::lower() const
    {
        if (!lowerPtr_ && !upperPtr_)
        {
            FatalErrorInFunction
                << "lowerPtr_ or upperPtr_ unallocated"
                << abort(FatalError);
        }

        if (lowerPtr_)
        {
            return *lowerPtr_;
        }
        else
        {
            return *upperPtr_;
        }
    }


    const scalarField& lduMatrix::diag() const
    {
        if (!diagPtr_)
        {
            FatalErrorInFunction
                << "diagPtr_ unallocated"
                << abort(FatalError);
        }

        return *diagPtr_;
    }


    const scalarField& lduMatrix::upper() const
    {
        if (!lowerPtr_ && !upperPtr_)
        {
            FatalErrorInFunction
                << "lowerPtr_ or upperPtr_ unallocated"
                << abort(FatalError);
        }

        if (upperPtr_)
        {
            return *upperPtr_;
        }
        else
        {
            return *lowerPtr_;
        }
    }


    void lduMatrix::setResidualField
    (
        const scalarField& residual,
        const word& fieldName,
        const bool initial
    ) const
    {
        if (!mesh().hasDb())
        {
            return;
        }

        scalarIOField* residualPtr =
            mesh().thisDb().getObjectPtr<scalarIOField>
            (
                initial
                ? IOobject::scopedName("initialResidual", fieldName)
                : IOobject::scopedName("residual", fieldName)
                );

        if (residualPtr)
        {
            const IOdictionary* dataPtr =
                mesh().thisDb().findObject<IOdictionary>("data");

            if (dataPtr)
            {
                if (initial && dataPtr->found("firstIteration"))
                {
                    *residualPtr = residual;
                    DebugInfo
                        << "Setting residual field for first solver iteration "
                        << "for solver field: " << fieldName << endl;
                }
            }
            else
            {
                *residualPtr = residual;
                DebugInfo
                    << "Setting residual field for solver field "
                    << fieldName << endl;
            }
        }
    }


    // * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

    Ostream& operator<<(Ostream& os, const lduMatrix& ldum)
    {
        Switch hasLow = ldum.hasLower();
        Switch hasDiag = ldum.hasDiag();
        Switch hasUp = ldum.hasUpper();

        os << hasLow << token::SPACE << hasDiag << token::SPACE
            << hasUp << token::SPACE;

        if (hasLow)
        {
            os << ldum.lower();
        }

        if (hasDiag)
        {
            os << ldum.diag();
        }

        if (hasUp)
        {
            os << ldum.upper();
        }

        os.check(FUNCTION_NAME);

        return os;
    }


    Ostream& operator<<(Ostream& os, const InfoProxy<lduMatrix>& ip)
    {
        const lduMatrix& ldum = ip.t_;

        Switch hasLow = ldum.hasLower();
        Switch hasDiag = ldum.hasDiag();
        Switch hasUp = ldum.hasUpper();

        os << "Lower:" << hasLow
            << " Diag:" << hasDiag
            << " Upper:" << hasUp << endl;

        if (hasLow)
        {
            os << "lower:" << ldum.lower().size() << endl;
        }
        if (hasDiag)
        {
            os << "diag :" << ldum.diag().size() << endl;
        }
        if (hasUp)
        {
            os << "upper:" << ldum.upper().size() << endl;
        }


        //if (hasLow)
        //{
        //    os  << "lower contents:" << endl;
        //    forAll(ldum.lower(), i)
        //    {
        //        os  << "i:" << i << "\t" << ldum.lower()[i] << endl;
        //    }
        //    os  << endl;
        //}
        //if (hasDiag)
        //{
        //    os  << "diag contents:" << endl;
        //    forAll(ldum.diag(), i)
        //    {
        //        os  << "i:" << i << "\t" << ldum.diag()[i] << endl;
        //    }
        //    os  << endl;
        //}
        //if (hasUp)
        //{
        //    os  << "upper contents:" << endl;
        //    forAll(ldum.upper(), i)
        //    {
        //        os  << "i:" << i << "\t" << ldum.upper()[i] << endl;
        //    }
        //    os  << endl;
        //}

        os.check(FUNCTION_NAME);

        return os;
    }

}
// ************************************************************************* //
