/*
utils.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    See utils.hpp for more information.
*/




//Includes
#include "utils.hpp"    //Header




namespace UnitTest {

//Constants
const float FAST_SQRT_ERROR = 0.0017f;

bool isWithin( const float value, const float ideal, const float err ) {
    return value >= ideal * ( 1 - err ) &&
           value <= ideal * ( 1 + err ); 
}

bool allWithin( const float* values, const float* ideals, const float err, const int size ) {
    for( int i = 0; i < size; ++i )
        if( !isWithin( values[i], ideals[i], err ) )
            return false;
    return true;
}

}