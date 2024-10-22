/*
graphics/DGraphicsImpl.hpp
--------------------------
Copyright (c) 2024, theJ89

Description:
    Forward declares the following classes' implementation,
    which varies according to build settings:
    * GraphicsImpl
    * ShaderImpl
    * ProgramImpl
    * VertexBufferImpl
    * TextureImpl
    * SamplerImpl

    These types are defined as a aliases of the chosen implementation.
*/
#ifndef BS_GRAPHICS_DGRAPHICSIMPL_HPP
#define BS_GRAPHICS_DGRAPHICSIMPL_HPP




namespace Brimstone::Private {




//Types
#if defined( BS_BUILD_DIRECT3D )
using GraphicsImpl     = class D3DGraphicsImpl;
using ShaderImpl       = class D3DShader;
using ProgramImpl      = class D3DProgram;
using VertexBufferImpl = class D3DVertexBuffer;
using TextureImpl      = class D3DTexture;
using SamplerImpl      = class D3DSampler;
#elif defined( BS_BUILD_OPENGL )
using GraphicsImpl     = class GLGraphicsImpl;
using ShaderImpl       = class GLShader;
using ProgramImpl      = class GLProgram;
using VertexBufferImpl = class GLVertexBuffer;
using TextureImpl      = class GLTexture;
using SamplerImpl      = class GLSampler;
#endif




} //namespace Brimstone::Private




#endif //BS_GRAPHICS_DGRAPHICSIMPL_HPP
