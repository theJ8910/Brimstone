/*
graphics/DGraphicsImpl.hpp
-----------------------
Copyright (c) 2014, theJ89

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

namespace Brimstone {
namespace Private {

#if defined( BS_BUILD_DIRECT3D )
typedef class D3DGraphicsImpl   GraphicsImpl;
typedef class D3DShader         ShaderImpl;
typedef class D3DProgram        ProgramImpl;
typedef class D3DVertexBuffer   VertexBufferImpl;
typedef class D3DTexture        TextureImpl;
typedef class D3DSampler        SamplerImpl;
#elif defined( BS_BUILD_OPENGL )
typedef class GLGraphicsImpl    GraphicsImpl;
typedef class GLShader          ShaderImpl;
typedef class GLProgram         ProgramImpl;
typedef class GLVertexBuffer    VertexBufferImpl;
typedef class GLTexture         TextureImpl;
typedef class GLSampler         SamplerImpl;
#endif

}
}

#endif //BS_GRAPHICS_DGRAPHICSIMPL_HPP