// System Headers
#include "shader.h"
#include "window.h"

#include <GLFW/glfw3.h>
#include <filesystem>
#include <glad/glad.h>
// Standard Headers

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  lun::Window window{800, 600, "lun"};

  lun::Shader<GL_VERTEX_SHADER> vertexShader{};
  vertexShader.source("Glitter/triangle.vert");
  vertexShader.compile();

  lun::Shader<GL_FRAGMENT_SHADER> fragmentShader{};
  fragmentShader.source("Glitter/triangle.frag");
  fragmentShader.compile();

  lun::Program shaderProgram;
  shaderProgram.attach(vertexShader);
  shaderProgram.attach(fragmentShader);
  shaderProgram.link();

  vertexShader.remove();
  fragmentShader.remove();

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  window.run([&]() {
    shaderProgram.use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  });

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  shaderProgram.remove();

  return 0;
}
