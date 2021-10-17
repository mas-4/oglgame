#pragma once

#include "Renderer.h"
#include <vector>

struct VertexBufferElement
{
  unsigned int type;
  unsigned int count;
  unsigned char normalized;

  static unsigned int GetSizeOfType(unsigned int type)
  {
    switch (type)
    {
      case GL_FLOAT: return 4;
      case GL_UNSIGNED_INT: return 4;
      case GL_UNSIGNED_BYTE: return 1;
    }
    ASSERT(false);
    return 0;
  }
};

class VertexBufferLayout
{
private:
  std::vector<VertexBufferElement> m_Elements;
  unsigned int m_Stride;

public:
  VertexBufferLayout()
      : m_Stride(0) {};

  /*
   * In windows world I believe static_assert(false) will only trigger if the
   * program is compiled to call the function. I.e., if there's anywhere in the
   * program that the function VertexBufferLayout::Push<T> is called (for which
   * there is not a specialization). In Linux world, at least with clang (I
   * can't speak to GCC because GCC won't acknowledge c++14 declaration that
   * explicit specialization can be declared in any scope as below; this only
   * compiles on clang), having this static_assert fails to compile. This is
   * just a theory because Jesus Christ, it won't compile, but in the tutorial
   * it does. The least I can do is add a ASSERT(false) for a runtime error.
   *
   * On the other hand, it may be another issue: not that there is no call to
   * this function, but rather that the compiler interprets any call to the
   * specializations as being calls to this function first.
   *
   * Idea for an experiment: a function that static_assert but is not called.
   *
   * Nope, verified, static_assert(false) will always prevent compilation.
   * Crazy, that would be handy.
   *
   * - Michael.
   */
  template <typename T> void Push(unsigned int count)
  {
    /* static_assert(false) */
    ASSERT(false);
  }

  template <> void Push<float>(unsigned int count)
  {
    m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
    m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT);
  }

  template <> void Push<unsigned int>(unsigned int count)
  {
    m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
    m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
  }

  template <> void Push<unsigned char>(unsigned int count)
  {
    m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
    m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
  }

  inline const std::vector<VertexBufferElement> GetElements() const
  {
    return m_Elements;
  }
  inline unsigned int GetStride() const { return m_Stride; }
};