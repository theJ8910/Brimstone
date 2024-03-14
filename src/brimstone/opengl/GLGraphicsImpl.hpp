/*
opengl/GLGraphicsImpl.hpp
-------------------------
Copyright (c) 2024, theJ89

Description:
    A Graphics implementation utilizing OpenGL
*/

#ifndef BS_OPENGL_GLGRAPHICSIMPL_HPP
#define BS_OPENGL_GLGRAPHICSIMPL_HPP




//Includes
#include <brimstone/graphics/Enums.hpp> //Brimstone::ShaderType
#include <brimstone/Bounds.hpp>         //Brimstone::Bounds2i

#include "GLContext.hpp"                //Brimstone::Private::GLContext

#include <atomic>                       //std::atomic






namespace Brimstone {
//Forward declarations
class Window;

namespace Private {

//Forward declarations
class GLShader;
class GLProgram;
class GLVertexBuffer;
class GLTexture;
class GLSampler;

class GLGraphicsImpl {
public:
    void            init( const Brimstone::Window& window );
    void            destroy();

    void            begin();
    void            end();

    GLShader*       createShader( const ShaderType type );
    GLProgram*      createProgram();
    GLVertexBuffer* createVertexBuffer();
    GLTexture*      createTexture();
    GLSampler*      createSampler();

    void            flush();

    void            enableDepthTest();
    void            disableDepthTest();
    void            setDepthTest( const bool enabled );
    bool            getDepthTest() const;

    void            enableBackFaceCulling();
    void            disableBackFaceCulling();
    void            setBackFaceCulling( const bool enabled );
    bool            getBackFaceCulling() const;

    void            enableScissor();
    void            disableScissor();
    void            setScissor( const bool enabled );
    bool            getScissor() const;
    void            setScissorBox( const int x, const int y, const int width, const int height );
    void            getScissorBox( int (&xywhOut)[4] ) const;

    void            enableBlend();
    void            disableBlend();
    void            setBlend( const bool enabled );
    bool            getBlend() const;

    void            setClearColor( const float r, const float g, const float b, const float a );
    void            getClearColor( float (&rgbaOut)[4] ) const;
    void            setClearDepth( const float depth );
    void            setClearDepth( const double depth );
    double          getClearDepth() const;
    void            clear();

    void            setViewport( const int x, const int y, const int width, const int height );
    void            getViewport( int (&xywhOut)[4] ) const;

    void            setVSync( const bool enabled );
    bool            getVSync() const;

    void            swapBuffers();
private:
    //Context used by this object
    GLContext   m_context;

    //Current viewport info
    Bounds2i    m_viewport;

    //Current projection-view-world matrix
    //Matrix4x4f  m_pvw;

    //Current scissor bounds
    //Bounds2i    m_scissor;
public:
    static void initOpenGL( GLContext& context );
private:
    static std::atomic<bool>    m_initialized;
};

}
}




#endif //BS_OPENGL_GLGRAPHICS_HPP
