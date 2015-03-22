/*
opengl/LinuxGLContext.cpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    See LinuxGLContext.hpp for more information.
*/




//Includes
#include "LinuxGLContext.hpp"           //Header file
#include "../linux/XException.hpp"      //Brimstone::XException

#include <brimstone/Window.hpp>         //Brimstone::Window

#include <GL/glx.h>                     //GLX*, glX*




namespace {

typedef GLXContext (*GLXCREATECONTEXTATTRIBSARBPROC)( Display*, GLXFBConfig, GLXContext, Bool, const int* );
GLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB = nullptr;

/*
isExtensionSupported
-----------------------

Description:
    Checks whether a given extension is supported.

Arguments:
    extensions: An extensions string returned by glXQueryExtensionsString
    extension:  Name of the extension to check.

Returns:
    bool:       true if the extension is supported, false otherwise.
*/
bool isExtensionSupported( const char* extensions, const char* extension ) {
    //There are no extensions with an empty name
    if( *extension == '\0' )
        return false;

    //NOTE: The extensions string is a list of space-separated extension names
    const char* p = extensions;
    const char* p2 = extension;

    while( true ) {
        //Reached end of search string
        if( *p2 == '\0' ) {
            if( *p == ' ' || *p == '\0' ) { return true; }  //Reached end of current string as well; perfect match
            else                          {}                //Current name is longer than search string
        //Reached end of current name; shorter than search string
        } else if( *p == ' ' ) {
            ++p;
            p2 = extension;
            continue;
        //Reached end of final name; shorter than search string
        } else if( *p == '\0' ) {
            return false;
        //Match between characters in current name and search string
        } else if( *p == *p2 ) {
            ++p;
            ++p2;
            continue;
        //Mismatch between characters
        } else {
        }

        //Find the next space, then move 1 character past it to get the next name.
        //If we encounter a null character instead, we've found the end of the final name; we already know it doesn't match so just return false here.
        //NOTE: If we're here, *p isn't ' ' or '\0'.
        //Doing ++p at the start is safe, and can land us on ' ' or '\0' immediately.
        for( ++p; *p != ' '; ++p )
            if( *p == '\0' )
                return false;
        ++p;
        p2 = extension;
    }
}

}

namespace Brimstone {
namespace Private {




int         LinuxGLContext::m_contextCount   = 0;
bool        LinuxGLContext::m_glxInitialized = false;
Display*    LinuxGLContext::m_display        = nullptr;
GLXFBConfig LinuxGLContext::m_bestFBC        = nullptr;





LinuxGLContext::LinuxGLContext() :
    m_context( nullptr ),
    m_window( 0 ) {
}

LinuxGLContext::LinuxGLContext( const Window& window ) :
    LinuxGLContext() {
    init( window );
}

LinuxGLContext::~LinuxGLContext() {
    try                           { destroyContext();        }
    catch( const IException& ex ) { uncaughtException( ex ); }

    destroyFinish();
}

/*
LinuxGLContext::initGLX
-----------------------

Description:
    Initialize GLX with the given display.
    Initializing GLX will locate an ideal framebuffer configuration suitable for rendering to.
    A reference to the provided display as well as the ideal framebuffer that was located will be stored for later use.

    If GLX has already been initialized, the given display must be the same display that GLX was initialized with previously.
    If the displays are the same, the function returns immediately. Otherwise, a GraphicsException is thrown.

Arguments:
    display:                The X11 display to initialize GLX with.

Returns:
    void:                   N/A

Throws:
    GraphicsException:      If display is different from the display that was used to initialize GLX previously.
*/
void LinuxGLContext::initGLX( Display* display ) {
    if( m_glxInitialized ) {
        if( display != m_display )
            throw GraphicsException( "GLX is already loaded with a different display!" );
        return;
    }
    /*
    Our ideal frame buffer should have these attributes.
    Taken from https://www.opengl.org/wiki/Tutorial:_OpenGL_3.0_Context_Creation_%28GLX%29

    Of particular note:
    * 32-bit RGBA color (i.e. each color component is 8-bits large)
    * An 24-bit (3 byte) depth buffer
    * An 8-bit (1 byte) stencil buffer
    * Double buffering
    */
    static int visual_attribs[] = {
        GLX_X_RENDERABLE    , True,
        GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
        GLX_RENDER_TYPE     , GLX_RGBA_BIT,
        GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
        GLX_RED_SIZE        , 8,
        GLX_GREEN_SIZE      , 8,
        GLX_BLUE_SIZE       , 8,
        GLX_ALPHA_SIZE      , 8,
        GLX_DEPTH_SIZE      , 24,
        GLX_STENCIL_SIZE    , 8,
        GLX_DOUBLEBUFFER    , True,
      //GLX_SAMPLE_BUFFERS  , 1,
      //GLX_SAMPLES         , 4,
        None
    };

    //First, we'll need to find the ideal framebuffer configuration.
    //This will be used to provide an ideal Visual for X11 windows later on.
    //Framebuffer configurations were added in GLX 1.3, therefore we require GLX 1.3+.
    int glxMajor, glxMinor;
    if( glXQueryVersion( display, &glxMajor, &glxMinor ) == False )
        throw GraphicsException( "glxQueryVersion failed" );

    if( glxMajor < 1 || ( glxMajor == 1 && glxMinor < 3 ) )
        throw GraphicsException( "Supported version of GLX is too low; GLX 1.3+ is required." );

    //There can be potentially many framebuffer configurations that match our request.
    //We'll enumerate them here.
    //Note: glXChooseFBConfig returns nullptr if it fails OR if there aren't any results.
    int fbCount = 0;
    GLXFBConfig* fbcs = glXChooseFBConfig( display, DefaultScreen( display ), visual_attribs, &fbCount );
    if( fbcs == nullptr )
        throw GraphicsException( "glXChooseFBConfig failed." );

    //Now that we have a list of acceptable framebuffer configurations, lets choose the best one,
    //based on how many samples the framebuffer config supports.
    GLXFBConfig bestFBC = nullptr;
    {
        //Automatically clean up our list of FBCs after we've selected the one we want
        std::unique_ptr< GLXFBConfig, int (*)( void* ) > uptr( fbcs, &XFree );

        int haveSamples = 0;
        int samples     = 0;
        int maxSamples  = 0;
        for( int i = 1; i < fbCount; ++i ) {
            //NOTE: GLX_SAMPLE_BUFFERS is not documented anywhere.
            //It seems to be a simple X11 True / False indicating if a framebuffer configuration supports sample buffers.
            if( glXGetFBConfigAttrib( display, fbcs[i], GLX_SAMPLE_BUFFERS, &haveSamples ) != Success )
                throw GraphicsException( "glxGetFBConfigAttrib failed." );

            if( !haveSamples )
                continue;

            //Grab the # of samples this frame buffer config supports.
            //Replace the previous winner (if any) if this config supports more samples.
            if( glXGetFBConfigAttrib( display, fbcs[i], GLX_SAMPLES,        &samples     ) != Success )
                throw GraphicsException( "glxGetFBConfigAttrib failed." );

            if( bestFBC == nullptr || samples > maxSamples ) {
                maxSamples = samples;
                bestFBC    = fbcs[i];
            }
        }
    }

    //This shouldn't ever happen since the list returned by glXChooseFBConfig will have at least 1 entry, but it doesn't hurt to check.
    if( bestFBC == nullptr )
        throw GraphicsException( "No framebuffer config matching requirements could be found." );
    
    //Grab GLX extensions and check if the "GLX_ARB_create_context" extension is available.
    //We'll need to know this extension is present in order to load glXCreateContextAttribsARB(),
    //a function that will allow us to create an OpenGL context. If the extension isn't available,
    //we can fall back on glXCreateNewContext (an older, less powerful context creation function).
    const char* glxExts = glXQueryExtensionsString( m_display, DefaultScreen( m_display ) );
    if( isExtensionSupported( glxExts, "GLX_ARB_create_context" ) ) {
        //Load the glXCreateContextAttribsARB function. We'll be using it to create a context below.
        //NOTE: Unlike Window's wglGetProcAddress, glXGetProcAddressARB can be called even if a context has not yet been created / made active.
        glXCreateContextAttribsARB = (GLXCREATECONTEXTATTRIBSARBPROC)glXGetProcAddressARB( reinterpret_cast<const GLubyte*>( "glXCreateContextAttribsARB" ) );
        if( glXCreateContextAttribsARB == nullptr )
            throw GraphicsException( "Couldn't load glXCreateContextAttribsARB." );
    }

    m_display        = display;
    m_bestFBC        = bestFBC;
    m_glxInitialized = true;
}

/*
LinuxGLContext::destroyGLX
-----------------------

Description:
    Releases any resources related to GLX.

    TODO:
        Currently, initGLX() can be invoked for reasons other than context creation (i.e. to provide the ideal Visual for the X11 window).
        However, destroyGLX() is called only after the last context is destroyed.
        If no context is ever created, this function will never be called.
        This currently isn't much of an issue since we're not storing any resources that need to be released,
        but we need to find a way to ensure destroyGLX() gets called regardless.
Arguments:
    N/A

Returns:
    void:                   N/A
*/
void LinuxGLContext::destroyGLX() {
    if( !m_glxInitialized )
        return;

    //Note: GLX doesn't own the display, it just holds a reference to it
    m_glxInitialized = false;
    m_bestFBC        = nullptr;
    m_display        = nullptr;

    glXCreateContextAttribsARB = nullptr;
}

/*
LinuxGLContext::getIdealVisualInfo
-----------------------

Description:
    Returns an XVisualInfo detailing the ideal format for an X11 window in the given display to use.
    
    NOTE: You must call XFree() to dispose of the returned XVisualInfo once you're done using it.
Arguments:
    display:                The X11 display.

Returns:
    XVisualInfo*:           Info about the ideal visual to use in the given Display.

Throws:
    GraphicsException:      If GLX was already initialized and the given display was different.
*/
XVisualInfo* LinuxGLContext::getIdealVisualInfo( Display* display ) {
    //Need to init GLX first to find the ideal framebuffer config
    initGLX( display );

    //Now that we've found our ideal framebuffer config, we can grab the Visual:    
    XVisualInfo* vi = glXGetVisualFromFBConfig( m_display, m_bestFBC );
    if( vi == nullptr )
        throw GraphicsException( "Invalid framebuffer config." );

    return vi;
}

//Windowless init currently isn't supported
void LinuxGLContext::init() {
    throw NotImplementedException();
}

/*
LinuxGLContext::init
-----------------------

Description:
    Initialize the context with the given window.
    
    Initializes GLX if it hasn't been already.
    If GLX has been initialized, verifies that the given window belongs to the same Display.

Arguments:
    display:                The X11 display to initialize GLX with.

Returns:
    void:                   N/A

Throws:
    GraphicsException:      If display is different from the display that was used to initialize GLX previously.
*/
void LinuxGLContext::init( const Window& window ) {
    //Ensure GLX is initialized, and that the window is using the same Display as us.
    initGLX( window.getDisplay() );
    ++m_contextCount;

    m_window  = window.getHandle();

    //NOTE: When context creation fails, glXCreateContextAttribsARB and glXCreateNewContext return nullptr.
    //However, in addition to that, the code below can generate X11 errors. We need to ensure these errors are handled, so we capture X11 errors here with xerrBegin() and xerrEnd().
    //You'll see calls below to XSync; these are done before xerrExists() checks to ensure that any generated errors have been processed.
    //glxCreateNewContext can generate GLXBadContext, GLXBadFBConfig, BadMatch, BadAlloc, and BadValue errors (https://www.opengl.org/sdk/docs/man2/xhtml/glXCreateNewContext.xml).
    //glxCreateContextAttribsARB can generate all of those as well as GLXBadProfileARB (https://www.opengl.org/registry/specs/ARB/glx_create_context.txt)
    xerrBegin();
    //We support GLX_ARB_create_context!
    if( glXCreateContextAttribsARB != nullptr ) {
        //Request a GL 3.0 or higher context
        //TEMP: Hardcoded. I'll allow for specifying the version later.
        int context_attribs[] = {
            GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
            GLX_CONTEXT_MINOR_VERSION_ARB, 0,
            //GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
            None
        };

        m_context = glXCreateContextAttribsARB( m_display, m_bestFBC, 0, True, context_attribs );

        //Couldn't create a GL 3.0+ context? Lets try GL 1.0+ instead.
        XSync( m_display, False );
        if( m_context == nullptr || xerrExists() ) {
            context_attribs[1] = 1;
            context_attribs[3] = 0;
            m_context = glXCreateContextAttribsARB( m_display, m_bestFBC, 0, True, context_attribs );
        }
    //GLX_ARB_create_context not supported. Falling back to old-style context creation...
    } else {
        m_context = glXCreateNewContext( m_display, m_bestFBC, GLX_RGBA_TYPE, nullptr, True );
        XSync( m_display, False );
    }
    XSync( m_display, False );
    xerrEnd();

    if( m_context == nullptr || xerrExists() )
        throw GraphicsException( "Context creation failed." );

    //Ensure that we've created a direct rendering context.
    //Direct contexts are preferred as they sends commands directly to the rendering system.
    //Indirect contexts send commands through the X server, which is much, much slower.
    //NOTE: GLXBadContext can be generated by this.
    if( !glXIsDirect( m_display, m_context ) )
        throw GraphicsException( "Created context was not a direct context." );

    //Hurrah! Context successfully initialized!
}

/*
LinuxGLContext::destroy
-----------------------

Description:
    Destroys the context.
    If the context is current, the current context is cleared.
    Additionally, if this is the last context, GLX is destroyed.

Arguments:
    display:    The X11 display to initialize GLX with.

Returns:
    void:       N/A

Throws:
    XException: Thrown if the context could not be destroyed.
*/
void LinuxGLContext::destroy() {
    destroyContext();
    destroyFinish();
}

void LinuxGLContext::destroyContext() {
    if( m_context != nullptr ) {
        if( glXGetCurrentContext() == m_context )
            glXMakeCurrent( m_display, 0, nullptr );

        xerrBegin();
        glXDestroyContext( m_display, m_context );
        xerrEnd();
        if( xerrExists() )
            throw xerrGet();

        m_context = nullptr;
    }
}

void LinuxGLContext::destroyFinish() {
    if( m_window != 0 )
        m_window = 0;

    if( --m_contextCount == 0 )
        destroyGLX();
}

void LinuxGLContext::setVSync( const bool vsync ) {
}

void LinuxGLContext::swapBuffers() {
    glXSwapBuffers( m_display, m_window );
}

void LinuxGLContext::begin() {
    glXMakeCurrent( m_display, m_window, m_context );
}

void LinuxGLContext::end() {
    glXMakeCurrent( m_display, 0, nullptr );
}

}
}