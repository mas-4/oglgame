#include "Renderer.h"

#include <csignal>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

#define BASIC_SHADER "../res/shaders/basic.shader"

struct ShaderProgramSource
{
  std::string VertexSource;
  std::string FragmentSource;
};

static ShaderProgramSource parse_shader(const std::string& file_path)
{
  std::ifstream stream(file_path);

  enum class ShaderType
  {
    NONE = -1,
    VERTEX = 0,
    FRAGMENT = 1
  };

  std::string line;
  std::stringstream ss[2];
  ShaderType type = ShaderType::NONE;
  while (getline(stream, line))
  {
    if (line.find("#shader") != std::string::npos)
    {
      if (line.find("vertex") != std::string::npos)
        type = ShaderType::VERTEX;
      else if (line.find("fragment") != std::string::npos)
        type = ShaderType::FRAGMENT;
    }
    else
      ss[(int)type] << line << std::endl;
  }

  return { ss[0].str(), ss[1].str() };
}

/* Compiles a shader, `source`,  of type `type` and returns the id */
static unsigned int compile_shader(unsigned int type, const std::string& source)
{
  GLCall(unsigned int id = glCreateShader(type));
  const char* src = source.c_str();
  GLCall(glShaderSource(id, 1, &src, 0));
  GLCall(glCompileShader(id));

  int result;
  GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
  if (result == GL_FALSE)
  {
    int length;
    GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
    char* message = (char*)alloca(length * sizeof(char));
    GLCall(glGetShaderInfoLog(id, length, &length, message));
    std::cerr << "Failed to compile shader! "
              << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
              << std::endl;
    std::cerr << "Source:\n"
              << "=======\n"
              << source << std::endl;
    std::cerr << message << std::endl;
    GLCall(glDeleteShader(id));
    return -1;
  }

  return id;
}

/* creates a shader with `vertex_shader` and `fragment_shader` and ties it to
 * the program. Returns program id */
static unsigned int create_shader(
    const std::string& vertex_shader, const std::string& fragment_shader)
{
  unsigned int program = glCreateProgram();
  unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
  unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);
  GLCall(glAttachShader(program, vs));
  GLCall(glAttachShader(program, fs));
  GLCall(glLinkProgram(program));
  GLCall(glValidateProgram(program));

  GLCall(glDeleteShader(vs));
  GLCall(glDeleteShader(fs));

  return program;
}

int main(void)
{
  std::cout << "Starting..." << std::endl;

  GLFWwindow* window;

  /* Initialize glfw */
  if (!glfwInit()) return -1;

  /* specify ogl profile */
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(640, 480, "OGL", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    std::cout << "Opening window failed." << std::endl;
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  /* Initialize glew */
  if (glewInit() != GLEW_OK)
  {
    std::cout << "glewInit failed" << std::endl;
    return -1;
  }

  std::cout << glGetString(GL_VERSION) << std::endl;

  /* vertices of the triangle */
  // clang-format off
  float positions[] = {
      -0.5f, -0.5f,
       0.5f, -0.5f,
       0.5f,  0.5f,
      -0.5f,  0.5f,
  };
  unsigned int indices[] {
      0, 1, 2,
      2, 3, 0
  };
  // clang-format on

  VertexBuffer vb(positions, 4 * 2 * sizeof(float));

  VertexArray va;
  VertexBufferLayout layout;
  layout.Push<float>(2);
  va.AddBuffer(vb, layout);

  IndexBuffer ib(indices, 6);

  // set up the shader
  ShaderProgramSource source = parse_shader(BASIC_SHADER);
  unsigned int shader
      = create_shader(source.VertexSource, source.FragmentSource);
  GLCall(glUseProgram(shader));

  GLCall(int location = glGetUniformLocation(shader, "u_color"));
  ASSERT(location != -1);
  GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));

  /* unbind everything */
  va.Unbind();
  GLCall(glUseProgram(0));
  vb.Unbind();
  ib.Unbind();

  /*
  GLCall(glBindVertexArray(0));
  GLCall(glUseProgram(0));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
  */

  /* for my uniform */
  float r = 0.0f;
  float increment = 0.05f;

  std::cout << "Starting loop..." << std::endl;
  while (!glfwWindowShouldClose(window))
  {
    GLCall(glClear(GL_COLOR_BUFFER_BIT));

    /* set the shader, specify the uniform */
    GLCall(glUseProgram(shader));
    GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

    /* bind vertices, indices buffers */
    va.Bind();
    ib.Bind();

    /* draw */
    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

    /* increment/decrement the red value for the uniform */
    if (r > 1.0f)
      increment = -0.05f;
    else if (r < 0.0f)
      increment = 0.05f;
    r += increment;

    GLCall(glfwSwapBuffers(window));
    GLCall(glfwPollEvents());
  }

  std::cout << "Exiting..." << std::endl;
  glfwTerminate();
  GLCall(glDeleteProgram(shader));
  return 0;
}
