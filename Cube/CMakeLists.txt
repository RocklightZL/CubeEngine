cmake_minimum_required(VERSION 3.15)
project(Cube VERSION 1.0.0 LANGUAGES CXX)

add_subdirectory(external/glad)
add_subdirectory(external/stb_image)

add_library(Cube STATIC)

file(GLOB_RECURSE CUBE_SRC
    src/*.cpp
    src/*.h
)
target_sources(Cube PRIVATE ${CUBE_SRC})

target_precompile_headers(Cube PRIVATE src/pch.h)

target_include_directories(Cube PUBLIC
    src
    external/spdlog-1.15.3/include
    external/glfw-3.4/include
    external/glad/include
    external/glm
    external/stb_image
    external/nlohmann-json
    external/freetype/include
)

target_link_libraries(Cube PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw-3.4/lib-vc2022/glfw3.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/external/freetype/freetype.lib
    glad
    stb_image
)

target_compile_definitions(Cube PRIVATE
    $<$<CONFIG:Debug>:CB_DEBUG>
    $<$<CONFIG:Release>:CB_RELEASE>
)

if(MSVC)
    target_compile_options(Cube PRIVATE /GR-)
else()
    target_compile_options(Cube PRIVATE -fno-rtti)
endif()
