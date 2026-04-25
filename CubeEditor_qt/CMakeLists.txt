cmake_minimum_required(VERSION 3.15)
project(CubeEditor_qt)

set(CMAKE_PREFIX_PATH "D:/GitHubRepo/Qt_6.10.3-static-Release_msvc2022_64_RP_MD" ${CMAKE_PREFIX_PATH})

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
set(CMAKE_AUTOMOC ON)

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

target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:CB_DEBUG>
    $<$<CONFIG:Release>:CB_RELEASE>
)
