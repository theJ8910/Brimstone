/*
opengl/GLVertexBuffer.cpp
-------------------------
Copyright (c) 2024, theJ89

Description:
    See GLVertexBuffer.hpp for more information.
*/




//Includes
#include "GLVertexBuffer.hpp"   //Header file

#include <gll/gl_4_4_comp.hpp>  //gll::* (GL 4.4 and below + compatibility)
using namespace gll;




namespace Brimstone {
namespace Private {

GLVertexBuffer::GLVertexBuffer() :
    m_name( 0 ),
    m_count( 0 ) {
    create();
}

GLVertexBuffer::~GLVertexBuffer() {
    destroy();
}

void GLVertexBuffer::create() {
    glGenBuffers( 1, &m_name );
}

void GLVertexBuffer::destroy() {
    if( m_name != 0 ) {
        glDeleteBuffers( 1, &m_name );
        m_name = 0;
    }
}

void GLVertexBuffer::set( const float* const data, const std::size_t sizeInBytes ) {
    //Temporarily bind the buffer to GL_ARRAY_BUFFER so we can fill it
    //with the data the user provided
    glBindBuffer( GL_ARRAY_BUFFER, m_name                            );
    glBufferData( GL_ARRAY_BUFFER, sizeInBytes, data, GL_STATIC_DRAW );
    glBindBuffer( GL_ARRAY_BUFFER, 0                                 );

    int vsize = 0;
    switch( m_type ) {
    case 0:
        vsize = 12;
        break;
    case 1:
        vsize = 2;
        break;
    case 2:
        vsize = 4;
        break;
    }

    //Number of vertices in the buffer, determined by dividing
    //the size of the buffer by the size of a single vertex
    m_count = sizeInBytes / (vsize*sizeof(float));
}

void GLVertexBuffer::set( const float* const data, const std::size_t offsetInBytes, const std::size_t sizeInBytes ) {
    glBindBuffer( GL_ARRAY_BUFFER, m_name );
    glBufferSubData( GL_ARRAY_BUFFER, offsetInBytes, sizeInBytes, data );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

void GLVertexBuffer::bind() {
    //IMPORTANT: Whether or not a buffer is bound has an impact on glVertexAttribPointer.
    //If a buffer is bound, glVertexAttribPointer treats the last argument as a BYTE OFFSET.
    //Otherwise, it treats the last argument as a pointer to the first attribute in client memory.
    //OpenGL 3.1+ removed the ability to use client memory, which means we NEED to bind the buffer before calling glVertexAttribPointer.
    //See for more info: http://stackoverflow.com/questions/15380491/glvertexattribpointer-in-opengl-and-in-opengles
    glBindBuffer( GL_ARRAY_BUFFER, m_name );

    //TEMP: attribute arrays hardcoded in
    /*
    glVertexAttribPointer(
        index,      //ID# of the attribute, starting from 0
        count,      //number of elements that make up the attribute. Can be 1,2,3, or 4.
        type,       //type of data used. Can be GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_FIXED, or GL_FLOAT.
        normalized, //If type is GL_FIXED, providing GL_TRUE normalizes provided numbers, and performs direct conversion otherwise. I don't understand it that well.
        stride,     //Distance, in bytes, between two attributes of the same type.
        pointer     //Distance, in bytes, between the beginning of the array and the first attribute of this type.
    );

    Note:
    Because our attributes are arranged in a per-vertex fashion, (e.g. {0,1,2,3}, {0,1,2,3}, ... )
    The stride for all attributes will be the same (the sum of all the attribute sizes).
    The offset for each attribute will be the sum of the attribute sizes that came before it.
    */
    switch( m_type ) {
    //3D textured, colored, lit
    case 0:
        //Position
        glEnableVertexAttribArray( 0 );
        glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, 12*sizeof( float ), (GLvoid*)(0*sizeof(float)) );

        //Color
        glEnableVertexAttribArray( 1 );
        glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 12*sizeof( float ), (GLvoid*)(4*sizeof(float)) );

        //UV (texture coordinates)
        glEnableVertexAttribArray( 2 );
        glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 12*sizeof( float ), (GLvoid*)(7*sizeof(float)) );

        //Normal ('facing direction')
        glEnableVertexAttribArray( 3 );
        glVertexAttribPointer( 3, 3, GL_FLOAT, GL_FALSE, 12*sizeof( float ), (GLvoid*)(9*sizeof(float)) );
        break;
    //2D untextured
    case 1:
        //Position
        glEnableVertexAttribArray( 0 );
        glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 2*sizeof( float ), (GLvoid*)(0*sizeof(float)) );
        break;
    //2D textured
    case 2:
        //Position
        glEnableVertexAttribArray( 0 );
        glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 4*sizeof( float ), (GLvoid*)(0*sizeof(float)) );

        //UV (texture coordinates)
        glEnableVertexAttribArray( 1 );
        glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 4*sizeof( float ), (GLvoid*)(2*sizeof(float)) );
        break;
    }
}

void GLVertexBuffer::unbind() {
    //TEMP: attribute arrays hardcoded in
    switch( m_type ) {
    //3D textured, colored, lit
    case 0:
        glDisableVertexAttribArray( 0 );
        glDisableVertexAttribArray( 1 );
        glDisableVertexAttribArray( 2 );
        glDisableVertexAttribArray( 3 );
        break;
    //2D untextured
    case 1:
        glDisableVertexAttribArray( 0 );
        break;
    //2D textured
    case 2:
        glDisableVertexAttribArray( 0 );
        glDisableVertexAttribArray( 1 );
        break;
    }

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

void GLVertexBuffer::draw() {
    glDrawArrays( GL_TRIANGLES, 0, m_count );
}

//TEMP
void GLVertexBuffer::setType( const int type ) {
    m_type = type;
}

}
}
