#pragma once

struct Seconds {
    float value;

    Seconds() : value(0) {}

    Seconds(float value) : value(value) {}

    Seconds(const Seconds& other) : value(other.value) {}

    Seconds& operator=(const Seconds& other) {
        value = other.value;
        return *this;
    }

    Seconds operator+(const Seconds& other) const {
        return {value + other.value};
    }

    Seconds operator-(const Seconds& other) const {
        return {value - other.value};
    }

    Seconds operator*(const Seconds& other) const {
        return {value * other.value};
    }

    Seconds operator/(const Seconds& other) const {
        return {value / other.value};
    }

    void operator+=(const Seconds& other) {
        value += other.value;
    }

    void operator-=(const Seconds& other) {
        value -= other.value;
    }

    void operator*=(const Seconds& other) {
        value *= other.value;
    }

    void operator/=(const Seconds& other) {
        value /= other.value;
    }

    bool operator==(const Seconds& other) const {
        return value == other.value;
    }

    bool operator!=(const Seconds& other) const {
        return value != other.value;
    }

    bool operator<(const Seconds& other) const {
        return value < other.value;
    }

    bool operator>(const Seconds& other) const {
        return value > other.value;
    }

    bool operator<=(const Seconds& other) const {
        return value <= other.value;
    }

    bool operator>=(const Seconds& other) const {
        return value >= other.value;
    }

    static Seconds zero() {
        return {0};
    }
};