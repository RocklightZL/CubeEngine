project "Cube"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/"..outputdir.."/%{prj.name}")
    objdir ("%{wks.location}/bin-obj/"..outputdir.."/%{prj.name}")

    files {
        "src/**.h",
        "src/**.cpp",
        "external/glad/src/glad.c"
    }

    includedirs{
        "src",
        "external/spdlog-1.15.3/include",
        "external/glfw-3.4/include",
        "external/glad/include"
    }

    links{
        "external/glfw-3.4/lib-vc2022/glfw3.lib"
    }

    filter "configurations:Debug"
        defines "CB_DEBUG"
        runtime "Debug"
        symbols "on"
    filter "configurations:Release"
        defines "CB_RELEASE"
        runtime "Release"
        optimize "on"
    filter "configurations:Dist"
        defines "CB_DIST"
        runtime "Release"
        optimize "on"

    filter "files:external/**"
        pchheader ""
        
    filter "toolset:msc"
        pchheader "pch.h"
        pchsource "src/pch.cpp"