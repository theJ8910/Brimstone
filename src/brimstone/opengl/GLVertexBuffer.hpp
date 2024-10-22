/*
opengl/GLVertexBuffer.hpp
-------------------------
Copyright (c) 2024, theJ89

Description:
    GLVertexBuffer is defined here.
    These objects wrap OpenGL buffers.
*/
#ifndef BS_OPENGL_GLVERTEXBUFFER_HPP
#define BS_OPENGL_GLVERTEXBUFFER_HPP




//Includes
#include <cstddef>           //std::size_t
#include <gll/gl_types.hpp>  //gll::GLuint, gll::GLsizei




namespace Brimstone::Private {




class GLVertexBuffer {
public:
    GLVertexBuffer();
    GLVertexBuffer( GLVertexBuffer& toCopy ) = delete;
    GLVertexBuffer& operator =( GLVertexBuffer& toCopy ) = delete;
    ~GLVertexBuffer();

    void create();
    void destroy();

    void set( const float* const data, const std::size_t sizeInBytes );
    void set( const float* const data, const std::size_t offsetInBytes, const std::size_t sizeInBytes );
    void bind();
    void unbind();
    void draw();

    void setType( const int type );
private:
    gll::GLuint  m_name;
    gll::GLsizei m_count;
    int          m_type;
};




} //namespace Brimstone::Private




#endif //BS_OPENGL_GLVERTEXBUFFER_HPP
