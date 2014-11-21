/*
opengl/GLLoad.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Loads extensions for OpenGL
*/
#ifndef BS_OPENGL_GLLOAD_HPP
#define BS_OPENGL_GLLOAD_HPP




//Includes
#include "GLHeader.hpp"                             //OpenGL
#include "glext.h"                                  //GL_* constants
#include "glcorearb.h"                              //OpenGL

#ifdef BS_BUILD_WINDOWS
#include "wglext.h"                                 //TEMP: defines of the form PFNWGL...PROC for function pointer to OpenGL extensions
#endif //BS_BUILD_WINDOWS




//Forward declarations

//Windows GL extensions
#if defined( BS_BUILD_WINDOWS )
extern PFNWGLCREATECONTEXTATTRIBSARBPROC    wglCreateContextAttribsARB;
extern PFNWGLCHOOSEPIXELFORMATARBPROC       wglChoosePixelFormatARB;
extern PFNWGLGETEXTENSIONSSTRINGARBPROC     wglGetExtensionsStringARB;
extern PFNWGLSWAPINTERVALEXTPROC            wglSwapIntervalEXT;
extern PFNWGLGETSWAPINTERVALEXTPROC         wglGetSwapIntervalEXT;

//GL X11 extensions
#elif defined( BS_BUILD_LINUX )

#endif

//Globals
extern PFNGLGETSTRINGIPROC                  glGetStringi;

//Shaders
extern PFNGLCREATESHADERPROC                glCreateShader;
extern PFNGLDELETESHADERPROC                glDeleteShader;
extern PFNGLSHADERSOURCEPROC                glShaderSource;
extern PFNGLCOMPILESHADERPROC               glCompileShader;
extern PFNGLGETSHADERIVPROC                 glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC            glGetShaderInfoLog;

//Shader programs
extern PFNGLCREATEPROGRAMPROC               glCreateProgram;
extern PFNGLDELETEPROGRAMPROC               glDeleteProgram;
extern PFNGLATTACHSHADERPROC                glAttachShader;
extern PFNGLDETACHSHADERPROC                glDetachShader;
extern PFNGLGETPROGRAMIVPROC                glGetProgramiv;
extern PFNGLLINKPROGRAMPROC                 glLinkProgram;
extern PFNGLGETPROGRAMINFOLOGPROC           glGetProgramInfoLog;
extern PFNGLUSEPROGRAMPROC                  glUseProgram;

//Buffers
extern PFNGLGENBUFFERSPROC                  glGenBuffers;
extern PFNGLDELETEBUFFERSPROC               glDeleteBuffers;
extern PFNGLBINDBUFFERPROC                  glBindBuffer;
extern PFNGLBUFFERDATAPROC                  glBufferData;
extern PFNGLBUFFERSUBDATAPROC               glBufferSubData;

//Vertex attributes
extern PFNGLVERTEXATTRIBPOINTERPROC         glVertexAttribPointer;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC     glEnableVertexAttribArray;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC    glDisableVertexAttribArray;

//Textures
extern PFNGLACTIVETEXTUREPROC               glActiveTexture;

//Samplers
extern PFNGLGENSAMPLERSPROC                 glGenSamplers;
extern PFNGLSAMPLERPARAMETERIPROC           glSamplerParameteri;
extern PFNGLBINDSAMPLERPROC                 glBindSampler;
extern PFNGLDELETESAMPLERSPROC              glDeleteSamplers;

//Uniforms
extern PFNGLUNIFORM1IPROC                   glUniform1i;
extern PFNGLUNIFORM1UIPROC                  glUniform1ui;
extern PFNGLUNIFORM1FPROC                   glUniform1f;
extern PFNGLUNIFORM1IVPROC                  glUniform1iv;
extern PFNGLUNIFORM1UIVPROC                 glUniform1uiv;
extern PFNGLUNIFORM1FVPROC                  glUniform1fv;
extern PFNGLUNIFORM2IPROC                   glUniform2i;
extern PFNGLUNIFORM2UIPROC                  glUniform2ui;
extern PFNGLUNIFORM2FPROC                   glUniform2f;
extern PFNGLUNIFORM2IVPROC                  glUniform2iv;
extern PFNGLUNIFORM2UIVPROC                 glUniform2uiv;
extern PFNGLUNIFORM2FVPROC                  glUniform2fv;
extern PFNGLUNIFORM3IPROC                   glUniform3i;
extern PFNGLUNIFORM3UIPROC                  glUniform3ui;
extern PFNGLUNIFORM3FPROC                   glUniform3f;
extern PFNGLUNIFORM3IVPROC                  glUniform3iv;
extern PFNGLUNIFORM3UIVPROC                 glUniform3uiv;
extern PFNGLUNIFORM3FVPROC                  glUniform3fv;
extern PFNGLUNIFORM4IPROC                   glUniform4i;
extern PFNGLUNIFORM4UIPROC                  glUniform4ui;
extern PFNGLUNIFORM4FPROC                   glUniform4f;
extern PFNGLUNIFORM4IVPROC                  glUniform4iv;
extern PFNGLUNIFORM4UIVPROC                 glUniform4uiv;
extern PFNGLUNIFORM4FVPROC                  glUniform4fv;
extern PFNGLUNIFORMMATRIX2FVPROC            glUniformMatrix2fv;
extern PFNGLUNIFORMMATRIX3FVPROC            glUniformMatrix3fv;
extern PFNGLUNIFORMMATRIX4FVPROC            glUniformMatrix4fv;
extern PFNGLGETUNIFORMLOCATIONPROC          glGetUniformLocation;

//Blending
extern PFNGLBLENDEQUATIONSEPARATEPROC       glBlendEquationSeparate;
extern PFNGLBLENDFUNCSEPARATEPROC           glBlendFuncSeparate;




namespace Brimstone {
namespace Private {

void loadOpenGLExtensions();

}
}

#endif //BS_OPENGL_GLLOAD_HPP