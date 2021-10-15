cmake_minimum_required(VERSION 3.12...3.21)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(
    oglgame VERSION 1.0
    DESCRIPTION "A little diddy using OpenGL"
    LANGUAGES CXX)

# flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILER_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

# directories
set(deps ${CMAKE_SOURCE_DIR}/dependencies)
set(src_dir ${CMAKE_SOURCE_DIR}/src)

# dependencies
set(glfw_dir ${deps}/GLFW/)
set(glew_dir ${deps}/GLEW/)

# Disabling some glfw files
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# including subdirectories
add_subdirectory(${glfw_dir})
add_subdirectory(${glew_dir}/build/cmake)
add_subdirectory(${src_dir})

add_executable(oglgame ${src_dir}/main.cpp)
target_link_libraries(oglgame glfw glew)
target_include_directories(oglgame PUBLIC
    ${glfw_dir}/include
    ${glew_dir}/include)
