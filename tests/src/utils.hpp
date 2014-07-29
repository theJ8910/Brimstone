/*
utils.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Shared utilities that unit tests use.
*/
#ifndef BS_UT_UTILS_HPP
#define BS_UT_UTILS_HPP




namespace Brimstone {
namespace UnitTest {

//Constants
extern const float FAST_SQRT_ERROR;

//Forward declarations
bool isWithin( const float value, const float ideal, const float err );
bool allWithin( const float* values, const float* ideals, const float err, const int size );

}
}




#endif //BS_UT_UTILS_HPP