/*
opengl/GLLoad.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    See GLLoad.hpp for more information.
*/




//Includes
#include "GLLoad.hpp"




//Windows GL extensions
#if defined( BS_BUILD_WINDOWS )
PFNWGLCREATECONTEXTATTRIBSARBPROC   wglCreateContextAttribsARB  = nullptr;
PFNWGLCHOOSEPIXELFORMATARBPROC      wglChoosePixelFormatARB     = nullptr;
PFNWGLGETEXTENSIONSSTRINGARBPROC    wglGetExtensionsStringARB   = nullptr;
PFNWGLSWAPINTERVALEXTPROC           wglSwapIntervalEXT          = nullptr;
PFNWGLGETSWAPINTERVALEXTPROC        wglGetSwapIntervalEXT       = nullptr;

//GL X11 extensions
#elif defined( BS_BUILD_LINUX )

#endif

//Globals
PFNGLGETSTRINGIPROC                 glGetStringi                = nullptr;

//Shaders
PFNGLCREATESHADERPROC               glCreateShader              = nullptr;
PFNGLDELETESHADERPROC               glDeleteShader              = nullptr;
PFNGLSHADERSOURCEPROC               glShaderSource              = nullptr;
PFNGLCOMPILESHADERPROC              glCompileShader             = nullptr;
PFNGLGETSHADERIVPROC                glGetShaderiv               = nullptr;
PFNGLGETSHADERINFOLOGPROC           glGetShaderInfoLog          = nullptr;

//Shader programs
PFNGLCREATEPROGRAMPROC              glCreateProgram             = nullptr;
PFNGLDELETEPROGRAMPROC              glDeleteProgram             = nullptr;
PFNGLATTACHSHADERPROC               glAttachShader              = nullptr;
PFNGLDETACHSHADERPROC               glDetachShader              = nullptr;
PFNGLLINKPROGRAMPROC                glLinkProgram               = nullptr;
PFNGLGETPROGRAMIVPROC               glGetProgramiv              = nullptr;
PFNGLGETPROGRAMINFOLOGPROC          glGetProgramInfoLog         = nullptr;
PFNGLUSEPROGRAMPROC                 glUseProgram                = nullptr;

//Buffers
PFNGLGENBUFFERSPROC                 glGenBuffers                = nullptr;
PFNGLDELETEBUFFERSPROC              glDeleteBuffers             = nullptr;
PFNGLBINDBUFFERPROC                 glBindBuffer                = nullptr;
PFNGLBUFFERDATAPROC                 glBufferData                = nullptr;
PFNGLBUFFERSUBDATAPROC              glBufferSubData             = nullptr;


//Vertex attributes
PFNGLVERTEXATTRIBPOINTERPROC        glVertexAttribPointer       = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC    glEnableVertexAttribArray   = nullptr;
PFNGLDISABLEVERTEXATTRIBARRAYPROC   glDisableVertexAttribArray  = nullptr;

//Textures
PFNGLACTIVETEXTUREPROC              glActiveTexture             = nullptr;

//Samplers
PFNGLGENSAMPLERSPROC                glGenSamplers               = nullptr;
PFNGLSAMPLERPARAMETERIPROC          glSamplerParameteri         = nullptr;
PFNGLBINDSAMPLERPROC                glBindSampler               = nullptr;
PFNGLDELETESAMPLERSPROC             glDeleteSamplers            = nullptr;

//Uniforms
PFNGLUNIFORM1IPROC                  glUniform1i                 = nullptr;
PFNGLUNIFORM1UIPROC                 glUniform1ui                = nullptr;
PFNGLUNIFORM1FPROC                  glUniform1f                 = nullptr;
PFNGLUNIFORM1IVPROC                 glUniform1iv                = nullptr;
PFNGLUNIFORM1UIVPROC                glUniform1uiv               = nullptr;
PFNGLUNIFORM1FVPROC                 glUniform1fv                = nullptr;

PFNGLUNIFORM2IPROC                  glUniform2i                 = nullptr;
PFNGLUNIFORM2UIPROC                 glUniform2ui                = nullptr;
PFNGLUNIFORM2FPROC                  glUniform2f                 = nullptr;
PFNGLUNIFORM2IVPROC                 glUniform2iv                = nullptr;
PFNGLUNIFORM2UIVPROC                glUniform2uiv               = nullptr;
PFNGLUNIFORM2FVPROC                 glUniform2fv                = nullptr;

PFNGLUNIFORM3IPROC                  glUniform3i                 = nullptr;
PFNGLUNIFORM3UIPROC                 glUniform3ui                = nullptr;
PFNGLUNIFORM3FPROC                  glUniform3f                 = nullptr;
PFNGLUNIFORM3IVPROC                 glUniform3iv                = nullptr;
PFNGLUNIFORM3UIVPROC                glUniform3uiv               = nullptr;
PFNGLUNIFORM3FVPROC                 glUniform3fv                = nullptr;

PFNGLUNIFORM4IPROC                  glUniform4i                 = nullptr;
PFNGLUNIFORM4UIPROC                 glUniform4ui                = nullptr;
PFNGLUNIFORM4FPROC                  glUniform4f                 = nullptr;
PFNGLUNIFORM4IVPROC                 glUniform4iv                = nullptr;
PFNGLUNIFORM4UIVPROC                glUniform4uiv               = nullptr;
PFNGLUNIFORM4FVPROC                 glUniform4fv                = nullptr;

PFNGLUNIFORMMATRIX2FVPROC           glUniformMatrix2fv          = nullptr;
PFNGLUNIFORMMATRIX3FVPROC           glUniformMatrix3fv          = nullptr;
PFNGLUNIFORMMATRIX4FVPROC           glUniformMatrix4fv          = nullptr;
PFNGLGETUNIFORMLOCATIONPROC         glGetUniformLocation        = nullptr;

//Blending
PFNGLBLENDEQUATIONSEPARATEPROC      glBlendEquationSeparate     = nullptr;
PFNGLBLENDFUNCSEPARATEPROC          glBlendFuncSeparate         = nullptr;




namespace Brimstone {
namespace Private {




//Returns the process address
#if defined( BS_BUILD_WINDOWS )

PROC getProcAddress( LPCSTR extension ) {
    return wglGetProcAddress( extension );
}

#elif defined( BS_BUILD_LINUX )

typedef void(*ProcAddress)();
ProcAddress getProcAddress( const GLubyte* extension ) {
    return glXGetProcAddress( extension );
}

#endif




void loadOpenGLExtensions() {
    //Windows GL extension
#if defined( BS_BUILD_WINDOWS )
    wglCreateContextAttribsARB  = (PFNWGLCREATECONTEXTATTRIBSARBPROC)   getProcAddress( "wglCreateContextAttribsARB" );
    wglChoosePixelFormatARB     = (PFNWGLCHOOSEPIXELFORMATARBPROC)      getProcAddress( "wglChoosePixelFormatARB"    );
    wglGetExtensionsStringARB   = (PFNWGLGETEXTENSIONSSTRINGARBPROC)    getProcAddress( "wglGetExtensionsStringARB"  );
    wglSwapIntervalEXT          = (PFNWGLSWAPINTERVALEXTPROC)           getProcAddress( "wglSwapIntervalEXT"         );
    wglGetSwapIntervalEXT       = (PFNWGLGETSWAPINTERVALEXTPROC)        getProcAddress( "wglGetSwapIntervalEXT"      );

    //GL X11 extensions
#elif defined( BS_BUILD_LINUX )

#endif

    //Globals
    glGetStringi                = (PFNGLGETSTRINGIPROC)                 getProcAddress( "glGetStringi"               );

    //Shaders
    glCreateShader              = (PFNGLCREATESHADERPROC)               getProcAddress( "glCreateShader"             );
    glDeleteShader              = (PFNGLDELETESHADERPROC)               getProcAddress( "glDeleteShader"             );
    glShaderSource              = (PFNGLSHADERSOURCEPROC)               getProcAddress( "glShaderSource"             );
    glCompileShader             = (PFNGLCOMPILESHADERPROC)              getProcAddress( "glCompileShader"            );
    glGetShaderiv               = (PFNGLGETSHADERIVPROC)                getProcAddress( "glGetShaderiv"              );
    glGetShaderInfoLog          = (PFNGLGETSHADERINFOLOGPROC)           getProcAddress( "glGetShaderInfoLog"         );

    //Shader programs
    glCreateProgram             = (PFNGLCREATEPROGRAMPROC)              getProcAddress( "glCreateProgram"            );
    glDeleteProgram             = (PFNGLDELETEPROGRAMPROC)              getProcAddress( "glDeleteProgram"            );
    glAttachShader              = (PFNGLATTACHSHADERPROC)               getProcAddress( "glAttachShader"             );
    glDetachShader              = (PFNGLDETACHSHADERPROC)               getProcAddress( "glDetachShader"             );
    glLinkProgram               = (PFNGLLINKPROGRAMPROC)                getProcAddress( "glLinkProgram"              );
    glGetProgramiv              = (PFNGLGETPROGRAMIVPROC)               getProcAddress( "glGetProgramiv"             );
    glGetProgramInfoLog         = (PFNGLGETPROGRAMINFOLOGPROC)          getProcAddress( "glGetProgramInfoLog"        );
    glUseProgram                = (PFNGLUSEPROGRAMPROC)                 getProcAddress( "glUseProgram"               );

    //Buffers
    glGenBuffers                = (PFNGLGENBUFFERSPROC)                 getProcAddress( "glGenBuffers"               );
    glBindBuffer                = (PFNGLBINDBUFFERPROC)                 getProcAddress( "glBindBuffer"               );
    glBufferData                = (PFNGLBUFFERDATAPROC)                 getProcAddress( "glBufferData"               );
    glDeleteBuffers             = (PFNGLDELETEBUFFERSPROC)              getProcAddress( "glDeleteBuffers"            );

    //Vertex attributes
    glVertexAttribPointer       = (PFNGLVERTEXATTRIBPOINTERPROC)        getProcAddress( "glVertexAttribPointer"      );
    glEnableVertexAttribArray   = (PFNGLENABLEVERTEXATTRIBARRAYPROC)    getProcAddress( "glEnableVertexAttribArray"  );
    glDisableVertexAttribArray  = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)   getProcAddress( "glDisableVertexAttribArray" );

    //Textures
    glActiveTexture             = (PFNGLACTIVETEXTUREPROC)              getProcAddress( "glActiveTexture"            );

    //Samplers
    glGenSamplers               = (PFNGLGENSAMPLERSPROC)                getProcAddress( "glGenSamplers"              );
    glSamplerParameteri         = (PFNGLSAMPLERPARAMETERIPROC)          getProcAddress( "glSamplerParameteri"        );
    glBindSampler               = (PFNGLBINDSAMPLERPROC)                getProcAddress( "glBindSampler"              );
    glDeleteSamplers            = (PFNGLDELETESAMPLERSPROC)             getProcAddress( "glDeleteSamplers"           );

    //Uniforms
    glUniform1i                 = (PFNGLUNIFORM1IPROC)                  getProcAddress( "glUniform1i"                );
    glUniform1ui                = (PFNGLUNIFORM1UIPROC)                 getProcAddress( "glUniform1ui"               );
    glUniform1f                 = (PFNGLUNIFORM1FPROC)                  getProcAddress( "glUniform1f"                );
    glUniform1iv                = (PFNGLUNIFORM1IVPROC)                 getProcAddress( "glUniform1iv"               );
    glUniform1uiv               = (PFNGLUNIFORM1UIVPROC)                getProcAddress( "glUniform1uiv"              );
    glUniform1fv                = (PFNGLUNIFORM1FVPROC)                 getProcAddress( "glUniform1fv"               );

    glUniform2i                 = (PFNGLUNIFORM2IPROC)                  getProcAddress( "glUniform2i"                );
    glUniform2ui                = (PFNGLUNIFORM2UIPROC)                 getProcAddress( "glUniform2ui"               );
    glUniform2f                 = (PFNGLUNIFORM2FPROC)                  getProcAddress( "glUniform2f"                );
    glUniform2iv                = (PFNGLUNIFORM2IVPROC)                 getProcAddress( "glUniform2iv"               );
    glUniform2uiv               = (PFNGLUNIFORM2UIVPROC)                getProcAddress( "glUniform2uiv"              );
    glUniform2fv                = (PFNGLUNIFORM2FVPROC)                 getProcAddress( "glUniform2fv"               );

    glUniform3i                 = (PFNGLUNIFORM3IPROC)                  getProcAddress( "glUniform3i"                );
    glUniform3ui                = (PFNGLUNIFORM3UIPROC)                 getProcAddress( "glUniform3ui"               );
    glUniform3f                 = (PFNGLUNIFORM3FPROC)                  getProcAddress( "glUniform3f"                );
    glUniform3iv                = (PFNGLUNIFORM3IVPROC)                 getProcAddress( "glUniform3iv"               );
    glUniform3uiv               = (PFNGLUNIFORM3UIVPROC)                getProcAddress( "glUniform3uiv"              );
    glUniform3fv                = (PFNGLUNIFORM3FVPROC)                 getProcAddress( "glUniform3fv"               );

    glUniform4i                 = (PFNGLUNIFORM4IPROC)                  getProcAddress( "glUniform4i"                );
    glUniform4ui                = (PFNGLUNIFORM4UIPROC)                 getProcAddress( "glUniform4ui"               );
    glUniform4f                 = (PFNGLUNIFORM4FPROC)                  getProcAddress( "glUniform4f"                );
    glUniform4iv                = (PFNGLUNIFORM4IVPROC)                 getProcAddress( "glUniform4iv"               );
    glUniform4uiv               = (PFNGLUNIFORM4UIVPROC)                getProcAddress( "glUniform4uiv"              );
    glUniform4fv                = (PFNGLUNIFORM4FVPROC)                 getProcAddress( "glUniform4fv"               );

    glUniformMatrix2fv          = (PFNGLUNIFORMMATRIX2FVPROC)           getProcAddress( "glUniformMatrix2fv"         );
    glUniformMatrix3fv          = (PFNGLUNIFORMMATRIX3FVPROC)           getProcAddress( "glUniformMatrix3fv"         );
    glUniformMatrix4fv          = (PFNGLUNIFORMMATRIX4FVPROC)           getProcAddress( "glUniformMatrix4fv"         );
    glGetUniformLocation        = (PFNGLGETUNIFORMLOCATIONPROC)         getProcAddress( "glGetUniformLocation"       );

    //Blending
    glBlendEquationSeparate     = (PFNGLBLENDEQUATIONSEPARATEPROC)      getProcAddress( "glBlendEquationSeparate"    );
    glBlendFuncSeparate         = (PFNGLBLENDFUNCSEPARATEPROC)          getProcAddress( "glBlendFuncSeparate"        );
}

}
}