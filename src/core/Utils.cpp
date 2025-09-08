#include "Utils.h"
#include <random>

namespace core {

namespace detail {
std::mt19937& getGenerator()
{
	static thread_local std::mt19937 gen;
	return gen;
}
} // namespace detail

void seed(unsigned int seed)
{
	detail::getGenerator().seed(seed);
}

float random(float min, float max)
{
	return std::uniform_real_distribution<float>(min, max)(detail::getGenerator());
}

int random(int min, int max)
{
	return std::uniform_int_distribution<int>(min, max)(detail::getGenerator());
}

} // namespace core
