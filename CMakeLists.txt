cmake_minimum_required(VERSION 3.20)

project(ccc VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/vendor/imgui)

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
)

set(IMGUI_BACKENDS
    ${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

add_executable(ccc
    ${SOURCES}
    ${IMGUI_SOURCES}
    ${IMGUI_BACKENDS}
)

target_include_directories(ccc PRIVATE
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${SDL2_INCLUDE_DIRS}
)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

target_link_libraries(ccc PRIVATE
    ${SDL2_LIBRARIES}
    OpenGL::GL
)

if(APPLE)
    target_link_libraries(ccc PRIVATE "-framework OpenGL")
endif()
