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
#     This file is part of OpenFOAM, distributed under GPL-3.0-or-later.
#
# Description
#     Collection of `standard' functions and type-specific ones.
#
#     `rules_inplace_unary'
#     `rules_scalar_functions'
#     `rules_vector_functions'
#     `rules_sphTensor_functions'
#     `rules_symTensor_functions'
#     `rules_tensor_functions'
#
# Defined after inclusion
#     `rule_unary_func'         `incomplete_rule_unary_func'
#     `rule_binary_func'
#     `rule_inplace_unary'
#
#------------------------------------------------------------------------------

# These are to be defined *after* inclusion - undefine now

undefine([rule_unary_func])
undefine([rule_binary_func])
undefine([rule_inplace_unary])
undefine([incomplete_rule_unary_func])


#------------------------------------------------------------------------------
# rules_inplace_unary(inOut)
#
# Description
#     Production rules for standard unary field reductions.
#
#     Calls [rule_inplace_unary()] with
#     (MIN, min), (MAX, max), (SUM, sum), (AVERAGE, average)
#
# Example
#     rules_inplace_unary(sfield)
#------------------------------------------------------------------------------

define([rules_inplace_unary],

 namespace Foam{
[rule_inplace_unary($1, MIN, min)
rule_inplace_unary($1, MAX, max)
rule_inplace_unary($1, SUM, sum)
rule_inplace_unary($1, AVERAGE, average)]
)


#------------------------------------------------------------------------------
# rules_inplace_gUnary(inOut)
#
# Description
#     Production rules for standard global unary field reductions.
#
#     Calls [rule_inplace_unary()] with
#     (MIN, gMin), (MAX, gMax), (SUM, gSum), (AVERAGE, gAverage)
#
# Example
#     rules_inplace_gUnary(sfield)
#------------------------------------------------------------------------------

define([rules_inplace_gUnary],
[rule_inplace_unary($1, MIN, gMin)
rule_inplace_unary($1, MAX, gMax)
rule_inplace_unary($1, SUM, gSum)
rule_inplace_unary($1, AVERAGE, gAverage)]
)


#------------------------------------------------------------------------------
# Functions - magnitude
#------------------------------------------------------------------------------

define([rules_mag_functions],
[dnl
rule_unary_func($1, $2, MAG, mag)
rule_unary_func($1, $2, MAGSQR, magSqr)
])


#------------------------------------------------------------------------------
# Functions - scalar
#------------------------------------------------------------------------------

define([rules_scalar_functions],
[dnl
rule_unary_func(_scalar_, _scalar_, EXP, exp)
rule_unary_func(_scalar_, _scalar_, LOG, log)
rule_unary_func(_scalar_, _scalar_, LOG10, log10)
rule_unary_func(_scalar_, _scalar_, SQR, sqr)
rule_unary_func(_scalar_, _scalar_, SQRT, sqrt)
rule_unary_func(_scalar_, _scalar_, CBRT, cbrt)
rule_unary_func(_scalar_, _scalar_, SIN, sin)
rule_unary_func(_scalar_, _scalar_, COS, cos)
rule_unary_func(_scalar_, _scalar_, TAN, tan)
rule_unary_func(_scalar_, _scalar_, ASIN, asin)
rule_unary_func(_scalar_, _scalar_, ACOS, acos)
rule_unary_func(_scalar_, _scalar_, ATAN, atan)
rule_unary_func(_scalar_, _scalar_, SINH, sinh)
rule_unary_func(_scalar_, _scalar_, COSH, cosh)
rule_unary_func(_scalar_, _scalar_, TANH, tanh)
rule_binary_func(_scalar_, _scalar_, _scalar_, POW, pow)
rule_binary_func(_scalar_, _scalar_, _scalar_, ATAN2, atan2)
dnl
incomplete_rule_unary_func(_scalar_, _scalar_, POS, pos)
incomplete_rule_unary_func(_scalar_, _scalar_, NEG, neg)
incomplete_rule_unary_func(_scalar_, _scalar_, POS0, pos0)
incomplete_rule_unary_func(_scalar_, _scalar_, NEG0, neg0)
incomplete_rule_unary_func(_scalar_, _scalar_, SIGN, sign)
])

#------------------------------------------------------------------------------
# Functions - vector
#------------------------------------------------------------------------------

define([rules_vector_functions], [])


#------------------------------------------------------------------------------
# Functions - sphericalTensor
#------------------------------------------------------------------------------

define([rules_sphTensor_functions], [])


#------------------------------------------------------------------------------
# Functions - symmTensor
#------------------------------------------------------------------------------

define([rules_symTensor_functions],[])


#------------------------------------------------------------------------------
# Functions - tensor
#------------------------------------------------------------------------------

define([rules_tensor_functions], [])


#------------------------------------------------------------------------------
divert(0)dnl

 } // End namespace Foam
