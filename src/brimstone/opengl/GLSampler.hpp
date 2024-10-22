/*
opengl/GLSampler.hpp
--------------------
Copyright (c) 2024, theJ89

Description:
    GLSampler is defined here.
    These objects wrap OpenGL samplers.
*/
#ifndef BS_OPENGL_GLSAMPLER_HPP
#define BS_OPENGL_GLSAMPLER_HPP




//Includes
#include <gll/gl_types.hpp>  //gll::GLuint




namespace Brimstone {




//Forward declarations
enum class FilterType;
enum class WrapType;




} //namespace Brimstone




namespace Brimstone::Private {




class GLSampler {
public:
    GLSampler();
    ~GLSampler();

    void create();
    void destroy();

    void setMinFilter( const FilterType type );
    void setMagFilter( const FilterType type );
    void setUWrap( const WrapType type );
    void setVWrap( const WrapType type );

    void bind();
    void unbind();
private:
    gll::GLuint m_name;
};




} //namespace Brimstone::Private




#endif //BS_OPENGL_GLSAMPLER_HPP
