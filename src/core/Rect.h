#pragma once
#include "Vec2.h"

namespace core {

template<typename T>
class Rect
{
public:
	Rect(T x, T y, T width, T height)
		: x_(x)
		, y_(y)
		, width_(width)
		, height_(height)
	{
	}
	Rect(int x, int y, int width, int height)
		: Rect(static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height))
	{
	}

	Rect() = default;

	T x() const { return x_; }
	T y() const { return y_; }
	T width() const { return width_; }
	T height() const { return height_; }
	T left() const { return x_; }
	T top() const { return y_; }
	T right() const { return x_ + width_; }
	T bottom() const { return y_ + height_; }
	Vec2<T> topLeft() const { return Vec2<T>(x_, y_); }
	Vec2<T> topRight() const { return Vec2<T>(x_ + width_, y_); }
	Vec2<T> bottomLeft() const { return Vec2<T>(x_, y_ + height_); }
	Vec2<T> bottomRight() const { return Vec2<T>(x_ + width_, y_ + height_); }
	Vec2<T> center() const { return Vec2<T>(x_ + width_ / 2, y_ + height_ / 2); }

	bool contains(const Vec2<T>& point) const
	{
		return point.x() >= x_ && point.x() < x_ + width_ && point.y() >= y_ && point.y() < y_ + height_;
	}

	Rect moveCenter(const Vec2<T>& center) const
	{
		auto tmp = *this;
		tmp.x_ = center.x() - width_ / 2;
		tmp.y_ = center.y() - height_ / 2;
		return tmp;
	}

	Rect moveTopLeft(const Vec2<T>& topLeft) const
	{
		auto tmp = *this;
		tmp.x_ = topLeft.x();
		tmp.y_ = topLeft.y();
		return tmp;
	}

	Rect moveTopRight(const Vec2<T>& topRight) const
	{
		auto tmp = *this;
		tmp.x_ = topRight.x() - tmp.width_;
		tmp.y_ = topRight.y();
		return tmp;
	}

private:
	T x_;
	T y_;
	T width_;
	T height_;
};

using RectF = Rect<float>;

} // namespace core
