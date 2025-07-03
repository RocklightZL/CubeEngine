workspace "CubeEngine"
    architecture "x64"
    startproject "Sandbox"

    configurations{
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Cube"
include "Sandbox"
