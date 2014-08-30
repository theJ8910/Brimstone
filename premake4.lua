solution( "Brimstone" )
    configurations( { "debug", "release" } )
    platforms( { "x32", "x64" } )

    project( "Brimstone" )
        kind( "StaticLib" )
        language( "C++" )
        files( {
            "src/**.cpp",
            "src/**.hpp",
            "include/brimstone/**.hpp"
        } )

        includedirs( { "include" } )
        targetdir( "lib" )

        --We need to specify that we want to use the C++11 standard when compiling with G++
        configuration( "gmake" )
            buildoptions( "-std=c++11" )

        --Define a preprocessor symbol indicating what OS we're compiling for
        configuration( "windows" )
            defines( { "BS_BUILD_WINDOWS" } )
        configuration( "linux" )
            defines( { "BS_BUILD_LINUX" } )

        --Define BS_BUILD_64BIT if we're compiling for the x64 architecture
        configuration( "x64" )
            defines( { "BS_BUILD_64BIT" } )

        --Exclude Windows files when compiling for non-window OSes
        for k,v in pairs( { "bsd", "linux", "macosx", "solaris" } ) do
        configuration( v )
            excludes( {
                "src/windows/**.cpp",
                "include/brimstone/windows/**.hpp"
            } )
        end

        --Exclude Linux files when compiling for non-linux OSes
        for k,v in pairs( { "bsd", "macosx", "solaris", "windows" } ) do
        configuration( v )
            excludes( {
                "src/linux/**.cpp",
                "include/brimstone/linux/**.hpp"
            } )
        end

        --Debug builds enable a bunch of safeguards
        configuration( "debug" )
            defines( {
                "BS_ZERO",
                "BS_CHECK_NULLPTR",
                "BS_CHECK_SIZE",
                "BS_CHECK_INDEX",
                "BS_CHECK_DIVBYZERO",
                "BS_CHECK_DOMAIN"
            } )

        --Release builds optimize for speed
        configuration( "release" )
            flags( { "OptimizeSpeed" } )

        --The output library name is different depending on the
        --architecture and whether or not this is the debug/release version
        configuration( { "debug", "x32" } )
            targetname( "Brimstone_x86d" )

        configuration( { "release", "x32" } )
            targetname( "Brimstone_x86" )

        configuration( { "debug", "x64" } )
            targetname( "Brimstone_x64d" )

        configuration( { "release", "x64" } )
            targetname( "Brimstone_x64" )

    project( "UnitTests")
        kind( "ConsoleApp" )
        language( "C++" )
        files( {
            "tests/src/**.cpp",
            "tests/src/**.hpp"
        } )

        includedirs( { "include" } )
        targetdir( "tests/bin" )
        libdirs { "lib" }

        configuration( "gmake" )
            buildoptions( "-std=c++11" )
        
        configuration( "windows" )
            defines( { "BS_BUILD_WINDOWS" } )
            defines( { "UT_BUILD_WINDOWS" } )
        configuration( "linux" )
            defines( { "BS_BUILD_LINUX" } )
            defines( { "UT_BUILD_LINUX" } )

        configuration( "x64" )
            defines( { "BS_BUILD_64BIT" } )

        configuration( "debug" )
            defines( {
                "BS_ZERO",
                "BS_CHECK_NULLPTR",
                "BS_CHECK_SIZE",
                "BS_CHECK_INDEX",
                "BS_CHECK_DIVBYZERO",
                "BS_CHECK_DOMAIN"
            } )

        configuration( "release" )
            flags( { "OptimizeSpeed" } )

        configuration( { "debug", "x32" } )
            targetname( "UnitTests_x86d" )
            links( { "Brimstone_x86d" } )

        configuration( { "release", "x32" } )
            targetname( "UnitTests_x86" )
            links( { "Brimstone_x86" } )

        configuration( { "debug", "x64" } )
            targetname( "UnitTests_x64d" )
            links( { "Brimstone_x64d" } )

        configuration( { "release", "x64" } )
            targetname( "UnitTests_x64" )
            links( { "Brimstone_x64" } )