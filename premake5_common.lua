--Supported platforms
local build_platforms = { "x64", "x32" }

--Supported configurations
local build_configurations = { "release", "debug" }

--[[
Table of all supported builds, one for each combination of build_platforms and build_configurations.
Each build is a table with four members: platform, configuration, filter, suffix.
platform and configuration are the premake platform and configurations for this build.
filter can be passed to premake's filter() function to match this specific build.
suffix is the filename suffix that should be used for this build.
]]--
local builds = {}
function doBuilds()
    --Translation table from premake's platform strings to CPU architecture strings
    local pfToArch = {
        x64 = "x86-64",
        x32 = "x86"
    }

    --Create a list of builds. Builds are a cartesian product of platforms and configurations.
    for _,x in ipairs( build_platforms ) do
        for _,y in ipairs( build_configurations ) do
            table.insert( builds, {
                platform = x,
                configuration = y,
                filter = {
                    "architecture:"..x,
                    "configurations:"..y
                },
                suffix = pfToArch[x]..(y == "debug" and "d" or "")
            } )
        end
    end

    --Set platforms and configurations.
    platforms( build_platforms )
    configurations( build_configurations )
end

function doFlags()
    --Use C++20 and give extra warnings in all builds
    filter( {} )
        cppdialect( "C++20" )
        warnings( "Extra" )

    --The x32 platform uses the x86 architecture
    filter( "platforms:x32" )
        architecture( "x86" )

    --The x64 platform uses the x86-64 architecture
    filter( "platforms:x64" )
        architecture( "x86_64" )

    --Enable debugging symbols in debug builds
    filter( "configurations:debug" )
        symbols( "On" )

    --Optimize for speed in release builds
    filter( "configurations:release" )
        optimize( "Speed" )

    --[[
    When compiling with G++:
    * -pthread:             Enable multithreading
    * -Wno-unknown-pragmas: Don't generate warnings for unknown pragmas (e.g. pragmas compatible with MSVC but not other compilers)
    ]]--
    filter( "toolset:gcc" )
        buildoptions( {
            "-pthread",
            "-Wno-unknown-pragmas"
        } )
        linkoptions( {
            "-pthread"
        } )
end

function doBrimstoneLinks()
    --[[
    Note: the order that we link here is important.
    Brimstone links to LuaJIT and X11,
    and LuaJIT links to dl.
    Dependent libraries need to come BEFORE the libraries they depend on.
    ]]--

    --Each build of this project links to its respective build of the Brimstone library.
    for _,build in ipairs( builds ) do
        filter( build.filter )
            links( "Brimstone_"..build.suffix )
    end

    --[[
    We link to a different version of LuaJIT depending on the platform we're compiling for
    Honestly it would probably be better if we just changed the library directory instead,
    but this works for now
    ]]--
    filter( "platforms:x32" )
        links( { "luajit-5.1_x86", "gll_x86", "GL" } )
    filter( "platforms:x64" )
        links( { "luajit-5.1_x64", "gll_x86-64", "GL" } )

    --[[
    All configurations link to dl and X11.
    Whether we link to the 32-bit or 64-bit version is determined automatically
    based on the library directory that premake sets.
    Since I'm developing on a 64-bit OS, I needed to download the 32-bit versions of the library
    before it would allow me to build the 32-bit versions of the game.
    ]]--
    filter( {} )
        links( { "dl", "X11", "png" } )
end

function doBrimstoneDefines()
    --Use OpenGL for the graphics implementation
    filter( {} )
        defines( "BS_BUILD_OPENGL" )

    --Debug builds enable a bunch of safeguards
    filter( "configurations:debug" )
        defines( {
            "BS_BUILD_DEBUG",
            "BS_ZERO",
            "BS_CHECK_NULLPTR",
            "BS_CHECK_SIZE",
            "BS_CHECK_INDEX",
            "BS_CHECK_DIVBYZERO",
            "BS_CHECK_DOMAIN",
        } )

    --Define a preprocessor symbol indicating what OS we're compiling for
    filter( "system:windows" )
        defines( { "BS_BUILD_WINDOWS" } )
    filter( "system:linux" )
        defines( { "BS_BUILD_LINUX" } )

    --Define BS_BUILD_64BIT if we're compiling for the x86-64 architecture
    filter( "architecture:x64" )
        defines( { "BS_BUILD_64BIT" } )
end

--Use the appropriate suffix for each build
function doSuffixes()
    for _, build in ipairs( builds ) do
        filter( build.filter )
            targetsuffix( "_"..build.suffix )
    end
end
