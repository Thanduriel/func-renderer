cmake_minimum_required (VERSION 3.1)
project (worldRenderer)
set (CMAKE_CXX_STANDARD 14)

file(GLOB worldRenderer_SRC
    "src/*.hpp"
    "src/*.cpp"
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory ("dependencies/glfw-3.2.1")
include_directories("dependencies/glfw-3.2.1/include")

add_subdirectory ("dependencies/glew-2.0.0/build/cmake")
include_directories("dependencies/glew-2.0.0/include")

include_directories("dependencies/glm/glm")

add_executable(worldRenderer ${worldRenderer_SRC})

target_link_libraries (worldRenderer glfw)
target_link_libraries (worldRenderer glew)

find_package(OpenGL REQUIRED)
target_link_libraries(worldRenderer ${OPENGL_gl_LIBRARY})