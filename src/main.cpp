#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <csignal>
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>
#include <iostream>

#include "Assert.h"
#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

#define BASIC_SHADER "../res/shaders/basic.shader"
#define BASIC_TEXTURE "../res/textures/avatar.jpg"

#define RES_X 960
#define RES_Y 540

GLFWwindow* init_graphics_system()
{
  std::cout << "Starting System..." << std::endl;
  GLFWwindow* window;
  if (!glfwInit())
  {
    std::cout << "Failed to initialize glfw" << std::endl;
    raise(SIGTRAP);
  }

  /* specify ogl profile */
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(RES_X, RES_Y, "OpenGL", nullptr, nullptr);
  if (!window)
  {
    glfwTerminate();
    std::cout << "Opening window failed." << std::endl;
    raise(SIGTRAP);
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  if (glewInit() != GLEW_OK)
  {
    std::cout << "glewInit failed" << std::endl;
    raise(SIGTRAP);
  }

  std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

  // Enable blending for transparency
  GLCall(glEnable(GL_BLEND));
  GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  std::cout << "Initialization complete..." << std::endl;
  return window;
}

int main()
{
  GLFWwindow* window = init_graphics_system();
  /* vertices of the triangle */
  // clang-format off
  float positions[] = {
      100.0f, 100.0f, 0.0f, 0.0f,
      200.0f, 100.0f, 1.0f, 0.0f,
      200.0f, 200.0f, 1.0f, 1.0f,
      100.0f, 200.0f, 0.0f, 1.0f
  };
  unsigned int indices[] {
      0, 1, 2,
      2, 3, 0
  };
  // clang-format on

  VertexBuffer vb(positions, 4 * 4 * sizeof(float));

  VertexArray va;
  VertexBufferLayout layout;
  layout.Push<float>(2);
  layout.Push<float>(2);
  va.AddBuffer(vb, layout);
  IndexBuffer ib(indices, 6);

  // normalized projection against the screen
  glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
  // camera shifted right 100 px
  glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));

  Shader shader(BASIC_SHADER);
  shader.Bind();
  shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
  shader.SetUniform1i("u_Texture", 0);

  Texture texture(BASIC_TEXTURE);
  texture.Bind(); // default slot 0

  /* unbind everything */
  va.Unbind();
  vb.Unbind();
  ib.Unbind();
  shader.Unbind();

  Renderer renderer;
  ImGui::CreateContext();
  ImGui_ImplGlfwGL3_Init(window, true);
  ImGui::StyleColorsDark();
  glm::vec3 translation(200, 200, 0);
  //bool show_demo_window = true;
  //bool show_another_window = false;
  //ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  /* for my uniform */
  float r = 0.0f;
  float increment = 0.05f;

  std::cout << "Starting loop..." << std::endl;
  while (!glfwWindowShouldClose(window))
  {

    renderer.Clear();

    ImGui_ImplGlfwGL3_NewFrame();

    // model moved up and left 200 px at the perspective
    glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
    glm::mat4 mvp = proj * view * model;

    shader.Bind();
    shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
    shader.SetUniformMat4f("u_MVP", mvp);

    renderer.Draw(va, ib, shader);

    /* increment/decrement the red value for the uniform */
    if (r > 1.0f)
      increment = -0.05f;
    else if (r < 0.0f)
      increment = 0.05f;
    r += increment;
    {
      ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 960.0f);
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
          1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

    ImGui::Render();
    ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  std::cout << "Exiting..." << std::endl;
  ImGui_ImplGlfwGL3_Shutdown();
  ImGui::DestroyContext();
  glfwTerminate();
  return 0;
}
