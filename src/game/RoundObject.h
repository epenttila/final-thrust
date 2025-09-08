#pragma once
#include "../core/Entity.h"
#include <memory>

namespace core {

class Game;

} // namespace core

namespace game {

class RoundObject : public core::Entity
{
public:
	RoundObject(core::Game& game, const core::Vec2f& position, float radius);
	~RoundObject();
	void update(core::Game& game, float deltaTime) override;
	void render(core::Renderer&) override {}
	float radius() const { return radius_; }
	bool contains(const core::Vec2f& point) const;
	bool intersects(const RoundObject& other) const;
	auto velocity() const { return velocity_; }
	void setVelocity(const core::Vec2f& velocity) { velocity_ = velocity; }
	auto acceleration() const { return acceleration_; }
	void setAcceleration(const core::Vec2f& acceleration) { acceleration_ = acceleration; }
	void addAcceleration(const core::Vec2f& acceleration) { acceleration_ += acceleration; }
	auto angle() const { return angle_; }
	void setAngle(float angle) { angle_ = angle; }
	auto angularVelocity() const { return angularVelocity_; }
	void setAngularVelocity(float angularVelocity) { angularVelocity_ = angularVelocity; }

private:
	float radius_ = 0.0f;
	core::Vec2f velocity_;
	core::Vec2f acceleration_;
	float angle_ = 0.0f;
	float angularVelocity_ = 0.0f;
};

} // namespace game
