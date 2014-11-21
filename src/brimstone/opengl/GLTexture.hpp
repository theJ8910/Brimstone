/*
opengl/GLTexture.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    GLTexture is defined here.
    These objects wrap OpenGL textures.
*/
#ifndef BS_OPENGL_GLTEXTURE_HPP
#define BS_OPENGL_GLTEXTURE_HPP

//Includes
#include "GLHeader.hpp"     //OpenGL




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
    GLuint m_name;

    GLsizei m_width;
    GLsizei m_height;
};

}
}

#endif //BS_OPENGL_GLTEXTURE_HPP