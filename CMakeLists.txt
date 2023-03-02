cmake_minimum_required(VERSION 3.17)
project(glz)
set(CMAKE_C_COMPILER "clang++")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_STANDARD 20)

link_libraries(glfw3 glad GL GLU X11 png)
include_directories(vendor/include)
link_directories(vendor)

# file(GLOB LIBGLZ_DEV_SRC_DIR "src/*.cc")
# add_library(glz-dev SHARED
#     ${LIBGLZ_DEV_SRC_DIR})

add_executable(exe main.cc)
# configure_file(01-more-shapes/fragment_shader.glsl  ${CMAKE_BINARY_DIR}/01-more-shapes-dir/fragment_shader.glsl)
# configure_file(01-more-shapes/vertex_shader.glsl  ${CMAKE_BINARY_DIR}/01-more-shapes-dir/vertex_shader.glsl)