#include <filesystem>
#include <fstream>
#include <glad/glad.h>
#include <iostream>

namespace {
namespace fs = std::filesystem;
std::string readFile(fs::path path) {
  // Open the stream to 'lock' the file.
  std::ifstream f(path, std::ios::in | std::ios::binary);

  // Obtain the size of the file.
  const auto sz = fs::file_size(path);

  // Create a buffer.
  std::string result(sz, '\0');

  // Read the whole file into the buffer.
  f.read(result.data(), sz);

  return result;
}
} // namespace

namespace lun {

template <int Type> class Shader {
public:
  void source(std::filesystem::path filePath) {
    const auto vertexShaderSource = readFile(filePath);
    auto c_str = vertexShaderSource.c_str();
    glShaderSource(rawShader, 1, &c_str, nullptr);
  }

  void compile() {
    glCompileShader(rawShader);

    int success;
    glGetShaderiv(rawShader, GL_COMPILE_STATUS, &success);
    if (!success) {
      char infoLog[512];
      glGetShaderInfoLog(rawShader, 512, nullptr, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                << infoLog << std::endl;
    }
  }

  auto raw() { return rawShader; }

  void remove() { glDeleteShader(rawShader); }

private:
  unsigned int rawShader = glCreateShader(Type);
};

class Program {
public:
  template <int T> void attach(Shader<T> shader) {
    glAttachShader(shaderProgram, shader.raw());
  }

  ~Program() { glDeleteProgram(shaderProgram); }

  void link() {
    glLinkProgram(shaderProgram);
    {
      int success;
      char infoLog[512];
      glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
      if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
      }
    }
  }

  void use() { glUseProgram(shaderProgram); }

  void remove() { glDeleteProgram(shaderProgram); }

  auto raw() { return shaderProgram; }

private:
  unsigned int shaderProgram = glCreateProgram();
};
} // namespace lun
