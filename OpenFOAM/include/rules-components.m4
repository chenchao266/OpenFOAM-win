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
#     Collection of VectorSpace `component' functions
#
#     `rule_scalar_components'
#     `rule_vector_components'
#     `rule_sphTensor_components'
#     `rule_symTensor_components'
#     `rule_tensor_components'
#     `rule_tensor_unzipAll'  (diag and rows)
#
# Defined after inclusion
#     `rule_method_component'
#     `rule_tensor_unzipDiag'
#     `rule_tensor_unzipRow'
#
#------------------------------------------------------------------------------

# These are to be defined *after* inclusion - undefine now

undefine([rule_method_component])
undefine([rule_tensor_unzipDiag])
undefine([rule_tensor_unzipRow])


#------------------------------------------------------------------------------
# rule_scalar_components(source)
#
# Description
#     Extract scalar field from scalar field - no-op
#------------------------------------------------------------------------------

define([rule_scalar_components], [])


#------------------------------------------------------------------------------
# rule_vector_components(out, in)
#
# Description
#     Extract scalar field from vector field
#------------------------------------------------------------------------------

define([rule_vector_components],

 namespace Foam{
[rule_method_component($1, $2, CMPT_X, vector::X)
rule_method_component($1, $2, CMPT_Y, vector::Y)
rule_method_component($1, $2, CMPT_Z, vector::Z)]
)


#------------------------------------------------------------------------------
# rule_sphTensor_components(out, in)
#
# Description
#     Extract scalar field from sphericalTensor field
#------------------------------------------------------------------------------

define([rule_sphTensor_components],
[rule_method_component($1, $2, CMPT_II, sphericalTensor::II)]
)


#------------------------------------------------------------------------------
# rule_symTensor_components(out, in)
#
# Description
#     Extract scalar field from symmTensor field
#------------------------------------------------------------------------------

define([rule_symTensor_components],
[rule_method_component($1, $2, CMPT_XX, symmTensor::XX)
rule_method_component($1, $2, CMPT_XY, symmTensor::XY)
rule_method_component($1, $2, CMPT_XZ, symmTensor::XZ)
rule_method_component($1, $2, CMPT_YY, symmTensor::YY)
rule_method_component($1, $2, CMPT_YZ, symmTensor::YZ)
rule_method_component($1, $2, CMPT_ZZ, symmTensor::ZZ)]
)

#------------------------------------------------------------------------------
# rule_tensor_components(out, in)
#
# Description
#     Extract scalar field from tensor field
#------------------------------------------------------------------------------

define([rule_tensor_components],
[rule_method_component($1, $2, CMPT_XX, tensor::XX)
rule_method_component($1, $2, CMPT_XY, tensor::XY)
rule_method_component($1, $2, CMPT_XZ, tensor::XZ)
rule_method_component($1, $2, CMPT_YX, tensor::YX)
rule_method_component($1, $2, CMPT_YY, tensor::YY)
rule_method_component($1, $2, CMPT_YZ, tensor::YZ)
rule_method_component($1, $2, CMPT_ZX, tensor::ZX)
rule_method_component($1, $2, CMPT_ZY, tensor::ZY)
rule_method_component($1, $2, CMPT_ZZ, tensor::ZZ)]
)


#------------------------------------------------------------------------------
# rule_tensor_unzipAll(out, in)
#
# Description
#     Extract vector diagonal and rows from tensor field
#------------------------------------------------------------------------------

define([rule_tensor_unzipAll],
[rule_tensor_unzipDiag($1, $2)
rule_tensor_unzipRow($1, $2, CMPT_X, vector::X)
rule_tensor_unzipRow($1, $2, CMPT_Y, vector::Y)
rule_tensor_unzipRow($1, $2, CMPT_Z, vector::Z)]
)


#------------------------------------------------------------------------------
divert(0)dnl

 } // End namespace Foam
