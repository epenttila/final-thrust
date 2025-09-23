#pragma once
#include "../core/State.h"
#include "../core/Vec2.h"
#include "RoundObject.h"
#include <vector>
#include <memory>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>

namespace game {

class Player;
class Planet;
class Asteroid;

class PlayState : public core::State
{
public:
	static constexpr auto MAX_LEVEL = 81;

	PlayState(core::Game& game, int level);
	~PlayState() = default;
	void update(core::Game& game, float deltaTime) override;
	void render(core::Renderer& renderer) override;
	void enter(core::Game& game) override;
	void exit(core::Game& game) override;

private:
	struct Star
	{
		float x = 0;
		float y = 0;
		float m = 0.0f;
	};

	core::Vec2f makeGravity(RoundObject& object) const;
	bool isPlayerCollidingWithWorld(core::Game& game) const;
	void renderFuelBar(core::Renderer& renderer);
	void renderBackground(core::Renderer& renderer);

	int level_ = 0;
	std::unique_ptr<Player> player_;
	std::vector<std::unique_ptr<Planet>> planets_;
	std::vector<std::unique_ptr<Asteroid>> asteroids_;
	TTF_Font* font_ = nullptr;
	TTF_Font* bigFont_ = nullptr;
	MIX_Audio* music_ = nullptr;
	std::vector<Star> stars_;
};

} // namespace game
