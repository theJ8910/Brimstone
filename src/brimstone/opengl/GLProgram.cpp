/*
opengl/GLProgram.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    See GLProgram.hpp for more information.
*/




//Includes
#include <brimstone/Exception.hpp>  //GraphicsException
#include "GLProgram.hpp"            //Class header
#include "GLShader.hpp"             //GLShader
#include "GLLoad.hpp"               //OpenGL Extensions

#include <memory>                   //std::unique_ptr


namespace Brimstone {
namespace Private {

GLProgram::GLProgram() :
    m_name( 0 ) {
    create();
}
GLProgram::~GLProgram() {
    destroy();
}

void GLProgram::create() {
    m_name = glCreateProgram();
}

void GLProgram::destroy() {
    if( m_name != 0 ) {
        glDeleteProgram( m_name );
        m_name = 0;
    }
}

void GLProgram::attachShader( const GLShader& shader ) {
    glAttachShader( m_name, shader.m_name );
}

void GLProgram::detachShader( const GLShader& shader ) {
    glDetachShader( m_name, shader.m_name );
}

void GLProgram::link() {
    glLinkProgram( m_name );

    //Check if the shader program linked successfully or not
    GLint status = GL_FALSE;
    glGetProgramiv( m_name, GL_LINK_STATUS, &status );
    if( status == GL_TRUE )
        return;

    //If not, retrieve the info log and throw it as an exception.
    GLsizei size = 0;
    glGetProgramiv( m_name, GL_INFO_LOG_LENGTH, &size );
    if( size == 0 )
        throw GraphicsException();

    std::unique_ptr< GLchar[] > log( new GLchar[ size ] );
    glGetProgramInfoLog( m_name, size, nullptr, log.get() );

    throw GraphicsException( log.get() );
}

void GLProgram::use() {
    glUseProgram( m_name );
}

void GLProgram::stopUsing() {
    glUseProgram( 0 );
}

void GLProgram::setUniform( const GLchar* const name, const GLint value ) {
    GLint loc = glGetUniformLocation( m_name, name );
    glUniform1i( loc, value );
}

void GLProgram::setUniform( const GLchar* const name, const GLuint value ) {
    GLint loc = glGetUniformLocation( m_name, name );
    glUniform1ui( loc, value );
}

void GLProgram::setUniform( const GLchar* const name, const GLfloat value ) {
    GLint loc = glGetUniformLocation( m_name, name );
    glUniform1f( loc, value );
}

void GLProgram::setUniform( const GLchar* const name, const GLint x, const GLint y ) {
    GLint loc = glGetUniformLocation( m_name, name );
    glUniform2i( loc, x, y );
}

void GLProgram::setUniform( const GLchar* const name, const GLfloat x, const GLfloat y ) {
    GLint loc = glGetUniformLocation( m_name, name );
    glUniform2f( loc, x, y );
}

void GLProgram::setUniform( const GLchar* const name, const GLint x, const GLint y, const GLint z, const GLint w ) {
    GLint loc = glGetUniformLocation( m_name, name );
    glUniform4i( loc, x, y, z, w );
}

void GLProgram::setUniform( const GLchar* const name, const GLfloat x, const GLfloat y, const GLfloat z, const GLfloat w ) {
    GLint loc = glGetUniformLocation( m_name, name );
    glUniform4f( loc, x, y, z, w );
}

}
}