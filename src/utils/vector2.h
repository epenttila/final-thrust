#pragma once

#include <cmath>



struct Vector2 {
    float x = 0.0f;
    float y = 0.0f;
    
    Vector2() = default;
    Vector2(float x, float y) : x(x), y(y) {}
    
    Vector2 operator+(const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }
    
    Vector2 operator-(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }
    
    Vector2 operator*(float scalar) const {
        return Vector2(x * scalar, y * scalar);
    }
    
    Vector2 operator/(float scalar) const {
        return Vector2(x / scalar, y / scalar);
    }
    
    Vector2& operator+=(const Vector2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    
    Vector2& operator-=(const Vector2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    
    Vector2& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    
    float length() const {
        return std::sqrt(x * x + y * y);
    }
    
    float lengthSquared() const {
        return x * x + y * y;
    }
    
    Vector2 normalized() const {
        const float len = length();
        if (len > 0.0f)
            return *this / len;
        return Vector2(0.0f, 0.0f);
    }
    
    float dot(const Vector2& other) const {
        return x * other.x + y * other.y;
    }
    
    static float distance(const Vector2& a, const Vector2& b) {
        return (b - a).length();
    }
    
    static float distanceSquared(const Vector2& a, const Vector2& b) {
        return (b - a).lengthSquared();
    }
};

