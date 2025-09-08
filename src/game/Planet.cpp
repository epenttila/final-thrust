#include "Planet.h"
#include "../core/Renderer.h"
#include "../core/AssetManager.h"
#include "../core/Utils.h"
#include "../core/Game.h"

namespace game {

namespace {

float typeToRadius(PlanetType type)
{
	switch (type)
	{
	case PlanetType::Earth: return 8.0f;
	case PlanetType::Jupiter: return 15.0f;
	case PlanetType::Uranus: return 12.0f;
	case PlanetType::Saturn: return 10.0f;
	case PlanetType::Mars: return 8.0f;
	}

	return 0.0f;
}

} // namespace

Planet::Planet(core::Game& game, const core::Vec2f& position, PlanetType type)
	: RoundObject(game, position, typeToRadius(type))
	, type_(type)
	, mass_(radius() * radius() * 0.1f)
{
	switch (type)
	{
	case PlanetType::Earth: texture_ = game.assets()->loadTexture("home.png"); break;
	case PlanetType::Mars: texture_ = game.assets()->loadTexture("mars.png"); break;
	case PlanetType::Jupiter: texture_ = game.assets()->loadTexture("jupiter.png"); break;
	case PlanetType::Saturn: texture_ = game.assets()->loadTexture("saturn.png"); break;
	case PlanetType::Uranus: texture_ = game.assets()->loadTexture("uranus.png"); break;
	}
}

void Planet::render(core::Renderer& renderer)
{
	renderer.drawTexture(texture_, position());
}

} // namespace game
