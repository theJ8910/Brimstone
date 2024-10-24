/*
opengl/GLShader.hpp
-------------------
Copyright (c) 2024, theJ89

Description:
    GLShader is defined here.
    These objects manage created shaders.
*/
#ifndef BS_OPENGL_GLSHADER_HPP
#define BS_OPENGL_GLSHADER_HPP




//Includes
#include <brimstone/types.hpp>  //Brimstone::ustring
#include <gll/gl_types.hpp>     //gll::GLuint




namespace Brimstone {




//Forward declarations
enum class ShaderType;




} //namespace Brimstone




namespace Brimstone::Private {




class GLShader {
friend class GLProgram;
public:
    GLShader( const ShaderType type );
    GLShader( GLShader& toCopy ) = delete;
    GLShader& operator =( GLShader& toCopy ) = delete;
    ~GLShader();

    void create();
    void destroy();
    void setSource( const ustring& source );
    void compile();
private:
    ShaderType  m_type;
    gll::GLuint m_name;
};




} //namespace Brimstone::Private




#endif //BS_OPENGL_GLSHADER_HPP
