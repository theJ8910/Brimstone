/*
opengl/GLShader.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    See GLShader.hpp for more information.
*/




//Includes
#include <brimstone/types.hpp>              //Brimstone::ustring
#include <brimstone/graphics/Enums.hpp>     //ShaderType
#include <brimstone/Exception.hpp>
#include "GLShader.hpp"
#include "GLLoad.hpp"

#include <memory>




namespace Brimstone {
namespace Private {

const int ShaderTypeToGLShaderType[] {
    GL_VERTEX_SHADER,       //VERTEX
    GL_GEOMETRY_SHADER,     //GEOMETRY
    GL_FRAGMENT_SHADER      //FRAGMENT
};

GLShader::GLShader( const ShaderType type ) :
    m_type( type ), m_name( 0 ) {
    create();
}
GLShader::~GLShader() {
    destroy();
}

void GLShader::create() {
    m_name = glCreateShader( ShaderTypeToGLShaderType[ (int)m_type ] );
}

void GLShader::destroy() {
    if( m_name != 0 ) {
        glDeleteShader( m_name );
        m_name = 0;
    }
}

void GLShader::setSource( const ustring& source ) {
    const GLchar* p = source.c_str();
    glShaderSource( m_name, 1, &p, nullptr );
}

void GLShader::compile() {
    //Compile shader
    glCompileShader( m_name );

    //Check if the shader compiled successfully or not
    GLint status = GL_FALSE;
    glGetShaderiv( m_name, GL_COMPILE_STATUS, &status );
    if( status == GL_TRUE )
        return;

    //If not, retrieve the info log and throw it as an exception.
    GLsizei size = 0;
    glGetShaderiv( m_name, GL_INFO_LOG_LENGTH, &size );
    if( size == 0 )
        throw GraphicsException();

    std::unique_ptr< GLchar[] > log( new GLchar[ size ] );
    glGetShaderInfoLog( m_name, size, nullptr, log.get() );

    throw GraphicsException( log.get() );
}

}
}