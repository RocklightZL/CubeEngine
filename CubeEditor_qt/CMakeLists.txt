cmake_minimum_required(VERSION 3.15)
project(MyQtApp)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:>")
endif()

set(CMAKE_PREFIX_PATH "D:/GitHubRepo/Qt_6.10.3-static/msvc2022_64" ${CMAKE_PREFIX_PATH})

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

file(GLOB_RECURSE EDITOR_SRC
    src/*.cpp
    src/*.h
)
add_executable(${PROJECT_NAME} ${EDITOR_SRC})

target_compile_definitions(${PROJECT_NAME} PRIVATE QT_STATIC)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Widgets
    Cube
)

qt_import_plugins(${PROJECT_NAME}
    INCLUDE Qt6::QWindowsIntegrationPlugin
)