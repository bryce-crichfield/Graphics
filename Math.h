#pragma once

#include <array>
#include <cmath>

struct Vector2 {
    float x, y;

    Vector2() : x(0), y(0) {}

    Vector2(float x, float y) : x(x), y(y) {}

    Vector2(const Vector2& other) : x(other.x), y(other.y) {}

    Vector2& operator=(const Vector2& other) {
        x = other.x;
        y = other.y;
        return *this;
    }

    Vector2 operator+(const Vector2& other) const {
        return {x + other.x, y + other.y};
    }

    Vector2 operator-(const Vector2& other) const {
        return {x - other.x, y - other.y};
    }

    Vector2 operator*(const Vector2& other) const {
        return {x * other.x, y * other.y};
    }

    Vector2 operator/(const Vector2& other) const {
        return {x / other.x, y / other.y};
    }
    
    Vector2 operator+(float scalar) const {
        return {x + scalar, y + scalar};
    }

    Vector2 operator-(float scalar) const {
        return {x - scalar, y - scalar};
    }

    Vector2 operator*(float scalar) const {
        return {x * scalar, y * scalar};
    }

    Vector2 operator/(float scalar) const {
        return {x / scalar, y / scalar};
    }

    float dot(const Vector2& other) const {
        return x * other.x + y * other.y;
    }

    float length() const {
        return std::sqrt(x * x + y * y);
    }

    Vector2 normalized() const {
        return *this / length();
    }

    bool operator==(const Vector2& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Vector2& other) const {
        return !(*this == other);
    }
};

struct Vector3 {
    float x, y, z;

    Vector3() : x(0), y(0), z(0) {}

    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vector3(const Vector3& other) : x(other.x), y(other.y), z(other.z) {}

    Vector3& operator=(const Vector3& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    Vector3 operator+(const Vector3& other) const {
        return {x + other.x, y + other.y, z + other.z};
    }

    Vector3 operator-(const Vector3& other) const {
        return {x - other.x, y - other.y, z - other.z};
    }

    Vector3 operator*(const Vector3& other) const {
        return {x * other.x, y * other.y, z * other.z};
    }

    Vector3 operator/(const Vector3& other) const {
        return {x / other.x, y / other.y, z / other.z};
    }

    Vector3 operator+(float scalar) const {
        return {x + scalar, y + scalar, z + scalar};
    }

    Vector3 operator-(float scalar) const {
        return {x - scalar, y - scalar, z - scalar};
    }

    Vector3 operator*(float scalar) const {
        return {x * scalar, y * scalar, z * scalar};
    }

    Vector3 operator/(float scalar) const {
        return {x / scalar, y / scalar, z / scalar};
    }

    float dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    Vector3 cross(const Vector3& other) const {
        return {
            y * other.z - z * other.y, //
            z * other.x - x * other.z, //
            x * other.y - y * other.x, //
        };
    }

    float length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vector3 normalized() const {
        return *this / length();
    }

    bool operator==(const Vector3& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const Vector3& other) const {
        return !(*this == other);
    }
};

struct Vector4 {
    float x, y, z, w;

    Vector4() : x(0), y(0), z(0), w(0) {}

    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    Vector4(const Vector4& other) : x(other.x), y(other.y), z(other.z), w(other.w) {}

    Vector4& operator=(const Vector4& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
        return *this;
    }

    Vector4 operator+(const Vector4& other) const {
        return {x + other.x, y + other.y, z + other.z, w + other.w};
    }

    Vector4 operator-(const Vector4& other) const {
        return {x - other.x, y - other.y, z - other.z, w - other.w};
    }

    Vector4 operator*(const Vector4& other) const {
        return {x * other.x, y * other.y, z * other.z, w * other.w};
    }

    Vector4 operator/(const Vector4& other) const {
        return {x / other.x, y / other.y, z / other.z, w / other.w};
    }

    Vector4 operator+(float scalar) const {
        return {x + scalar, y + scalar, z + scalar, w + scalar};
    }

    Vector4 operator-(float scalar) const {
        return {x - scalar, y - scalar, z - scalar, w - scalar};
    }

    Vector4 operator*(float scalar) const {
        return {x * scalar, y * scalar, z * scalar, w * scalar};
    }

    Vector4 operator/(float scalar) const {
        return {x / scalar, y / scalar, z / scalar, w / scalar};
    }

    float dot(const Vector4& other) const {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    float length() const {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }

    Vector4 normalized() const {
        return *this / length();
    }

    bool operator==(const Vector4& other) const {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }

    bool operator!=(const Vector4& other) const {
        return !(*this == other);
    }
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