/*
Graphics.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Graphics and its associated classes are defined here.
    Graphics is a low-level class that acts as a wrapper around an underlying graphics API (OpenGL, Direct3D, etc).
    A Graphics object is the means by which a program renders graphics to a Window.
    It provides several methods for rendering
*/
#ifndef BS_GRAPHICS_HPP
#define BS_GRAPHICS_HPP




//Includes
#include <brimstone/graphics/DGraphicsImpl.hpp> //Brimstone::Private::DGraphicsImpl, etc
#include <brimstone/types.hpp>                  //Brimstone::ustring, Brimstone::uint
#include <brimstone/graphics/Enums.hpp>         //Brimstone::FilterType, Brimstone::WrapType




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

    void set( const float* const data, const size_t sizeInBytes );
    void set( const float* const data, const size_t offsetInBytes, const size_t sizeInBytes );
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

    void set( const size_t width, const size_t height, const void* data );

    void bind();
    void unbind();

    size_t getWidth() const;
    size_t getHeight() const;
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