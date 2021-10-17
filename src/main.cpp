#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <csignal>
#include <iostream>

#include "Assert.h"
#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

#define BASIC_SHADER "../res/shaders/basic.shader"
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

  Shader shader(BASIC_SHADER);
  shader.Bind();
  shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

  /* unbind everything */
  va.Unbind();
  vb.Unbind();
  ib.Unbind();
  shader.Unbind();

  Renderer renderer;

  /* for my uniform */
  float r = 0.0f;
  float increment = 0.05f;

  std::cout << "Starting loop..." << std::endl;
  while (!glfwWindowShouldClose(window))
  {

    renderer.Clear();

    shader.Bind();
    shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

    renderer.Draw(va, ib, shader);

    /* increment/decrement the red value for the uniform */
    if (r > 1.0f)
      increment = -0.05f;
    else if (r < 0.0f)
      increment = 0.05f;
    r += increment;

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  std::cout << "Exiting..." << std::endl;
  glfwTerminate();
  return 0;
}
