#pragma once

#include <SDL3/SDL.h>
#include <cmath>

// Vector operations for SDL_FPoint
inline SDL_FPoint operator+(const SDL_FPoint& a, const SDL_FPoint& b) {
    return {a.x + b.x, a.y + b.y};
}

inline SDL_FPoint operator-(const SDL_FPoint& a, const SDL_FPoint& b) {
    return {a.x - b.x, a.y - b.y};
}

inline SDL_FPoint operator*(const SDL_FPoint& v, float scalar) {
    return {v.x * scalar, v.y * scalar};
}

inline SDL_FPoint operator*(float scalar, const SDL_FPoint& v) {
    return {v.x * scalar, v.y * scalar};
}

inline SDL_FPoint operator/(const SDL_FPoint& v, float scalar) {
    return {v.x / scalar, v.y / scalar};
}

inline SDL_FPoint& operator+=(SDL_FPoint& a, const SDL_FPoint& b) {
    a.x += b.x;
    a.y += b.y;
    return a;
}

inline SDL_FPoint& operator-=(SDL_FPoint& a, const SDL_FPoint& b) {
    a.x -= b.x;
    a.y -= b.y;
    return a;
}

inline SDL_FPoint& operator*=(SDL_FPoint& v, float scalar) {
    v.x *= scalar;
    v.y *= scalar;
    return v;
}

inline float lengthSquared(const SDL_FPoint& v) {
    return v.x * v.x + v.y * v.y;
}

inline float length(const SDL_FPoint& v) {
    return std::sqrt(lengthSquared(v));
}

inline SDL_FPoint normalized(const SDL_FPoint& v) {
    const float len = length(v);
    if (len > 0.0f)
        return v / len;
    return {0.0f, 0.0f};
}

inline float distance(const SDL_FPoint& a, const SDL_FPoint& b) {
    return length(b - a);
}

inline float distanceSquared(const SDL_FPoint& a, const SDL_FPoint& b) {
    return lengthSquared(b - a);
}

inline float dot(const SDL_FPoint& a, const SDL_FPoint& b) {
    return a.x * b.x + a.y * b.y;
}