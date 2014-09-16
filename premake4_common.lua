function doFlags()

    configuration( {} )
        flags( { "ExtraWarnings" } )
    
    --We need to specify that we want to use the C++11 standard when compiling with G++
    --Additionally we ignore pragmas used by MSVC which are unknown to G++.
    --The -pthread option enables threading, which Brimstone makes use of
    configuration( "gmake" )
        buildoptions( {
            "-std=c++11",
            "-pthread",
            "-Wno-unknown-pragmas"
        } )
        linkoptions( {
            "-pthread",
        } )

    --Debug builds add symbols to generated libraries / executables to enable debugging
    configuration( "debug" )
        flags( { "Symbols" } )

    --Release builds optimize for speed
    configuration( "release" )
        flags( { "OptimizeSpeed" } )
end

function doBrimstoneDefines()
    --Debug builds enable a bunch of safeguards
    configuration( "debug" )
        defines( {
            "BS_BUILD_DEBUG",
            "BS_ZERO",
            "BS_CHECK_NULLPTR",
            "BS_CHECK_SIZE",
            "BS_CHECK_INDEX",
            "BS_CHECK_DIVBYZERO",
            "BS_CHECK_DOMAIN"
        } )

    --Define a preprocessor symbol indicating what OS we're compiling for
    configuration( "windows" )
        defines( { "BS_BUILD_WINDOWS" } )
    configuration( "linux" )
        defines( { "BS_BUILD_LINUX" } )

    --Define BS_BUILD_64BIT if we're compiling for the x64 architecture
    configuration( "x64" )
        defines( { "BS_BUILD_64BIT" } )
end

builds = {
    { "x32", "debug"   },
    { "x32", "release" },
    { "x64", "debug"   },
    { "x64", "release" }
}
local pfToArch = {
    x32 = "x86",
    x64 = "x64"
}
function getSuffix( build )
    return pfToArch[build[1]]..(build[2] == "debug" and "d" or "")
end