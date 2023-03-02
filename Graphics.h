#pragma once

#include "Shader.h"
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <cmath>
#include "Math.h"
#include <glad/glad.h>
#include "Texture.h"

using Attribute = unsigned int;

// Wraps an OpenGL Vertex Buffer
struct VertexBuffer {
    unsigned target = GL_ARRAY_BUFFER;
    GLuint id;

    VertexBuffer() { glGenBuffers(1, &id); }

    void bind() { glBindBuffer(target, id); }

    void unbind() { glBindBuffer(target, 0); }

    ~VertexBuffer() { glDeleteBuffers(1, &id); }
};

// Wraps an OpenGL Vertex Array
struct VertexArray {
    unsigned indexCount;
    GLuint id;

    std::vector<std::unique_ptr<VertexBuffer>> buffers;

    VertexArray() { glGenVertexArrays(1, &id); }

    void bind() {
        glBindVertexArray(id);
        for (auto& buffer : buffers) {
            buffer->bind();
        }
    }

    void unbind() {
        for (auto& buffer : buffers) {
            buffer->unbind();
        }
        glBindVertexArray(0);
    }

    ~VertexArray() { glDeleteVertexArrays(1, &id); }

    void addBuffer(std::unique_ptr<VertexBuffer> buffer) { buffers.push_back(std::move(buffer)); }

    void draw(ShaderProgram& program, const Matrix4& transform, DeviceTexture& texture) {
        program.use();
        bind();
        program.setUniform("uTransform", transform);
        texture.bind();
        glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
        unbind();
    }
};

// Constructs a Vertex Array from a list of vertices and a list of uvs
// where the vertices and uvs are in separate subsections of the buffer
struct VertexArrayBuilder {
    struct Component {
        float x, y, z, w;
        float u, v;
        float nx, ny, nz;
    };
    
    Component current;
    std::vector<float> data;
    std::vector<uint32_t> indices;

    VertexArrayBuilder& vertex(float x, float y, float z, float w) {
        current.x = x;
        current.y = y;
        current.z = z;
        current.w = w;
        return (*this);
    }

    VertexArrayBuilder& uv(float u, float v) {
        current.u = u;
        current.v = v;
        return (*this);
    }

    VertexArrayBuilder& normal(float x, float y, float z) {
        current.nx = x;
        current.ny = y;
        current.nz = z;
        return (*this);
    }

    void end() {
        data.push_back(current.x);
        data.push_back(current.y);
        data.push_back(current.z);
        data.push_back(current.w);
        data.push_back(current.u);
        data.push_back(current.v);
        data.push_back(current.nx);
        data.push_back(current.ny);
        data.push_back(current.nz);
    }

    VertexArrayBuilder& index(uint32_t i) {
        indices.push_back(i);
        return *this;
    }

    std::unique_ptr<VertexArray> build(Attribute vertex, Attribute uv, Attribute normal) {
        auto vao = std::make_unique<VertexArray>();
        auto vbo = std::make_unique<VertexBuffer>();
        auto ebo = std::make_unique<VertexBuffer>();
        ebo->target = GL_ELEMENT_ARRAY_BUFFER;

        vao->bind();
        vbo->bind();
        ebo->bind();

        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);


        glVertexAttribPointer(vertex, 4, GL_FLOAT, GL_FALSE, sizeof(Component), (void*) offsetof(Component, x));
        glEnableVertexAttribArray(vertex);

        glVertexAttribPointer(uv, 2, GL_FLOAT, GL_FALSE, sizeof(Component), (void*) offsetof(Component, u));
        glEnableVertexAttribArray(uv);

        glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, sizeof(Component), (void*) offsetof(Component, nx));
        glEnableVertexAttribArray(normal);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(),
            GL_STATIC_DRAW);

        vao->indexCount = indices.size();

        vao->unbind();
        vbo->unbind();
        ebo->unbind();

        vao->addBuffer(std::move(vbo));
        vao->addBuffer(std::move(ebo));
        return vao;
    }
};

namespace Geometry {

std::unique_ptr<VertexArray> buildQuad(Attribute vertex, Attribute uv, Attribute normal) {
    VertexArrayBuilder builder;

    builder.vertex(-1, -1, 0, 0).uv(0, 0).normal(0, 0, 1).end();
    builder.vertex(1, -1, 0, 0).uv(1, 0).normal(0, 0, 1).end();
    builder.vertex(1, 1, 0, 0).uv(1, 1).normal(0, 0, 1).end();
    builder.vertex(-1, 1, 0, 0).uv(0, 1).normal(0, 0, 1).end();
    

    // Index using a triangle strip
    builder.index(0).index(1).index(2).index(3).index(0);

    return builder.build(vertex, uv, normal);
}
} // namespace Geometry

