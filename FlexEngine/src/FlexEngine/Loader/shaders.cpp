#include "pch.h"

#include "shaders.h"

#include <glad/glad.h>

namespace FlexEngine
{
  Shader::Shader()
    : m_vertex_shader(0), m_fragment_shader(0), m_shader_program(0)
  {
    FE_FLOW_BEGINSCOPE();
  }

  Shader::~Shader()
  {
    Destroy();

    FE_FLOW_ENDSCOPE();
  }

  void Shader::Destroy()
  {
    if (m_vertex_shader != 0) glDeleteShader(m_vertex_shader);
    if (m_fragment_shader != 0) glDeleteShader(m_fragment_shader);
    if (m_shader_program != 0) glDeleteProgram(m_shader_program);
  }

  Shader* Shader::SetBasePath(const std::string& base_path = "")
  {
    m_base_path = base_path;
    return this;
  }

  Shader* Shader::CreateVertexShader(const std::string& path_to_vertex_shader)
  {
    FE_FLOW_FUNCTION();

    // chain error handling
    if (this == nullptr) return nullptr;

    // warning if vertex shader already exists
    // handled by overriding the old shader
    if (m_vertex_shader != 0) {
      Log::Warning("Vertex shader already exists!");
      return nullptr;
    }

    // read vertex shader file
    std::string vertex_shader_source;
    std::ifstream ifs(m_base_path + '/' + path_to_vertex_shader);
    try
    {
      vertex_shader_source.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    }
    catch (const std::ifstream::failure& e)
    {
      std::stringstream error{};
      error << "Could not read vertex shader file!\n" << e.what() << '\n';
      Log::Error(error.str());
      return nullptr;
    }

    // create vertex shader
    m_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertex_shader_source_cstr = vertex_shader_source.c_str();
    glShaderSource(m_vertex_shader, 1, &vertex_shader_source_cstr, NULL);
    glCompileShader(m_vertex_shader);

    // check for vertex shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(m_vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(m_vertex_shader, 512, NULL, infoLog);
      std::stringstream error{};
      error << "Vertex shader did not compile!\n" << infoLog << '\n';
      Log::Error(error.str());
      return nullptr;
    }

    return this;
  }

  Shader* Shader::CreateFragmentShader(const std::string& path_to_fragment_shader)
  {
    FE_FLOW_FUNCTION();

    // chain error handling
    if (this == nullptr) return nullptr;

    // warning if fragment shader already exists
    // handled by overriding the old shader
    if (m_fragment_shader != 0) {
      Log::Warning("Vertex shader already exists!");
      return nullptr;
    }

    // read fragment shader file
    std::string fragment_shader_source;
    std::ifstream ifs(m_base_path + '/' + path_to_fragment_shader);
    try
    {
      fragment_shader_source.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    }
    catch (const std::ifstream::failure& e)
    {
      std::stringstream error{};
      error << "Could not read fragment shader file!\n" << e.what() << '\n';
      Log::Error(error.str());
      return nullptr;
    }

    // create fragment shader
    m_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragment_shader_source_cstr = fragment_shader_source.c_str();
    glShaderSource(m_fragment_shader, 1, &fragment_shader_source_cstr, NULL);
    glCompileShader(m_fragment_shader);

    // check for fragment shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(m_fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(m_fragment_shader, 512, NULL, infoLog);
      std::stringstream error{};
      error << "Fragment shader did not compile!\n" << infoLog << '\n';
      Log::Error(error.str());
      return nullptr;
    }

    return this;
  }

  void Shader::Link()
  {
    FE_FLOW_FUNCTION();

    if (this == nullptr || // chain error handling
      m_vertex_shader == 0 || m_fragment_shader == 0) // check if shaders are compiled
    {
      Log::Fatal("Shader could not be linked because one of the shaders is missing!");
      return;
    }

    // create shader program
    m_shader_program = glCreateProgram();

    glAttachShader(m_shader_program, m_vertex_shader);
    glAttachShader(m_shader_program, m_fragment_shader);
    glLinkProgram(m_shader_program);

    // check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(m_shader_program, GL_LINK_STATUS, &success);
    if (!success) {
      glDetachShader(m_shader_program, m_vertex_shader);
      glDetachShader(m_shader_program, m_fragment_shader);
      glGetProgramInfoLog(m_shader_program, 512, NULL, infoLog);
      std::stringstream error{};
      error << "Shader linker error!\n" << infoLog << '\n';
      Log::Fatal(error.str());
    }

    // delete shaders
    glDeleteShader(m_vertex_shader);
    glDeleteShader(m_fragment_shader);
  }

}