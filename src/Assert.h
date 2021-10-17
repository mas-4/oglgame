/*
 * There's a circular reference if this stuff is included in Renderer.h like
 * originally designed. The Cherno solves it by forward declaring the class
 * VertexBufferLayout and then including the header file in the cpp file. I
 * realized, at least for my own code, there were a lot of messy includes so I
 * just did a refactor and put this code here.
 */
#pragma once
#include <GL/glew.h>
#include <iostream>
#include <csignal>

#define ASSERT(x)                                                              \
  if (!(x)) raise(SIGTRAP);

#define GLCall(x)                                                              \
  GLClearError();                                                              \
  x;                                                                           \
  ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);