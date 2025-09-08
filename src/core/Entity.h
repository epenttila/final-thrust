#pragma once
#include "Vec2.h"

namespace core {

class Renderer;
class Game;

class Entity
{
public:
	Entity(const Vec2f& position)
		: position_(position)
	{
	}
	virtual ~Entity() = default;
	virtual void update(core::Game&, float) {}
	virtual void render(Renderer&) {}
	Vec2f position() const { return position_; }
	void setPosition(const Vec2f& pos) { position_ = pos; }

private:
	Vec2f position_;
};

} // namespace core
