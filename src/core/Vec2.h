#pragma once
#include <cmath>

namespace core {

template<typename T>
class Vec2
{
public:
	static T distance(const Vec2& a, const Vec2& b) { return (b - a).length(); }
	static T distanceSquared(const Vec2& a, const Vec2& b) { return (b - a).lengthSquared(); }

	Vec2() = default;
	Vec2(T x, T y)
		: x_(x)
		, y_(y)
	{
	}

	T x() const { return x_; }
	T y() const { return y_; }

	Vec2 operator+(const Vec2& other) const { return Vec2(x_ + other.x_, y_ + other.y_); }
	Vec2 operator-(const Vec2& other) const { return Vec2(x_ - other.x_, y_ - other.y_); }
	Vec2 operator*(T scalar) const { return Vec2(x_ * scalar, y_ * scalar); }
	Vec2 operator/(T scalar) const { return Vec2(x_ / scalar, y_ / scalar); }

	Vec2 operator-() const { return Vec2(-x_, -y_); }
	Vec2& operator+=(const Vec2& other)
	{
		x_ += other.x_;
		y_ += other.y_;
		return *this;
	}

	Vec2& operator-=(const Vec2& other)
	{
		x_ -= other.x_;
		y_ -= other.y_;
		return *this;
	}

	Vec2& operator*=(T scalar)
	{
		x_ *= scalar;
		y_ *= scalar;
		return *this;
	}

	T length() const { return std::sqrt(x_ * x_ + y_ * y_); }
	T lengthSquared() const { return x_ * x_ + y_ * y_; }

	Vec2 normalized() const
	{
		if (const auto len = length(); len)
			return *this / len;

		return Vec2();
	}

	T dot(const Vec2& other) const { return x_ * other.x_ + y_ * other.y_; }

	T angle() const { return std::atan2(y_, x_); }

private:
	T x_ = 0.0f;
	T y_ = 0.0f;
};

using Vec2f = Vec2<float>;

} // namespace core
