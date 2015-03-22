/*
opengl/GLTexture.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    See GLTexture.hpp for more information.
*/




//Includes
#include "GLSampler.hpp"    //Header file

#include <brimstone/graphics/Enums.hpp> //FilterType

#include <gll/gl_4_4_comp.hpp>      //gll::* (GL 4.4 and below + compatibility)
using namespace gll;



namespace Brimstone {
namespace Private {

const int FilterTypeToGLFilterType[] {
    GL_NEAREST,     //NEAREST
    GL_LINEAR,      //LINEAR
};

const int WrapTypeToGLWrapType[] {
    GL_REPEAT,                  //REPEAT,
    GL_MIRRORED_REPEAT,         //MIRRORED_REPEAT,
    GL_CLAMP_TO_EDGE,           //CLAMP,
    GL_MIRROR_CLAMP_TO_EDGE     //MIRRORED_CLAMP
};

GLSampler::GLSampler() :
    m_name( 0 ) {
    create();
}

GLSampler::~GLSampler() {
    destroy();
}

void GLSampler::create() {

    glGenSamplers( 1, &m_name );

    //Controls how textures appear at anything but a 1:1 ratio when rendered on the screen
    //GL_TEXTURE_MIN_FILTER controls how the texture is sampled when minified (rendered smaller than the texture itself).
    //GL_TEXTURE_MAG_FILTER controls how the texture is sampled when magnified (rendered larger than the texture itself).
    //GL_NEAREST makes textures look pixelated ("nearest neighbor" approach).
    //GL_LINEAR uses linear interpolation, which looks blurry when magnified
    glSamplerParameteri( m_name, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glSamplerParameteri( m_name, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

    //UV coordinates are called S and T by OpenGL
    //Wrap values can be GL_CLAMP_TO_EDGE, GL_MIRRORED_REPEAT, GL_REPEAT, or GL_MIRROR_CLAMP_TO_EDGE
    glSamplerParameteri( m_name, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glSamplerParameteri( m_name, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
}

void GLSampler::destroy() {
    if( m_name != 0 ) {
        glDeleteSamplers( 1, &m_name );
        m_name = 0;
    }
}

void GLSampler::bind() {
    glBindSampler( 0, m_name );
}

void GLSampler::unbind() {
    glBindSampler( 0, 0 );
}

void GLSampler::setMinFilter( const FilterType type ) {
    glSamplerParameteri( m_name, GL_TEXTURE_MIN_FILTER, FilterTypeToGLFilterType[ (int)type ] );
}

void GLSampler::setMagFilter( const FilterType type ) {
    glSamplerParameteri( m_name, GL_TEXTURE_MAG_FILTER, FilterTypeToGLFilterType[ (int)type ] );
}

void GLSampler::setUWrap( const WrapType type ) {
    glSamplerParameteri( m_name, GL_TEXTURE_WRAP_S, WrapTypeToGLWrapType[ (int)type ] );
}

void GLSampler::setVWrap( const WrapType type ) {
    glSamplerParameteri( m_name, GL_TEXTURE_WRAP_T, WrapTypeToGLWrapType[ (int)type ] );
}

}
}