/*
opengl/GLProgram.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    GLProgram is defined here.
    These objects manage shader programs.
*/
#ifndef BS_OPENGL_GLPROGRAM_HPP
#define BS_OPENGL_GLPROGRAM_HPP




//Includes
#include "GLHeader.hpp"                         //GLuint
#include "GLext.h"                              //GLchar




namespace Brimstone {
namespace Private {

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

    void setUniform( const GLchar* const name, const GLint value );
    void setUniform( const GLchar* const name, const GLuint value );
    void setUniform( const GLchar* const name, const GLfloat value );
    void setUniform( const GLchar* const name, const GLint x, const GLint y );
    void setUniform( const GLchar* const name, const GLfloat x, const GLfloat y );
    void setUniform( const GLchar* const name, const GLint x, const GLint y, const GLint z, const GLint w );
    void setUniform( const GLchar* const name, const GLfloat x, const GLfloat y, const GLfloat z, const GLfloat w );
private:
    GLuint     m_name;
};

}
}




#endif //BS_OPENGL_GLPROGRAM_HPP