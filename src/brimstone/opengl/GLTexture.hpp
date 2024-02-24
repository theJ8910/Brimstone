/*
opengl/GLTexture.hpp
--------------------
Copyright (c) 2024, theJ89

Description:
    GLTexture is defined here.
    These objects wrap OpenGL textures.
*/
#ifndef BS_OPENGL_GLTEXTURE_HPP
#define BS_OPENGL_GLTEXTURE_HPP

//Includes
#include <gll/gl_types.hpp>      //gll::GLsizei




namespace Brimstone {
namespace Private {

class GLTexture {
public:
    GLTexture();
    ~GLTexture();

    void create();
    void destroy();

    void set( const size_t width, const size_t height, const void* data );

    void bind();
    void unbind();

    size_t getWidth() const;
    size_t getHeight() const;
private:
    gll::GLuint m_name;

    gll::GLsizei m_width;
    gll::GLsizei m_height;
};

}
}

#endif //BS_OPENGL_GLTEXTURE_HPP
