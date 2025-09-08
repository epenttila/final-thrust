#pragma once
#include "RoundObject.h"

struct SDL_Texture;

namespace core {

class Game;

} // namespace core

namespace game {

enum class PlanetType
{
	Earth,
	Mars,
	Jupiter,
	Saturn,
	Uranus,
};

class Planet : public RoundObject
{
public:
	static float radiusForType(PlanetType type);

	Planet(core::Game& game, const core::Vec2f& position, PlanetType type);
	virtual ~Planet() = default;
	void update(core::Game&, float) override {}
	void render(core::Renderer& renderer) override;

private:
	PlanetType type_ = PlanetType::Earth;
	float mass_ = 0.0f;
	SDL_Texture* texture_ = nullptr;
};

} // namespace game
