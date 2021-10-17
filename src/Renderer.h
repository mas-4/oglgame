#pragma once

#define GLEW_STATIC
// clang-format off
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <csignal>

#define ASSERT(x)                                                              \
  if (!(x)) raise(SIGTRAP);

#define GLCall(x)                                                              \
  GLClearError();                                                              \
  x;                                                                           \
  ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);
