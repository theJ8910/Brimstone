/*
opengl/GLProgram.hpp
--------------------
Copyright (c) 2024, theJ89

Description:
    GLProgram is defined here.
    These objects manage shader programs.
*/
#ifndef BS_OPENGL_GLPROGRAM_HPP
#define BS_OPENGL_GLPROGRAM_HPP




//Includes
#include <gll/gl_types.hpp>  //gll:GLchar




namespace Brimstone::Private {




//Forward declarations
class GLShader;




class GLProgram {
public:
    GLProgram();
    GLProgram( GLProgram& toCopy ) = delete;
    GLProgram& operator =( GLProgram& toCopy ) = delete;
    ~GLProgram();

    void create();
    void destroy();

    void attachShader( const GLShader& shader );
    void detachShader( const GLShader& shader );

    void link();
    void use();
    void stopUsing();

    void setUniform( const char* const name, const int value );
    void setUniform( const char* const name, const unsigned int value );
    void setUniform( const char* const name, const float value );
    void setUniform( const char* const name, const int x, const int y );
    void setUniform( const char* const name, const float x, const float y );
    void setUniform( const char* const name, const int x, const int y, const int z, const int w );
    void setUniform( const char* const name, const float x, const float y, const float z, const float w );
private:
    gll::GLuint m_name;
};




} //namespace Brimstone::Private




#endif //BS_OPENGL_GLPROGRAM_HPP
