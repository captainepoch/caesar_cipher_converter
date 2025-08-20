cmake_minimum_required(VERSION 3.20)

project(ccc VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_compile_options(-Wall -Werror)

add_executable(ccc ${SOURCES})

target_include_directories(ccc
    PRIVATE
)

target_link_libraries(ccc
    PRIVATE
)
