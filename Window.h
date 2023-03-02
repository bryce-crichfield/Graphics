#pragma once

#include <glfw/glfw3.h>

#include <optional>
#include <memory>

struct Window {
    GLFWwindow* window;
    Window() = default;

  public:
    ~Window() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    static std::unique_ptr<Window> create(unsigned width, unsigned height) {
        if (!glfwInit()) {
            return nullptr;
        }

        auto window = std::make_unique<Window>();
        window->window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);

        if (!window->window) {
            glfwTerminate();
            return nullptr;
        }

        glfwMakeContextCurrent(window->window);

        return window;
    }

    bool isOpen() {
        return !glfwWindowShouldClose(window);
    }

    void update() {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
};