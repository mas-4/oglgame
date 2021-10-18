cmake_minimum_required(VERSION 3.12...3.21)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(
    oglgame VERSION 1.0
    DESCRIPTION "A little diddy using OpenGL"
    LANGUAGES CXX)

# flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILER_COMMANDS ON)
set(OpenGL_GL_PREFERENCE GLVND)

# directories
set(deps ${CMAKE_SOURCE_DIR}/dependencies)
set(src_dir ${CMAKE_SOURCE_DIR}/src)

# dependencies
set(glfw_dir ${deps}/GLFW/)
set(glew_dir ${deps}/GLEW/)
set(glm_dir ${deps}/glm_root_include/)
set(imgui_dir ${deps}/IMGUI)
set(stb_image ${src_dir}/vendor/stb_image/stb_image.cpp)

set(imgui_src
    ${imgui_dir}/imgui.cpp
    ${imgui_dir}/imgui_demo.cpp
    ${imgui_dir}/imgui_draw.cpp
    ${imgui_dir}/imgui_impl_glfw_gl3.cpp)


# Disabling some glfw files
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# including subdirectories
add_subdirectory(${glfw_dir})
add_subdirectory(${glew_dir}/build/cmake)
add_subdirectory(${src_dir})

set(TARGET_SRC
    ${stb_image}
    ${src_dir}/Assert.cpp
    ${src_dir}/IndexBuffer.cpp
    ${src_dir}/Renderer.cpp
    ${src_dir}/Shader.cpp
    ${src_dir}/Texture.cpp
    ${src_dir}/VertexArray.cpp
    ${src_dir}/VertexBuffer.cpp

    ${src_dir}/main.cpp)

add_executable(oglgame ${TARGET_SRC} ${imgui_src})
target_link_libraries(oglgame glfw glew)
target_include_directories(oglgame PUBLIC
    ${glfw_dir}/include
    ${glew_dir}/include
    ${glm_dir}
    ${imgui_dir}
    ${src_dir}/vendor)
