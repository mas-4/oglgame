#include "Texture.h"

Texture::Texture(const std::string& filepath)
    : m_RendererID(0)
    , m_FilePath(filepath)
    , m_LocalBuffer(nullptr)
    , m_Width(0)
    , m_Height(0)
    , m_BPP(0)
{
  stbi_set_flip_vertically_on_load(1); // ogl has y=0 at the bottom
  m_LocalBuffer = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_BPP,
      4); // 4 = RGBA for channels

  GLCall(glGenTextures(1, &m_RendererID));
  GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

  // level=0, border=0
  GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
  GLCall(glBindTexture(GL_TEXTURE_2D, 0));

  if (m_LocalBuffer) stbi_image_free(m_LocalBuffer);
}

Texture::~Texture() { GLCall(glDeleteTextures(1, &m_RendererID)); }

void Texture::Bind(unsigned int slot /*= 0 */) const
{
  // GL_TEXTUREn is just GL_TEXTURE0 + n
  GLCall(glActiveTexture(GL_TEXTURE0 + slot));
  GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const { GLCall(glBindTexture(GL_TEXTURE_2D, 0)) }