/*
graphics/Enums.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Enumerations used by the graphics classes are defined here.
*/
#ifndef BS_GRAPHICS_ENUMS_HPP
#define BS_GRAPHICS_ENUMS_HPP


namespace Brimstone {

//A ShaderType is passed to a function that creates a shader
//to indicate what kind of shader is desired.
enum class ShaderType {
    VERTEX,
    GEOMETRY,
    FRAGMENT
};

//A FilterType specifies what type of min / mag filter a Sampler should use.
enum class FilterType {
    NEAREST,
    LINEAR
};

//A WrapType specifies what type of UV wrapping a sampler should use.
enum class WrapType {
    REPEAT,
    MIRRORED_REPEAT,
    CLAMP,
    MIRRORED_CLAMP
};

}




#endif //BS_GRAPHICS_ENUMS_HPP