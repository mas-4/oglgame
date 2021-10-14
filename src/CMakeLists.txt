set(CXX_SRCS
    "main.cpp")

add_executable(oglgame main.cpp)
target_include_directories(oglgame PUBLIC
    ${glfw_dir}/include)
target_link_libraries(oglgame glfw glew)
