divert(-1)dnl
#-----------------------------------*- m4 -*-----------------------------------
#   =========                 |
#   \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
#    \\    /   O peration     |
#     \\  /    A nd           | www.openfoam.com
#      \\/     M anipulation  |
#------------------------------------------------------------------------------
#     Copyright (C) 2019 OpenCFD Ltd.
#------------------------------------------------------------------------------
# License
#     This file is part of OpenFOAM, licensed under GNU General Public License
#     <http://www.gnu.org/licenses/>.
#
# Description
#     Driver-specific m4/lemon macros for volume expressions.
#
#------------------------------------------------------------------------------

include(`m4/lemon/base-setup.m4')dnl
include([m4/lemon/operator-precedence.m4])dnl
dnl
include([m4/lemon/rules-standard.m4])dnl
include([m4/lemon/rules-operations.m4])dnl
include([m4/lemon/rules-functions.m4])dnl
include([m4/lemon/rules-components.m4])dnl
include([m4/lemon/rules-fields-components.m4])dnl
include([m4/lemon/rules-scalar-logic.m4])dnl
dnl
divert(-1)dnl

#------------------------------------------------------------------------------
# Driver rules
#------------------------------------------------------------------------------

define([rules_driver_volume_functions],
[dnl
rule_driver_select(_logic_, CSET, field_cellSet)dnl
rule_driver_select(_logic_, CZONE, field_cellZone)dnl
dnl
rule_driver_nullary(_scalar_, CELL_VOLUME, field_cellVolume)dnl
rule_driver_nullary(_vector_, POS, field_cellCentre)dnl CELL_CENTRE
dnl
rule_driver_inplace_unary(_scalar_, WEIGHT_AVERAGE, volAverage)dnl
rule_driver_inplace_unary(_vector_, WEIGHT_AVERAGE, volAverage)dnl
rule_driver_inplace_unary(_sphTensor_, WEIGHT_AVERAGE, volAverage)dnl
rule_driver_inplace_unary(_symTensor_, WEIGHT_AVERAGE, volAverage)dnl
rule_driver_inplace_unary(_tensor_, WEIGHT_AVERAGE, volAverage)dnl
dnl
rule_driver_inplace_unary(_scalar_, WEIGHT_SUM, volSum)dnl
rule_driver_inplace_unary(_vector_, WEIGHT_SUM, volSum)dnl
rule_driver_inplace_unary(_sphTensor_, WEIGHT_SUM, volSum)dnl
rule_driver_inplace_unary(_symTensor_, WEIGHT_SUM, volSum)dnl
rule_driver_inplace_unary(_tensor_, WEIGHT_SUM, volSum)dnl
dnl
])

define([rules_driver_surface_functions],
[dnl
rule_driver_select(_logic_, FSET, field_faceSet)dnl
rule_driver_select(_logic_, FZONE, field_faceZone)dnl
dnl
rule_driver_nullary(_scalar_, FACE_AREA, field_faceArea)dnl
rule_driver_nullary(_vector_, FACE_CENTRE, field_faceCentre)dnl
rule_driver_nullary(_vector_, FACE_EXPR, field_areaNormal)dnl
dnl
rule_driver_inplace_unary(_scalar_, WEIGHT_AVERAGE, areaAverage)dnl
rule_driver_inplace_unary(_vector_, WEIGHT_AVERAGE, areaAverage)dnl
rule_driver_inplace_unary(_sphTensor_, WEIGHT_AVERAGE, areaAverage)dnl
rule_driver_inplace_unary(_symTensor_, WEIGHT_AVERAGE, areaAverage)dnl
rule_driver_inplace_unary(_tensor_, WEIGHT_AVERAGE, areaAverage)dnl
dnl
rule_driver_inplace_unary(_scalar_, WEIGHT_SUM, areaSum)dnl
rule_driver_inplace_unary(_vector_, WEIGHT_SUM, areaSum)dnl
rule_driver_inplace_unary(_sphTensor_, WEIGHT_SUM, areaSum)dnl
rule_driver_inplace_unary(_symTensor_, WEIGHT_SUM, areaSum)dnl
rule_driver_inplace_unary(_tensor_, WEIGHT_SUM, areaSum)dnl
dnl
])

define([rules_driver_point_functions],
[dnl
rule_driver_select(_logic_, PSET, field_pointSet)dnl
rule_driver_select(_logic_, PZONE, field_pointZone)dnl
dnl
rule_driver_nullary(_vector_, POINTS, field_pointField)dnl
dnl
dnl NB use non-driver versions for points - ie, unweighted
dnl
rule_inplace_unary(_scalar_, WEIGHT_AVERAGE, Foam::gAverage)dnl
rule_inplace_unary(_vector_, WEIGHT_AVERAGE, Foam::gAverage)dnl
rule_inplace_unary(_sphTensor_, WEIGHT_AVERAGE, Foam::gAverage)dnl
rule_inplace_unary(_symTensor_, WEIGHT_AVERAGE, Foam::gAverage)dnl
rule_inplace_unary(_tensor_, WEIGHT_AVERAGE, Foam::gAverage)dnl
dnl
rule_inplace_unary(_scalar_, WEIGHT_SUM, Foam::gSum)dnl
rule_inplace_unary(_vector_, WEIGHT_SUM, Foam::gSum)dnl
rule_inplace_unary(_sphTensor_, WEIGHT_SUM, Foam::gSum)dnl
rule_inplace_unary(_symTensor_, WEIGHT_SUM, Foam::gSum)dnl
rule_inplace_unary(_tensor_, WEIGHT_SUM, Foam::gSum)dnl
dnl
])


#------------------------------------------------------------------------------
# rule_cellToFace(out, in)
# rule_cellToPoint(out, in)
# rule_pointToCell(out, in)
#
# Description
#     Production rules for driver cellToFace, cellToPoint, pointToCell,
#     methods
#------------------------------------------------------------------------------

define([rule_cellToFace],
[rule_driver_unary($1, $2, CELL_TO_FACE, cellToFace)])

define([rule_cellToPoint],
[rule_driver_unary($1, $2, CELL_TO_POINT, cellToPoint)])

define([rule_pointToCell],
[rule_driver_unary($1, $2, POINT_TO_CELL, pointToCell)])


#------------------------------------------------------------------------------
# Standard rules for fields: declaration, new/get, driver functions etc.

include([m4/lemon/rules-fields.m4])dnl
divert(-1)dnl


#------------------------------------------------------------------------------

# Additional safety measures

undefine([substr])dnl   # Avoid collision with C/C++ naming

#------------------------------------------------------------------------------
divert(0)dnl
