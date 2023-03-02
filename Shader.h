#pragma once

#include <iostream>
#include <memory>
#include <string>

#include <glad/glad.h>

#include "Math.h"


enum class ShaderType {
    Vertex = GL_VERTEX_SHADER,
    Fragment = GL_FRAGMENT_SHADER,
};

struct ShaderStage {
    GLuint id;

    ShaderStage(ShaderType type) {
        auto glType = static_cast<unsigned>(type);
        id = glCreateShader(glType);
    }

    ~ShaderStage() { glDeleteShader(id); }

    std::string getInfoLog() {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        std::string log(length, ' ');
        glGetShaderInfoLog(id, length, &length, log.data());

        return log;
    }

    bool compile(const std::string& source) {
        const char* src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        int success;
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        return success == GL_TRUE ? true : false;
    }

    static std::unique_ptr<ShaderStage> create(ShaderType type, const std::string& source) {
        auto shader = std::make_unique<ShaderStage>(type);
        if (shader->compile(source)) {
            return shader;
        } else {
            std::cout << "Shader compilation failed: " << shader->getInfoLog() << std::endl;
            return nullptr;
        }
    }
};

struct ShaderProgram {
    GLint id;

  public:
    ShaderProgram() { id = glCreateProgram(); }

    ~ShaderProgram() { glDeleteProgram(id); }

    std::string getInfoLog() {
        int length;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);

        std::string log(length, ' ');
        glGetProgramInfoLog(id, length, &length, log.data());

        return log;
    }

    bool link(const ShaderStage& vertex, const ShaderStage& fragment) {
        glAttachShader(id, vertex.id);
        glAttachShader(id, fragment.id);

        glLinkProgram(id);

        glDetachShader(id, vertex.id);
        glDetachShader(id, fragment.id);

        int success;
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        return success == GL_TRUE ? true : false;
    }

    void use() { glUseProgram(id); }

    unsigned getAttributeLocation(const std::string& name) {
        return glGetAttribLocation(id, name.c_str());
    }

    void setUniform(const std::string& name, float value) {
        glUniform1f(glGetUniformLocation(id, name.c_str()), value);
    }

    void setUniform(const std::string& name, const Vector3& vector) {
        float data[3] = {vector.x, vector.y, vector.z};
        glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, data);
    }

    void setUniform(const std::string& name, const Matrix4& matrix) {
        auto pointer = matrix.data.data();
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_TRUE, pointer);
    }

    static std::unique_ptr<ShaderProgram> create(std::string vertex, std::string fragment) {
        auto vertexShader = ShaderStage::create(ShaderType::Vertex, vertex);
        auto fragmentShader = ShaderStage::create(ShaderType::Fragment, fragment);

        if (!vertexShader || !fragmentShader) {
            return nullptr;
        }

        auto program = std::make_unique<ShaderProgram>();
        if (program->link(*vertexShader, *fragmentShader)) {
            return program;
        } else {
            std::cout << "Shader linking failed: " << program->getInfoLog() << std::endl;
            return nullptr;
        }
    }
};