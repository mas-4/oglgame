#include "Renderer.h"
#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& filepath)
    : m_FilePath(filepath)
    , m_RendererID(0)
{
  ShaderProgramSource source = ParseShader();
  m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader() { GLCall(glDeleteProgram(m_RendererID)); }

void Shader::Bind() const
{
  GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
  GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
  GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
  GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
  if (location == -1)
    std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
  return location;
}

unsigned int Shader::CreateShader(
    const std::string& vertex_shader, const std::string& fragment_shader)
{
  GLCall(unsigned int program = glCreateProgram());
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertex_shader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragment_shader);
  GLCall(glAttachShader(program, vs));
  GLCall(glAttachShader(program, fs));
  GLCall(glLinkProgram(program));
  GLCall(glValidateProgram(program));

  GLCall(glDeleteShader(vs));
  GLCall(glDeleteShader(fs));

  return program;
}

ShaderProgramSource Shader::ParseShader()
{
  std::ifstream stream(m_FilePath);

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
unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
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
