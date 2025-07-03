project "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off" 

    targetdir ("%{wks.location}/bin/"..outputdir.."/%{prj.name}")
    objdir ("%{wks.location}/bin-obj/"..outputdir.."/%{prj.name}")

    files {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs {
        "%{wks.location}/Cube/src"
    }

    links {
        "Cube"
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