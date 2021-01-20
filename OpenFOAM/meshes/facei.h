/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011 OpenFOAM Foundation
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

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //
namespace Foam {
// Edge to the right of face vertex i
inline label face::right(const label i) const
{
    return i;
}


// Edge to the left of face vertex i
inline label face::left(const label i) const
{
    return rcIndex(i);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline face::face()
{}


inline face::face(label s) :    labelList(s, -1)
{}


inline face::face(const labelUList& lst) :    labelList(lst)
{}


inline face::face(const labelList& lst) :    labelList(lst)
{}


inline face::face(const Xfer<labelList>& lst) :    labelList(lst)
{}


inline face::face(Istream& is)
{
    is >> *this;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline pointField face::points(const pointField& meshPoints) const
{
    // There are as many points as there labels for them
    pointField p(size());

    // For each point in list, set it to the point in 'pnts' addressed
    // by 'labs'
    forAll(p, i)
    {
        p[i] = meshPoints[operator[](i)];
    }

    // Return list
    return p;
}


inline scalar face::mag(const pointField& p) const
{
    return ::Foam::mag(normal(p));
}


inline label face::nEdges() const
{
    // for a closed polygon a number of edges is the same as number of points
    return size();
}


inline edge face::faceEdge(const label n) const
{
    return edge(operator[](n), operator[](fcIndex(n)));
}


// Next vertex on face
inline label face::nextLabel(const label i) const
{
    return operator[](fcIndex(i));
}


// Previous vertex on face
inline label face::prevLabel(const label i) const
{
    return operator[](rcIndex(i));
}

// Number of triangles directly known from number of vertices
inline label face::nTriangles() const
{
    return size() - 2;
}

// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

inline bool operator==(const face& a, const face& b)
{
    return face::compare(a,b) != 0;
}


inline bool operator!=(const face& a, const face& b)
{
    return face::compare(a,b) == 0;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

inline Istream& operator>>(Istream& is, face& f)
{
    if (is.version() == IOstream::originalVersion)
    {
        // Read starting (
        is.readBegin("face");

        // Read the 'name' token for the face
        token t(is);

        // Read labels
        is >> static_cast<labelList&>(f);

        // Read end)
        is.readEnd("face");
    }
    else
    {
        is >> static_cast<labelList&>(f);
    }

    // Check state of Ostream
    is.check("Istream& operator>>(Istream&, face&)");

    return is;
}
}
// ************************************************************************* //
