﻿/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2019 OpenCFD Ltd.
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

#include "xmgrGraph.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(xmgrGraph, 0);
    const word xmgrGraph::ext_("agr");

    typedef graph::writer graphWriter;
    addToRunTimeSelectionTable(graphWriter, xmgrGraph, word);



    // * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

    void xmgrGraph::write(const graph& g, Ostream& os) const
    {
        os << "@title " << g.title() << nl
            << "@xaxis label " << g.xName() << nl
            << "@yaxis label " << g.yName() << endl;

        label fieldi = 0;

        forAllConstIters(g, iter)
        {
            os << "@s" << fieldi << " legend "
                << iter()->name() << nl
                << "@target G0.S" << fieldi << nl
                << "@type xy" << endl;

            writeXY(g.x(), *iter(), os);

            os << endl;

            fieldi++;
        }
    }

}
// ************************************************************************* //
