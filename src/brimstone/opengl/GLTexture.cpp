/*
opengl/GLTexture.cpp
--------------------
Copyright (c) 2024, theJ89

Description:
    See GLTexture.hpp for more information.
*/




//Includes
#include "GLTexture.hpp"    //Header file

#include <gll/gl_4_4_comp.hpp>      //gll::* (GL 4.4 and below + compatibility)
using namespace gll;




namespace Brimstone {
namespace Private {

GLTexture::GLTexture() :
    m_name( 0 ) {
    create();
}

GLTexture::~GLTexture() {
    destroy();
}

void GLTexture::create() {
    glGenTextures( 1, &m_name );

    //Turn off mipmaps
    glBindTexture( GL_TEXTURE_2D, m_name );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0 );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL,  0 );
    glBindTexture( GL_TEXTURE_2D, 0 );
}

void GLTexture::destroy() {
    if( m_name != 0 ) {
        glDeleteTextures( 1, &m_name );
        m_name = 0;
    }
}

void GLTexture::set( const size_t width, const size_t height, const void* data ) {
    m_width  = width;
    m_height = height;
    glBindTexture( GL_TEXTURE_2D, m_name );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
    glBindTexture( GL_TEXTURE_2D, 0 );
}

void GLTexture::bind() {
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, m_name );
}

void GLTexture::unbind() {
    glBindTexture( GL_TEXTURE_2D, 0 );
}

size_t GLTexture::getWidth() const {
    return m_width;
}

size_t GLTexture::getHeight() const {
    return m_height;
}

}
}
