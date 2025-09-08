#include "Asteroid.h"
#include "../core/Renderer.h"
#include "../core/Utils.h"
#include "../core/Game.h"
#include "../core/AssetManager.h"

namespace game {

Asteroid::Asteroid(core::Game& game, const core::Vec2f& position)
	: RoundObject(game, position, 3.0f)
	, texture_(game.assets()->loadTexture("asteroid.png"))
{
	const auto angle = core::radians(core::random(0.0f, 360.0f));
	const auto speed = core::random(0.0f, 10.0f);
	const auto velocity = core::Vec2f(std::sin(angle) * speed, std::cos(angle) * speed);

	setVelocity(velocity);
	setAngularVelocity(core::random(-45.0f, 45.0f));
}

void Asteroid::render(core::Renderer& renderer)
{
	renderer.drawTexture(texture_, position(), angle());
}

} // namespace game
