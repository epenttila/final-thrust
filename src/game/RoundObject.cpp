#include "RoundObject.h"
#include <cmath>
#include <cstdio>
#include "../core/Renderer.h"
#include "../core/Utils.h"
#include "../core/Animation.h"

namespace game {

RoundObject::RoundObject(core::Game&, const core::Vec2f& position, float radius)
	: Entity(position)
	, radius_(radius)
{
}

RoundObject::~RoundObject() {}

void RoundObject::update(core::Game&, float deltaTime)
{
	velocity_ += acceleration_ * deltaTime;
	setPosition(position() + velocity_ * deltaTime);
	angle_ += angularVelocity_ * deltaTime;
	angle_ = core::normalize(angle_, 0.0f, 360.0f);
}

bool RoundObject::contains(const core::Vec2f& point) const
{
	return core::Vec2f::distanceSquared(position(), point) <= (radius_ * radius_);
}

bool RoundObject::intersects(const RoundObject& other) const
{
	return core::Vec2f::distance(position(), other.position()) < (radius_ + other.radius_);
}

} // namespace game
