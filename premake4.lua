dofile( "premake4_common.lua" )
solution( "Brimstone" )
    configurations( { "debug", "release" } )
    platforms( { "x32", "x64" } )

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
        configuration( "not windows" )
            excludes( {
                "src/brimstone/windows/**.cpp",
                "include/brimstone/windows/**.hpp"
            } )

        --Exclude Linux files when compiling for non-linux OSes
        configuration( "not linux" )
            excludes( {
                "src/brimstone/linux/**.cpp",
                "include/brimstone/linux/**.hpp"
            } )

        doFlags()
        doBrimstoneDefines()

        --The output library name is different depending on the
        --architecture and whether or not this is the debug/release version
        for _,build in pairs( builds ) do
            configuration( build )
                targetname( "Brimstone_"..getSuffix( build ) )
        end

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

        --UnitTests-specific defines.
        --The UnitTests framework is designed to be separate from Brimstone.
        --The upside is that it increases reusability, but the downside 
        --is that there is some redundancy between the two projects.
        configuration( "windows" )
            defines( "UT_BUILD_WINDOWS" )
        configuration( "linux" )
            defines( "UT_BUILD_LINUX" )

        --The output executable name and the library that UnitTests links to
        --is different depending on the architecture and whether or not this is the debug/release version
        for _,build in pairs( builds ) do
            local sx = getSuffix( build );
            configuration( build )
                targetname( "UnitTests_"..sx )
                links( "Brimstone_"..sx )
        end