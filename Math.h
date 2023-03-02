#pragma once

#include <array>
#include <cmath>

struct Vector2 {
    float x, y;
};

struct Vector3 {
    float x, y, z;
};

struct Vector4 {
    float x, y, z, w;
};

struct Matrix4 {
    // A column major matrix
    std::array<float, 16> data;

    Matrix4 operator*(const Matrix4& other) const {
        Matrix4 result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.data[i * 4 + j] = 0;
                for (int k = 0; k < 4; k++) {
                    result.data[i * 4 + j] += data[i * 4 + k] * other.data[k * 4 + j];
                }
            }
        }
        return result;
    }

    static Matrix4 identity() {
        Matrix4 result;
        result.data = {
            1, 0, 0, 0, //
            0, 1, 0, 0, //
            0, 0, 1, 0, //
            0, 0, 0, 1, //
        };
        return result;
    }

    static Matrix4 translate(float x, float y, float z) {
        Matrix4 result = identity();
        result.data[3] = x;
        result.data[7] = y;
        result.data[11] = z;
        return result;
    }

    static Matrix4 scale(float x, float y, float z) {
        Matrix4 result = identity();
        result.data[0] = x;
        result.data[5] = y;
        result.data[10] = z;
        return result;
    }
};