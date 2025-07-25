project "CubeEditor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/"..outputdir.."/%{prj.name}")
    objdir ("%{wks.location}/bin-obj/"..outputdir.."/%{prj.name}")

    files{
        "src/**.cpp",
        "src/**.h",
        "external/imgui/imgui/**.h",
        "external/imgui/imgui/**.cpp"
    }

    includedirs {
        "%{wks.location}/Cube/src",
        "%{wks.location}/Cube/external/spdlog-1.15.3/include",
        "%{wks.location}/Cube/external/glfw-3.4/include",
        "%{wks.location}/Cube/external/glad/include",
        "%{wks.location}/Cube/external/glm",
        "%{wks.location}/Cube/external/nlohmann-json",
        "external/imgui"
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