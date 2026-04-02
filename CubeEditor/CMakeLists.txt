cmake_minimum_required(VERSION 3.15)
project(CubeEditor VERSION 1.0.0 LANGUAGES CXX)

add_subdirectory(external/imgui/imgui)

add_executable(CubeEditor)

file(GLOB_RECURSE EDITOR_SRC
    src/*.cpp
    src/*.h
)
target_sources(CubeEditor PRIVATE ${EDITOR_SRC})

target_include_directories(CubeEditor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(CubeEditor PRIVATE
    Cube
    imgui
)

target_compile_definitions(CubeEditor PRIVATE
    $<$<CONFIG:Debug>:CB_DEBUG>
    $<$<CONFIG:Release>:CB_RELEASE>
)