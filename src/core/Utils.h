#pragma once
#include <numbers>
#include "Vec2.h"

struct SDL_Color;

namespace core {

void seed(unsigned int seed);
float random(float min, float max);
int random(int min, int max);

static constexpr float radians(float degrees)
{
	return degrees * (std::numbers::pi_v<float> / 180.0f);
}

static constexpr float degrees(float radians)
{
	return radians * (180.0f / std::numbers::pi_v<float>);
}

template<typename T>
T normalize(T value, T start, T end)
{
	const auto width = end - start;
	const auto offset = value - start;

	return start + offset - std::floor(offset / width) * width;
}

} // namespace core
