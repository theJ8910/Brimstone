/*
graphics/Enums.cpp
------------------------
Copyright (c) 2024, theJ89

Description:
    See Enums.hpp for more information.
*/




//Includes
#include <brimstone/graphics/Enums.hpp>  //Header

#include <cstddef>                       //std::size_t




namespace {




constexpr const char* alphaFuncToStringMap [] = {
    "NEVER",                  //<= NEVER
    "LESS_THAN",              //<= LESS_THAN
    "EQUAL",                  //<= EQUAL
    "LESS_THAN_OR_EQUAL",     //<= LESS_THAN_OR_EQUAL
    "GREATER_THAN",           //<= GREATER_THAN
    "NOT_EQUAL",              //<= NOT_EQUAL
    "GREATER_THAN_OR_EQUAL",  //<= GREATER_THAN_OR_EQUAL
    "ALWAYS"                  //<= ALWAYS
};




} //namespace




namespace Brimstone {




const char* alphaFuncToString( const AlphaFunc func ) {
    return alphaFuncToStringMap[ (std::size_t)func ];
}




} //namespace Brimstone
