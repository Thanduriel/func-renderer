cmake_minimum_required (VERSION 3.1)
project (worldRenderer)
set (CMAKE_CXX_STANDARD 14)

file(GLOB worldRenderer_SRC
    "src/*.hpp"
    "src/*.cpp"
)

add_executable(worldRenderer ${worldRenderer_SRC})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_subdirectory ("dependencies/glfw")
include_directories("dependencies/glfw/include")
target_link_libraries (worldRenderer glfw)

add_subdirectory ("dependencies/glew/build/cmake")
include_directories("dependencies/glew/include")
target_link_libraries (worldRenderer glew_s) # use static version

find_package(OpenGL REQUIRED)
target_link_libraries(worldRenderer ${OPENGL_gl_LIBRARY})


include_directories("dependencies/glm/glm")