cmake_minimum_required(VERSION 3.16)

project(cpp_ray_tracing LANGUAGES CXX)


include("${CMAKE_SOURCE_DIR}/cmake/PreventInSourceBuilds.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/StandardProjectSettings.cmake")

include_directories(include)

add_subdirectory(src)
add_subdirectory(app)
