#include "Graphics.h"
#include "Window.h"
#include <glfw/glfw3.h>
#include <iostream>
#include "Animation.h"

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

    auto texture = DeviceTexture();
    texture.load("../test.png");


    Animation animation;
    {
        auto start = Transform();
        start.translation = Vector3(0.0f, 0.0f, 0.0f);
        start.scale = Vector3(0.5f, 0.5f, 1.0f);

        auto end = Transform();
        end.translation = Vector3(0.5f, 0.5f, 0.0f);
        end.scale = Vector3(1.0f, 1.0f, 1.0f);

        auto frame = AnimationFrame(start, end, Easing::linear);

        animation.add(frame, Seconds(1.0f));
    }

    {
        auto start = Transform();
        start.translation = Vector3(0.5f, 0.5f, 0.0f);
        start.scale = Vector3(1.0f, 1.0f, 1.0f);

        auto end = Transform();
        end.translation = Vector3(0.0f, 0.0f, 0.0f);
        end.scale = Vector3(0.5f, 0.5f, 1.0f);

        auto frame = AnimationFrame(start, end, Easing::linear);

        animation.add(frame, Seconds(1.0f));
    }

    float lastTime = 0.0f;

    while (window->isOpen()) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        animation.update(Seconds(deltaTime));

        auto transform = animation.getTransform();
        quad->draw(*program, transform.toMatrix(), texture);


        window->update();
    }

    return 0;
}