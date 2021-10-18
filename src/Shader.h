#pragma once

#include <GLM/glm.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

#include "Assert.h"

struct ShaderProgramSource
{
  std::string VertexSource;
  std::string FragmentSource;
};

class Shader
{
private:
  std::string m_FilePath;
  unsigned int m_RendererID;
  std::unordered_map<std::string, int> m_UniformLocationCache;

public:
  Shader(const std::string& filepath);
  ~Shader();

  void Bind() const;
  void Unbind() const;

  // Set uniforms
  void SetUniform1i(const std::string& name, int v0);
  void SetUniform1f(const std::string& name, float v0);
  void SetUniform4f(
      const std::string& name, float v0, float v1, float v2, float v3);
  void SetUniformMat4f(const std::string& name, glm::mat4& matrix);

private:
  ShaderProgramSource ParseShader();
  int GetUniformLocation(const std::string& name);
  unsigned int CreateShader(
      const std::string& vertex_shader, const std::string& fragment_shader);
  unsigned int CompileShader(unsigned int type, const std::string& source);
};