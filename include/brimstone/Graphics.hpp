/*
Graphics.hpp
------------
Copyright (c) 2024, theJ89

Description:
    Graphics and its associated classes are defined here.
    Graphics is a low-level class that acts as a wrapper around an underlying graphics API (OpenGL, Direct3D, etc).
    A Graphics object is the means by which a program renders graphics to a Window.
    It provides several methods for rendering
*/
#ifndef BS_GRAPHICS_HPP
#define BS_GRAPHICS_HPP




//Includes
#include <cstddef>                              //std::size_t

#include <brimstone/types.hpp>                  //Brimstone::ustring, Brimstone::uint
#include <brimstone/graphics/DGraphicsImpl.hpp> //Brimstone::Private::DGraphicsImpl, etc
#include <brimstone/graphics/Enums.hpp>         //Brimstone::AlphaFunc, Brimstone::ShaderType, Brimstone::FilterType, Brimstone::WrapType




namespace Brimstone {

//Forward declarations
class Window;
class Shader;
class Program;
class VertexBuffer;
class Texture;
class Sampler;

class Graphics {
public:
    Graphics();
    Graphics( Graphics& toCopy ) = delete;
    Graphics& operator =( Graphics& toCopy ) = delete;
    ~Graphics();

    void            init( const Brimstone::Window& window );
    void            destroy();

    void            begin();
    void            end();

    Shader          createShader( const ShaderType type );
    Program         createProgram();
    VertexBuffer    createVertexBuffer();
    Texture         createTexture();
    Sampler         createSampler();

    void            flush();

    void            enableBackFaceCulling();
    void            disableBackFaceCulling();
    void            setBackFaceCulling( const bool enabled );
    bool            getBackFaceCulling() const;

    void            enableDepthTest();
    void            disableDepthTest();
    void            setDepthTest( const bool enabled );
    bool            getDepthTest() const;
    void            setDepthMask( const bool enabled );
    bool            getDepthMask() const;

    void            enableScissorTest();
    void            disableScissorTest();
    void            setScissorTest( const bool enabled );
    bool            getScissorTest() const;
    void            setScissorBox( const int x, const int y, const int width, const int height );
    void            getScissorBox( int (&xywhOut)[4] ) const;

    void            enableAlphaTest();
    void            disableAlphaTest();
    void            setAlphaTest( const bool enabled );
    bool            getAlphaTest() const;
    void            setAlphaFunc( const AlphaFunc func, const float ref );
    AlphaFunc       getAlphaFunc() const;
    float           getAlphaRef() const;

    void            enableBlend();
    void            disableBlend();
    void            setBlend( const bool enabled );
    bool            getBlend() const;
    void            setBlendModeToTransparency();

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
    Private::GraphicsImpl* m_impl;
};

//Class definitions
class Shader {
friend class Graphics;
friend class Program;
public:
    Shader();
    Shader( const Shader& toCopy ) = delete;
    Shader& operator =( const Shader& toCopy ) = delete;
    Shader( Shader&& toMove );
    Shader& operator =( Shader&& toMove );
    ~Shader();

    void create();
    void destroy();
    void setSource( const ustring& source );
    void compile();
private:
    Shader( Private::ShaderImpl* impl );
private:
    Private::ShaderImpl* m_impl;
};

class Program {
friend class Graphics;
public:
    Program();
    Program( const Program& toCopy ) = delete;
    Program& operator =( const Program& toCopy ) = delete;
    Program( Program&& toMove );
    Program& operator =( Program&& toMove );
    ~Program();

    void create();
    void destroy();

    void attachShader( const Shader& shader );
    void detachShader( const Shader& shader );

    void link();
    void use();
    void stopUsing();

    void setUniform( const char* const name, const int value );
    void setUniform( const char* const name, const uint value );
    void setUniform( const char* const name, const float value );
    void setUniform( const char* const name, const int x, const int y );
    void setUniform( const char* const name, const float x, const float y );
    void setUniform( const char* const name, const int x, const int y, const int z, const int w );
    void setUniform( const char* const name, const float x, const float y, const float z, const float w );
private:
    Program( Private::ProgramImpl* impl );
private:
    Private::ProgramImpl* m_impl;
};

class VertexBuffer {
friend class Graphics;
public:
    VertexBuffer();
    VertexBuffer( const VertexBuffer& toCopy ) = delete;
    VertexBuffer& operator =( const VertexBuffer& toCopy ) = delete;
    VertexBuffer( VertexBuffer&& toMove );
    VertexBuffer& operator =( VertexBuffer&& toMove );
    ~VertexBuffer();

    void create();
    void destroy();

    void set( const float* const data, const std::size_t sizeInBytes );
    void set( const float* const data, const std::size_t offsetInBytes, const std::size_t sizeInBytes );
    void bind();
    void unbind();
    void draw();

    void setType( const int type );
private:
    VertexBuffer( Private::VertexBufferImpl* impl );
private:
    Private::VertexBufferImpl* m_impl;
};

class Texture {
friend class Graphics;
public:
    Texture();
    Texture( const Texture& toCopy ) = delete;
    Texture& operator =( const Texture& toCopy ) = delete;
    Texture( Texture&& toMove );
    Texture& operator =( Texture&& toMove );
    ~Texture();

    void create();
    void destroy();

    void set( const std::size_t width, const std::size_t height, const void* data );

    void bind();
    void unbind();

    std::size_t getWidth() const;
    std::size_t getHeight() const;
private:
    Texture( Private::TextureImpl* impl );
private:
    Private::TextureImpl* m_impl;
};

class Sampler {
friend class Graphics;
public:
    Sampler();
    Sampler( Private::SamplerImpl* impl );
    Sampler( const Sampler& toCopy ) = delete;
    Sampler& operator =( const Sampler& toCopy ) = delete;
    Sampler( Sampler&& toMove );
    Sampler& operator =( Sampler&& toMove );
    ~Sampler();

    void create();
    void destroy();

    void setMinFilter( const FilterType type );
    void setMagFilter( const FilterType type );
    void setUWrap( const WrapType type );
    void setVWrap( const WrapType type );

    void bind();
    void unbind();
private:
    Private::SamplerImpl* m_impl;
};

}

#endif //BS_GRAPHICS_HPP
