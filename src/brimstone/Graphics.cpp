/*
Graphics.cpp
------------
Copyright (c) 2024, theJ89

Description:
    See Graphics.hpp for more information.
*/
//Includes
#include <brimstone/Graphics.hpp>       //Header file

#include "graphics/GraphicsImpl.hpp"    //Brimstone::Private::GraphicsImpl, etc




namespace Brimstone {

Graphics::Graphics() :
    m_impl( nullptr ) {
    m_impl = new Private::GraphicsImpl();
}

Graphics::~Graphics() {
    if( m_impl != nullptr )
        delete m_impl;
}

void Graphics::init( const Brimstone::Window& window ) {
    m_impl->init( window );
}

void Graphics::destroy() {
    m_impl->destroy();
}

void Graphics::begin() {
    m_impl->begin();
}

void Graphics::end() {
    m_impl->end();
}

Shader Graphics::createShader( const ShaderType type ) {
    return Shader( m_impl->createShader( type ) );
}

Program Graphics::createProgram() {
    return Program( m_impl->createProgram() );
}

VertexBuffer Graphics::createVertexBuffer() {
    return VertexBuffer( m_impl->createVertexBuffer() );
}

Texture Graphics::createTexture() {
    return Texture( m_impl->createTexture() );
}

Sampler Graphics::createSampler() {
    return Sampler( m_impl->createSampler() );
}

void Graphics::flush() {
    m_impl->flush();
}

void Graphics::enableBackFaceCulling() {
    m_impl->enableBackFaceCulling();
}

void Graphics::disableBackFaceCulling() {
    m_impl->disableBackFaceCulling();
}

void Graphics::enableScissor() {
    m_impl->enableScissor();
}

void Graphics::disableScissor() {
    m_impl->disableScissor();
}

void Graphics::setScissor( const int x, const int y, const int width, const int height ) {
    m_impl->setScissor( x, y, width, height );
}

void Graphics::enableBlend() {
    m_impl->enableBlend();
}

void Graphics::disableBlend() {
    m_impl->disableBlend();
}

void Graphics::clear( const float r, const float g, const float b, const float a, const float depth ) {
    m_impl->clear( r, g, b, a, depth );
}

void Graphics::setViewport( const int x, const int y, const int width, const int height ) {
    m_impl->setViewport( x, y, width, height );
}

void Graphics::setVSync( const bool vsync ) {
    m_impl->setVSync( vsync );
}

void Graphics::swapBuffers() {
    m_impl->swapBuffers();
}




Shader::Shader() :
    m_impl( nullptr ) {
}

Shader::Shader( Shader&& toMove ) :
    m_impl( toMove.m_impl ) {
    toMove.m_impl = nullptr;
}
Shader& Shader::operator =( Shader&& toMove ) {
    m_impl = toMove.m_impl;
    toMove.m_impl = nullptr;
    return *this;
}

Shader::Shader( Private::ShaderImpl* impl ) :
    m_impl( impl ) {
}

Shader::~Shader() {
    if( m_impl != nullptr )
        delete m_impl;
}

void Shader::create() {
    m_impl->create();
}

void Shader::destroy() {
    m_impl->destroy();
}

void Shader::setSource( const ustring& source ) {
    m_impl->setSource( source );
}

void Shader::compile() {
    m_impl->compile();
}




Program::Program() :
    m_impl( nullptr ) {
}

Program::Program( Program&& toMove ) :
    m_impl( toMove.m_impl ) {
    toMove.m_impl = nullptr;
}
Program& Program::operator =( Program&& toMove ) {
    m_impl = toMove.m_impl;
    toMove.m_impl = nullptr;
    return *this;
}

Program::Program( Private::ProgramImpl* impl ) :
    m_impl( impl ) {
}

Program::~Program() {
    if( m_impl != nullptr )
        delete m_impl;
}

void Program::create() {
    m_impl->create();
}

void Program::destroy() {
    m_impl->destroy();
}

void Program::attachShader( const Shader& shader ) {
    m_impl->attachShader( *shader.m_impl );
}

void Program::detachShader( const Shader& shader ) {
    m_impl->detachShader( *shader.m_impl );
}

void Program::link() {
    m_impl->link();
}

void Program::use() {
    m_impl->use();
}

void Program::stopUsing() {
    m_impl->stopUsing();
}

void Program::setUniform( const char* const name, const int value ) {
    m_impl->setUniform( name, value );
}

void Program::setUniform( const char* const name, const uint value ) {
    m_impl->setUniform( name, value );
}

void Program::setUniform( const char* const name, const float value ) {
    m_impl->setUniform( name, value );
}

void Program::setUniform( const char* const name, const int x, const int y ) {
    m_impl->setUniform( name, x, y );
}

void Program::setUniform( const char* const name, const float x, const float y ) {
    m_impl->setUniform( name, x, y );
}

void Program::setUniform( const char* const name, const int x, const int y, const int z, const int w ) {
    m_impl->setUniform( name, x, y, z, w );
}

void Program::setUniform( const char* const name, const float x, const float y, const float z, const float w ) {
    m_impl->setUniform( name, x, y, z, w );
}




VertexBuffer::VertexBuffer() :
    m_impl( nullptr ) {
}

VertexBuffer::VertexBuffer( VertexBuffer&& toMove ) :
    m_impl( toMove.m_impl ) {
    toMove.m_impl = nullptr;
}
VertexBuffer& VertexBuffer::operator =( VertexBuffer&& toMove ) {
    m_impl = toMove.m_impl;
    toMove.m_impl = nullptr;
    return *this;
}

VertexBuffer::VertexBuffer( Private::VertexBufferImpl* impl ) :
    m_impl( impl ) {
}

VertexBuffer::~VertexBuffer() {
    if( m_impl != nullptr )
        delete m_impl;
}

void VertexBuffer::create() {
    m_impl->create();
}

void VertexBuffer::destroy() {
    m_impl->destroy();
}

void VertexBuffer::set( const float* const data, const size_t sizeInBytes ) {
    m_impl->set( data, sizeInBytes );
}

void VertexBuffer::set( const float* const data, const size_t offsetInBytes, const size_t sizeInBytes ) {
    m_impl->set( data, offsetInBytes, sizeInBytes );
}

void VertexBuffer::bind() {
    m_impl->bind();
}

void VertexBuffer::unbind() {
    m_impl->unbind();
}

void VertexBuffer::draw() {
    m_impl->draw();
}

void VertexBuffer::setType( const int type ) {
    m_impl->setType( type );
}




Texture::Texture() :
    m_impl( nullptr ) {
}

Texture::Texture( Texture&& toMove ) :
    m_impl( toMove.m_impl ) {
    toMove.m_impl = nullptr;
}

Texture& Texture::operator =( Texture&& toMove ) {
    m_impl = toMove.m_impl;
    toMove.m_impl = nullptr;
    return *this;
}

Texture::Texture( Private::TextureImpl* impl ) :
    m_impl( impl ) {
}

Texture::~Texture() {
    if( m_impl != nullptr )
        delete m_impl;
}

void Texture::create() {
    m_impl->create();
}

void Texture::destroy() {
    m_impl->destroy();
}

void Texture::set( const size_t width, const size_t height, const void* data ) {
    m_impl->set( width, height, data );
}

void Texture::bind() {
    m_impl->bind();
}

void Texture::unbind() {
    m_impl->unbind();
}

size_t Texture::getWidth() const {
    return m_impl->getWidth();
}

size_t Texture::getHeight() const {
    return m_impl->getHeight();
}




Sampler::Sampler() :
    m_impl( nullptr ) {
}

Sampler::Sampler( Sampler&& toMove ) :
    m_impl( toMove.m_impl ) {
    toMove.m_impl = nullptr;
}

Sampler& Sampler::operator =( Sampler&& toMove ) {
    m_impl = toMove.m_impl;
    toMove.m_impl = nullptr;
    return *this;
}

Sampler::Sampler( Private::SamplerImpl* impl ) :
    m_impl( impl ) {
}

Sampler::~Sampler() {
    if( m_impl != nullptr )
        delete m_impl;
}
void Sampler::create() {
    m_impl->create();
}

void Sampler::destroy() {
    m_impl->destroy();
}

void Sampler::setMinFilter( const FilterType type ) {
    m_impl->setMinFilter( type );
}

void Sampler::setMagFilter( const FilterType type ) {
    m_impl->setMagFilter( type );
}

void Sampler::setUWrap( const WrapType type ) {
    m_impl->setUWrap( type );
}

void Sampler::setVWrap( const WrapType type ) {
    m_impl->setVWrap( type );
}

void Sampler::bind() {
    m_impl->bind();
}

void Sampler::unbind() {
    m_impl->unbind();
}

}
