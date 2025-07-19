workspace "CubeEngine"
    architecture "x64"
    startproject "CubeEditor"

    configurations{
        "Debug",
        "Release",
        "Dist"
    }
    characterset "Unicode"
    buildoptions { "/utf-8" }
    

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Cube"
include "Sandbox"
include "CubeEditor"