/*
Matrix.hpp
----------
Copyright (c) 2024, theJ89

Description:
    Include this file for access to MatrixN and its variations.
    Note: Since this file merely includes other files, there are no include guards necessary here.
*/




//Includes
#include <brimstone/matrix/MatrixRxC.hpp>
#include <brimstone/matrix/MatrixNxN.hpp>
#include <brimstone/matrix/Matrix2x2.hpp>
#include <brimstone/matrix/Matrix3x3.hpp>
#include <brimstone/matrix/Matrix4x4.hpp>

//These macros aren't needed outside of the above files
#undef BS_MATRIX_DECLARE_METHODS
#undef BS_MATRIX_DEFINE_METHODS
#undef BS_MATRIX_SQUARE_DECLARE_METHODS
#undef BS_MATRIX_SQUARE_DEFINE_METHODS
