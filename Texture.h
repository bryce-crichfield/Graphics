#pragma once

#include <glad/glad.h>
#include <vector>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>




// Wraps a texture in OpenGL which is stored on the GPU
struct DeviceTexture {
    GLuint id;

    DeviceTexture() { glGenTextures(1, &id); }

    void bind() { glBindTexture(GL_TEXTURE_2D, id); }

    void unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

    ~DeviceTexture() { glDeleteTextures(1, &id); }

    void load(const std::string& path) {
        // Use STB to load the image
        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        if (!data) {
            std::cout << "Failed to load texture: " << path << std::endl;
            return;
        }        


        // Configure the texture
        bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        unbind();

        // Free the image data from the cpu
        stbi_image_free(data);
    }
};

