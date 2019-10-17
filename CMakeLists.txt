cmake_minimum_required (VERSION 3.1)
project (worldRenderer)
set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCE_FILES "src/*.hpp" "src/*.cpp")

# organize the files according to the organisation in the file system
# from: http://cmake.3232098.n2.nabble.com/Keep-folder-structure-in-Visual-Studio-Project-generated-with-CMake-td7586044.html
macro(GroupSources curdir)
   file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/${curdir} ${PROJECT_SOURCE_DIR}/${curdir}/*)
   foreach(child ${children})
          if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${curdir}/${child})
                  GroupSources(${curdir}/${child})
          else()
                  string(REPLACE "/" "\\" groupname ${curdir})
            source_group(${groupname} FILES ${PROJECT_SOURCE_DIR}/${curdir}/${child})
          endif()
   endforeach()
endmacro()

GroupSources(src)

include_directories("src")
add_executable(worldRenderer ${SOURCE_FILES})

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