#include "utils/utils.h"
#include <cmath>
#include <algorithm>

namespace utils {

namespace detail {
    std::mt19937& getGenerator() {
        static std::mt19937 gen;
        return gen;
    }
}

void setSeed(unsigned int seed) {
    detail::getGenerator().seed(seed);
}

float random(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(detail::getGenerator());
}

int random(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(detail::getGenerator());
}

Vector2 randomVector2(float minX, float maxX, float minY, float maxY) {
    return Vector2(random(minX, maxX), random(minY, maxY));
}

float clamp(float value, float min, float max) {
    return std::clamp(value, min, max);
}

float lerp(float a, float b, float t) {
    return a + (b - a) * clamp(t, 0.0f, 1.0f);
}

float angleDifference(float a, float b) {
    float diff = b - a;
    while (diff > 180.0f) diff -= 360.0f;
    while (diff < -180.0f) diff += 360.0f;
    return diff;
}

float normalizeAngle(float angle) {
    while (angle > 360.0f) angle -= 360.0f;
    while (angle < 0.0f) angle += 360.0f;
    return angle;
}

Vector2 rotateVector(const Vector2& vec, float angle) {
    const float rad = angle * (M_PI / 180.0f);
    const float cosA = std::cos(rad);
    const float sinA = std::sin(rad);
    
    return Vector2(
        vec.x * cosA - vec.y * sinA,
        vec.x * sinA + vec.y * cosA
    );
}

}

