dofile( "premake5_common.lua" )
solution( "Brimstone" )
    doBuilds()

    project( "Brimstone" )
        kind( "StaticLib" )
        language( "C++" )
        files( {
            "src/brimstone/**.cpp",
            "src/brimstone/**.hpp",
            "include/brimstone/**.hpp"
        } )

        includedirs( "include" )
        targetdir( "lib" )

        --Exclude Windows files when compiling for non-window OSes
        filter( "system:not windows" )
            excludes( {
                "src/brimstone/windows/**.cpp",
                "include/brimstone/windows/**.hpp",
                "src/brimstone/opengl/WindowsGLContext.cpp",
                "src/brimstone/opengl/WindowsGLContext.hpp"
            } )

        --Exclude Linux files when compiling for non-linux OSes
        filter( "system:not linux" )
            excludes( {
                "src/brimstone/linux/**.cpp",
                "include/brimstone/linux/**.hpp",
                "src/brimstone/opengl/LinuxGLContext.cpp",
                "src/brimstone/opengl/LinuxGLContext.hpp"
            } )

        doFlags()
        doBrimstoneDefines()

        --The output library name is different depending on the
        --architecture and whether or not this is the debug/release version
        doSuffixes()

    project( "UnitTests")
        kind( "ConsoleApp" )
        language( "C++" )
        files( {
            "src/tests/**.cpp",
            "src/tests/**.hpp"
        } )

        includedirs( "include" )
        targetdir( "bin" )
        libdirs( "lib" )

        doFlags()
        doBrimstoneDefines()
        doBrimstoneLinks()
        doSuffixes()

        --UnitTests-specific defines.
        --The UnitTests framework is designed to be separate from Brimstone.
        --The upside is that it increases reusability, but the downside 
        --is that there is some redundancy between the two projects.
        filter( "system:windows" )
            defines( "UT_BUILD_WINDOWS" )
        filter( "system:linux" )
            defines( "UT_BUILD_LINUX" )

        --The output executable name and the library that UnitTests links to
        --is different depending on the architecture and whether or not this is the debug/release version
        doSuffixes()
