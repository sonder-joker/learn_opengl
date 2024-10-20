
#include <GLFW/glfw3.h>
#include <concepts>
#include <functional>
#include <glad/glad.h>
#include <iostream>

namespace lun {

template <typename T>
concept KeyInput = requires(T t) {
  { t.func1(int()) } -> std::same_as<int>;
};

class Window {
public:
  explicit Window(int width, int height, const char *title) {
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (window == nullptr) {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glfwSetFramebufferSizeCallback(window,
                                   [](GLFWwindow *, int width, int height) {
                                     glViewport(0, 0, width, height);
                                   });
  }

  template <std::invocable T> void run(T runable) {
    while (!glfwWindowShouldClose(window)) {
      if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
      }

      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      runable();

      glfwSwapBuffers(window);
      glfwPollEvents();
    }

    glfwTerminate();
  }

private:
  GLFWwindow *window;
};

} // namespace lun