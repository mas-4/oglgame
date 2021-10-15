#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

/* Compiles a shader, `source`,  of type `type` and returns the id */
static unsigned int compile_shader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile shader! " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return -1;
    }

    return id;
}

/* creates a shader with `vertex_shader` and `fragment_shader` and ties it to the program. Returns program id */
static unsigned int create_shader(const std::string& vertex_shader, const std::string& fragment_shader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
    unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    std::cout << "Starting..." << std::endl;

    GLFWwindow* window;

    /* Initialize glfw */
    if (!glfwInit())
        return -1;
    window = glfwCreateWindow(640, 480, "OGL", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    /* Initialize glew */
    if (glewInit() != GLEW_OK)
        std::cout << "glewInit failed" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    // vertices of the triangle
    float positions[6] = {
        -0.5f, -0.5f,
        0.0f, 0.5f,
        0.5f, -0.5f
    };

    // initialize my vertex buffer
    unsigned int buffer_id;
    glGenBuffers(1, &buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, positions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    std::string vertex_shader = "#version 330 core\n"
                                "\n"
                                "layout(location = 0) in vec4 position;\n"
                                "\n"
                                "void main()\n"
                                "{\n"
                                "   gl_Position = position;\n"
                                "}\n";

    std::string fragment_shader = "#version 330 core\n"
                                  "\n"
                                  "layout(location = 0) out vec4 color;\n"
                                  "\n"
                                  "void main()\n"
                                  "{\n"
                                  "   color = vec4(0.0, 1.0, 0.0, 1.0);\n"
                                  "}\n";

    unsigned int shader = create_shader(vertex_shader, fragment_shader);
    glUseProgram(shader);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
