/*
opengl/GLGraphicsImpl.cpp
-------------------------
Copyright (c) 2024, theJ89

Description:
    See GLGraphicsImpl.hpp for more information.
*/




//Includes
#include "GLGraphicsImpl.hpp"   //Header file
#include "GLShader.hpp"         //Brimstone::GLShader
#include "GLProgram.hpp"        //Brimstone::GLProgram
#include "GLVertexBuffer.hpp"   //Brimstone::GLVertexBuffer
#include "GLTexture.hpp"        //Brimstone::GLTexture
#include "GLSampler.hpp"        //Brimstone::GLSampler

#include <brimstone/Logger.hpp> //Brimstone::logInfo

#include <boost/format.hpp>     //boost::format

#include <gll/loader.hpp>       //gll::Load
#include <gll/gl_4_4_comp.hpp>  //gll::* (GL 4.4 and below + compatibility)
using namespace gll;

namespace Brimstone {
namespace Private {

//Static initializers
std::atomic<bool> GLGraphicsImpl::m_initialized( false );

void GLGraphicsImpl::init( const Brimstone::Window& window ) {
    m_context.init( window );

    //After the first context is created, we can load OpenGL extensions, among other things.
    initOpenGL( m_context );

    //Grab the viewport:
    begin();
    getViewport( m_viewport.data );
    end();
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

void GLGraphicsImpl::flush() {
    glFlush();
}

void GLGraphicsImpl::enableDepthTest() {
    glEnable( GL_DEPTH_TEST );
    if( glGetError() != GL_NO_ERROR )
        throw GraphicsException( "glEnable() failed." );
}

void GLGraphicsImpl::disableDepthTest() {
    glDisable( GL_DEPTH_TEST );
    if( glGetError() != GL_NO_ERROR )
        throw GraphicsException( "glDisable() failed." );
}

void GLGraphicsImpl::setDepthTest( const bool enabled ) {
    if( enabled ) {
        enableDepthTest();
    } else {
        disableDepthTest();
    }
}

bool GLGraphicsImpl::getDepthTest() const {
    GLboolean enabled;
    glGetBooleanv( GL_DEPTH_TEST, &enabled );
    if( glGetError() != GL_NO_ERROR )
        throw GraphicsException( "glGetBooleanv() failed." );
    return enabled == GL_TRUE;

}

void GLGraphicsImpl::enableBackFaceCulling() {
    //Initial value of GL_CULL_FACE_MODE is GL_BACK; no need to set it explicitly:
    //glCullFace( GL_BACK );
    glEnable( GL_CULL_FACE );
    if( glGetError() != GL_NO_ERROR )
        throw GraphicsException( "glEnable() failed." );
}

void GLGraphicsImpl::disableBackFaceCulling() {
    glDisable( GL_CULL_FACE );
    if( glGetError() != GL_NO_ERROR )
        throw GraphicsException( "glDisable() failed." );
}

void GLGraphicsImpl::setBackFaceCulling( const bool enabled ) {
    if( enabled ) {
        enableBackFaceCulling();
    } else {
        disableBackFaceCulling();
    }
}

bool GLGraphicsImpl::getBackFaceCulling() const {
    GLboolean enabled;
    glGetBooleanv( GL_CULL_FACE, &enabled );
    if( glGetError() != GL_NO_ERROR )
        throw GraphicsException( "glGetBooleanv() failed." );
    return enabled == GL_TRUE;
}

void GLGraphicsImpl::enableScissor() {
    glEnable( GL_SCISSOR_TEST );
    if( glGetError() != GL_NO_ERROR )
        throw GraphicsException( "glEnable() failed." );
}

void GLGraphicsImpl::disableScissor() {
    glDisable( GL_SCISSOR_TEST );
    if( glGetError() != GL_NO_ERROR )
        throw GraphicsException( "glDisable() failed." );
}

void GLGraphicsImpl::setScissor( const bool enabled ) {
    if( enabled ) {
        enableScissor();
    } else {
        disableScissor();
    }
}

bool GLGraphicsImpl::getScissor() const {
    GLboolean enabled;
    glGetBooleanv( GL_SCISSOR_TEST, &enabled );
    if( glGetError() != GL_NO_ERROR )
        throw GraphicsException( "glGetBooleanv() failed." );
    return enabled == GL_TRUE;
}

void GLGraphicsImpl::setScissorBox( const int x, const int y, const int width, const int height ) {
    //glScissor is relative to the lower-left corner of the viewport,
    //rather than the upper-left corner. We need to adjust the y-coordinate
    //to compensate for this difference.
    glScissor( x, m_viewport.getHeight() - y - height, width, height );
    if( glGetError() != GL_NO_ERROR )
        throw GraphicsException( "glScissor() failed." );
}

void GLGraphicsImpl::getScissorBox( int (&xywhOut)[4] ) const {
    glGetIntegerv( GL_SCISSOR_BOX, xywhOut );
    if( glGetError() != GL_NO_ERROR )
        throw GraphicsException( "glGetIntegerv() failed." );
    xywhOut[1] = m_viewport.getHeight() - xywhOut[1] - xywhOut[3];
}

void GLGraphicsImpl::enableBlend() {
    //TEMP: enabling blend sets the blending mode to transparency.
    //NOTE: transparency is linear interpolation between d_rgb and s_rgb,
    //with s_a as the parameter:
    //  d_rgb + s_a*(s_rgb-d_rgb)     =
    //s_a*s_rgb + 1*d_rgb - s_a*d_rgb =
    //s_a*s_rgb + (1 - s_a) * d_rgb
    glBlendEquationSeparate( GL_ADD, GL_ADD );
    if( glGetError() != GL_NO_ERROR )
        throw GraphicsException( "glBlendEquationSeparate() failed." );
    glBlendFuncSeparate( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE );
    if( glGetError() != GL_NO_ERROR )
        throw GraphicsException( "glBlendFuncSeparate() failed." );

    glEnable( GL_BLEND );
    if( glGetError() != GL_NO_ERROR )
        throw GraphicsException( "glEnable() failed." );
}

void GLGraphicsImpl::disableBlend() {
    glDisable( GL_BLEND );
    if( glGetError() != GL_NO_ERROR )
        throw GraphicsException( "glDisable() failed." );
}

void GLGraphicsImpl::setBlend( const bool enabled ) {
    if( enabled ) {
        enableBlend();
    } else {
        disableBlend();
    }
}

bool GLGraphicsImpl::getBlend() const {
    GLboolean enabled;
    glGetBooleanv( GL_BLEND, &enabled );
    if( glGetError() != GL_NO_ERROR )
        throw GraphicsException( "glGetBooleanv() failed." );
    return enabled == GL_TRUE;
}

void GLGraphicsImpl::setClearColor( const float r, const float g, const float b, const float a ) {
    glClearColor( r, g, b, a );
    if( glGetError() != GL_NO_ERROR )
        throw GraphicsException( "glClearColor() failed." );
}

void GLGraphicsImpl::getClearColor( float (&rgbaOut)[4] ) const {
    glGetFloatv( GL_COLOR_CLEAR_VALUE, rgbaOut );
    if( glGetError() != GL_NO_ERROR )
        throw GraphicsException( "glGetFloatv() failed." );
}

void GLGraphicsImpl::setClearDepth( const float depth ) {
    glClearDepthf( depth );
    if( glGetError() != GL_NO_ERROR )
        throw GraphicsException( "glClearDepthf() failed." );
}

void GLGraphicsImpl::setClearDepth( const double depth ) {
    glClearDepth( depth );
    if( glGetError() != GL_NO_ERROR )
        throw GraphicsException( "glClearDepth() failed." );
}

double GLGraphicsImpl::getClearDepth() const {
    double depth;
    glGetDoublev( GL_DEPTH_CLEAR_VALUE, &depth );
    if( glGetError() != GL_NO_ERROR )
        throw GraphicsException( "glGetDoublev() failed." );
    return depth;
}

void GLGraphicsImpl::clear() {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void GLGraphicsImpl::setViewport( const int x, const int y, const int width, const int height ) {
    //Note: glViewport is relative to the lower-left corner of the window,
    //rather than the upper-left corner. We need to adjust the y-coordinate
    //to compensate for this difference. TODO
    glViewport( x, y, width, height );
    if( glGetError() != GL_NO_ERROR )
        throw GraphicsException( "glViewport() failed." );
    m_viewport.set( x, y, width, height );
}

void GLGraphicsImpl::getViewport( int (&xywhOut)[4] ) const {
    //TODO: y-coordinate adjustment
    glGetIntegerv( GL_VIEWPORT, xywhOut );
    if( glGetError() != GL_NO_ERROR )
        throw GraphicsException( "glGetIntegerv() failed." );
}

void GLGraphicsImpl::setVSync( const bool enabled ) {
    m_context.setVSync( enabled );
}

bool GLGraphicsImpl::getVSync() const {
    return m_context.getVSync();
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
    gll::Load();

    //Get the version of OpenGL we're using and log it
    GLint major, minor;
    glGetIntegerv( GL_MAJOR_VERSION, &major );
    glGetIntegerv( GL_MINOR_VERSION, &minor );

    logInfo( ( boost::format( "OpenGL version is %d.%d." ) % major % minor ).str() );

    //Log supported extensions
    GLint extensions;
    glGetIntegerv( GL_NUM_EXTENSIONS, &extensions );
    logInfo( "Supported extensions:" );
    for( GLuint i = 0; i < (GLuint)extensions; ++i ) {
        logInfo( (
            boost::format( "%4d:    %s" ) %
            (i+1) %
            glGetStringi( GL_EXTENSIONS, i )
        ).str() );
    }

    //Done using it
    context.end();
}

}
}
