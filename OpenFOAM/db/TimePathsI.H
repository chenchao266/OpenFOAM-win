/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
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
inline bool TimePaths::distributed() const noexcept
{
    return distributed_;
}


inline bool TimePaths::processorCase() const noexcept
{
    return processorCase_;
}


inline bool TimePaths::processorCase(bool isProcessorCase) noexcept
{
    bool old(processorCase_);
    processorCase_ = isProcessorCase;
    return old;
}


inline const fileName& TimePaths::rootPath() const
{
    return rootPath_;
}


inline const fileName& TimePaths::globalCaseName() const
{
    return globalCaseName_;
}


inline const fileName& TimePaths::caseName() const
{
    return case_;
}


inline fileName& TimePaths::caseName()
{
    return case_;
}


inline fileName TimePaths::path() const
{
    return rootPath()/caseName();
}


inline fileName TimePaths::globalPath() const
{
    return rootPath()/globalCaseName();
}


inline fileName TimePaths::relativePath
(
    const fileName& input,
    const bool caseTag
) const
{
    return input.relative(globalPath(), caseTag);
}


inline const word& TimePaths::constant() const
{
    return constant_;
}


inline const word& TimePaths::system() const
{
    return system_;
}


inline fileName TimePaths::caseConstant() const
{
    if (processorCase_)
    {
        return ".."/constant();
    }

    return constant();
}


inline fileName TimePaths::caseSystem() const
{
    if (processorCase_)
    {
        return ".."/system();
    }

    return system();
}


inline fileName TimePaths::constantPath() const
{
    return path()/constant();
}


inline fileName TimePaths::systemPath() const
{
    return path()/system();
}


// ************************************************************************* //

 } // End namespace Foam
