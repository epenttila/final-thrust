#pragma once

#include <random>
#include "utils/vector2.h"

namespace utils {
    void setSeed(unsigned int seed);
    float random(float min, float max);
    int random(int min, int max);
    Vector2 randomVector2(float minX, float maxX, float minY, float maxY);
    float clamp(float value, float min, float max);
    float lerp(float a, float b, float t);
    float angleDifference(float a, float b);
    float normalizeAngle(float angle);
    Vector2 rotateVector(const Vector2& vec, float angle);
    
    namespace detail {
        std::mt19937& getGenerator();
    }
}

