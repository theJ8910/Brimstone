/*
graphics/GraphicsImpl.hpp
-------------------------
Copyright (c) 2024, theJ89

Description:
    Includes the header for GraphicsImpl's implementation,
    which varies according to build settings.
*/
#ifndef BS_GRAPHICS_GRAPHICSIMPL_HPP
#define BS_GRAPHICS_GRAPHICSIMPL_HPP




//Includes
#include <brimstone/graphics/DGraphicsImpl.hpp>  //Brimstone::Private::DGraphicsImpl, etc

#if defined( BS_BUILD_DIRECT3D )
#include "../direct3d/D3DGraphicsImpl.hpp"
#include "../direct3d/D3DShader.hpp"
#include "../direct3d/D3DProgram.hpp"
#include "../direct3d/D3DVertexBuffer.hpp"
#include "../direct3d/D3DTexture.hpp"
#include "../direct3d/D3DSampler.hpp"
#elif defined( BS_BUILD_OPENGL )
#include "../opengl/GLGraphicsImpl.hpp"
#include "../opengl/GLShader.hpp"
#include "../opengl/GLProgram.hpp"
#include "../opengl/GLVertexBuffer.hpp"
#include "../opengl/GLTexture.hpp"
#include "../opengl/GLSampler.hpp"
#endif




#endif //BS_GRAPHICS_GRAPHICSIMPL_HPP
