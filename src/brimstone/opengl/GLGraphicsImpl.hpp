/*
opengl/GLGraphicsImpl.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    A Graphics implementation utilizing OpenGL
*/

#ifndef BS_OPENGL_GLGRAPHICSIMPL_HPP
#define BS_OPENGL_GLGRAPHICSIMPL_HPP




//Includes
#include <brimstone/graphics/Enums.hpp> //Brimstone::ShaderType
#include <brimstone/Bounds.hpp>         //Brimstone::Bounds2i

#include "GLContext.hpp"                //Brimstone::Private::GLContext

#include <atomic>                       //std::mutex






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

    void            enableBackFaceCulling();
    void            disableBackFaceCulling();

    void            enableScissor();
    void            disableScissor();
    void            setScissor( const int x, const int y, const int width, const int height );

    void            enableBlend();
    void            disableBlend();

    void            clear( const float r, const float g, const float b, const float a, const float depth );
    void            setViewport( const int x, const int y, const int width, const int height );

    void            setVSync( const bool vsync );
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