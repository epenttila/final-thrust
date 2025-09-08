#pragma once
#include "RoundObject.h"

struct SDL_Texture;

namespace core {

class Game;

} // namespace core

namespace game {

class Asteroid : public RoundObject
{
public:
	Asteroid(core::Game& game, const core::Vec2f& position);
	void render(core::Renderer& renderer) override;

private:
	SDL_Texture* texture_ = nullptr;
};

} // namespace game
