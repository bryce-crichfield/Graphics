#include "Graphics.h"
#include "Window.h"
#include <glfw/glfw3.h>
#include <iostream>

#include "Shader.h"

auto vertexShaderSource = R"END(
    #version 330 core
    in vec4 aPos;
    in vec2 aTexCoord;
    in vec3 aNormal;

    out vec2 vTexCoord;

    uniform mat4 uTransform;

    void main() {
        vec4 position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
        position = uTransform * position;
        gl_Position = position;
        vTexCoord = aTexCoord;
    }
)END";

auto fragmentShaderSource = R"END(
    #version 330 core
    in vec2 vTexCoord;
    uniform sampler2D uTexture;
    out vec4 color;
    void main() { 
        color = texture(uTexture, vTexCoord);
    }
)END";

int main(void) {
    auto window = Window::create(800, 600);
    if (window == nullptr) {
        std::cout << "Failed to create window" << std::endl;
        return -1;
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    auto program = ShaderProgram::create(vertexShaderSource, fragmentShaderSource);
    if (!program) {
        std::cout << "Failed to create shader program" << std::endl;
        return -2;
    }

    // Load a 2d triangle vao with 3 vertices, indices and uvs
    auto positionAttribute = program->getAttributeLocation("aPos");
    auto uvAttribute = program->getAttributeLocation("aTexCoord");
    auto normalAttribute = program->getAttributeLocation("aNormal");

    auto quad = Geometry::buildQuad(positionAttribute, uvAttribute, normalAttribute);

    Matrix4 transform = Matrix4::identity();
    transform = transform * Matrix4::translate(-0.5f, 0.5f, 0.0f);
    transform = transform * Matrix4::scale(0.5f, 0.5f, 1.0f);

    auto texture = DeviceTexture();
    texture.load("../test.png");

    while (window->isOpen()) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        quad->draw(*program, transform, texture);

        window->update();
    }

    return 0;
}