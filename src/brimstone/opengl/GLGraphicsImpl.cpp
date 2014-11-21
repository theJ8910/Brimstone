/*
opengl/GLGraphicsImpl.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    See GLGraphicsImpl.hpp for more information.
*/




//Includes
#include "GLGraphicsImpl.hpp"   //Header file
#include "GLLoad.hpp"           //OpenGL extensions
#include "GLShader.hpp"         //Brimstone::GLShader
#include "GLProgram.hpp"        //Brimstone::GLProgram
#include "GLVertexBuffer.hpp"   //Brimstone::GLVertexBuffer
#include "GLTexture.hpp"        //Brimstone::GLTexture
#include "GLSampler.hpp"        //Brimstone::GLSampler

#include <brimstone/Logger.hpp> //Brimstone::logInfo

#include <boost/format.hpp>     //boost::format



namespace Brimstone {
namespace Private {

//Static initializers
std::atomic<bool> GLGraphicsImpl::m_initialized = false;

void GLGraphicsImpl::init( const Brimstone::Window& window ) {
    m_context.init( window );

    //After the first context is created, we can load OpenGL extensions, among other things.
    initOpenGL( m_context );
}

void GLGraphicsImpl::destroy() {
    m_context.destroy();
}

void GLGraphicsImpl::begin() {
    m_context.begin();
}

void GLGraphicsImpl::end() {
    m_context.end();
}

GLShader* GLGraphicsImpl::createShader( const ShaderType type ) {
    //TEMP: heap allocation
    return new GLShader( type );
}

GLProgram* GLGraphicsImpl::createProgram() {
    //TEMP: heap allocation
    return new GLProgram();
}

GLVertexBuffer* GLGraphicsImpl::createVertexBuffer() {
    //TEMP: heap allocation
    return new GLVertexBuffer();
}

GLTexture* GLGraphicsImpl::createTexture() {
    //TEMP: heap allocation
    return new GLTexture();
}

GLSampler* GLGraphicsImpl::createSampler() {
    //TEMP: heap allocation
    return new GLSampler();
}

void GLGraphicsImpl::clear( const float r, const float g, const float b, const float a, const float depth ) {
    glClearColor( r, g, b, a );
    glClearDepth( depth );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void GLGraphicsImpl::setViewport( const int x, const int y, const int width, const int height ) {
    glViewport( x, y, width, height );
}

void GLGraphicsImpl::flush() {
    glFlush();
}

void GLGraphicsImpl::enableBackFaceCulling() {
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
}

void GLGraphicsImpl::disableBackFaceCulling() {
    glDisable( GL_CULL_FACE );
}

void GLGraphicsImpl::enableScissor() {
    glEnable( GL_SCISSOR_TEST );
}

void GLGraphicsImpl::disableScissor() {
    glDisable( GL_SCISSOR_TEST );
}

void GLGraphicsImpl::enableBlend() {
    glEnable( GL_BLEND );

    //TEMP: enabling blend sets the blending mode to transparency.
    //NOTE: transparency is linear interpolation between d_rgb and s_rgb,
    //with s_a as the parameter:
    //  d_rgb + s_a*(s_rgb-d_rgb)     =
    //s_a*s_rgb + 1*d_rgb - s_a*d_rgb =
    //s_a*s_rgb + (1 - s_a) * d_rgb
    glBlendEquationSeparate( GL_ADD, GL_ADD );
    glBlendFuncSeparate( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE );
}

void GLGraphicsImpl::disableBlend() {
    glDisable( GL_BLEND );
}

void GLGraphicsImpl::setScissor( const int x, const int y, const int width, const int height ) {
    //glScissor is relative to the lower-left corner of the viewport,
    //rather than the upper-left corner. We need to adjust the y-coordinate
    //to compensate for this difference.
    glScissor( x, m_viewport.getHeight() - y - height, width, height );
}

void GLGraphicsImpl::setVSync( const bool vsync ) {
    m_context.setVSync( vsync );
}

void GLGraphicsImpl::swapBuffers() {
    //Swapping buffers is a platform-specific operation,
    //so we delegate the task to the OpenGL context
    m_context.swapBuffers();
}

void GLGraphicsImpl::initOpenGL( GLContext& context ) {
    //Is this the first time we've called this function?
    //If m_initialized is "false", set it to "true" and continue.
    //Otherwise, stop here.
    bool expect = false;
    if( !m_initialized.compare_exchange_strong( expect, true ) )
        return;

    //Activate our context temporarily
    context.begin();

    //Now that a context is loaded we can load extensions
    loadOpenGLExtensions();

    //Get the version of OpenGL we're using and log it
    GLint major, minor;
    glGetIntegerv( GL_MAJOR_VERSION, &major );
    glGetIntegerv( GL_MINOR_VERSION, &minor );

    logInfo( ( boost::format( "OpenGL version is %d.%d." ) % major % minor ).str() );

    //Log supported extensions
    GLint extensions;
    glGetIntegerv( GL_NUM_EXTENSIONS, &extensions );
    logInfo( "Supported extensions:" );
    for( GLuint i = 0; i < (GLuint)extensions; ++i )
        logInfo( (
            boost::format( "%4d:    %s" ) %
            (i+1) %
            glGetStringi( GL_EXTENSIONS, i )
        ).str() );

    //Done using it
    context.end();
}

}
}