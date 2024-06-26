﻿#ifndef curveTools_H
#define curveTools_H

#include "scalar.H"
#include "vector2.H"
#include "curve.H"
#include "_char.H"
#include "List.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

#define curveSmall 1.0e-8
#define curveGreat 1.0e8

scalar distance(const vector&, const vector&);
scalar distance(const vector&, const scalar );


bool stepForwardsToNextPoint
(
    const vector&,
    vector&,
    label&,
    label&,
    scalar,
    const curve&
);


bool stepBackwardsToNextPoint
(
    const vector&,
    vector&,
    label&,
    label&,
    scalar,
    const curve&
);


bool interpolate
(
    const vector&,
    const vector&,
    const vector&,
    vector&,
    scalar
);

bool interpolate
(
    const scalar,
    const scalar,
    const vector&,
    vector&,
    scalar
);

bool XstepForwardsToNextPoint
(
    const vector&,
    vector&,
    label&,
    label&,
    scalar,
    const curve&
);


bool Xinterpolate
(
    const vector&,
    const vector&,
    const vector&,
    vector&,
    scalar
);

bool Xinterpolate
(
    const scalar,
    const scalar,
    const vector&,
    vector&,
    scalar
);
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

#endif
