#include "PlayState.h"
#include <string>
#include <cmath>
#include <iostream>
#include "MenuState.h"
#include "../core/Game.h"
#include "../core/Renderer.h"
#include "../core/AssetManager.h"
#include "../core/InputManager.h"
#include "../core/AudioManager.h"
#include "../core/StateManager.h"
#include "Player.h"
#include "Planet.h"
#include "Asteroid.h"
#include "../core/Utils.h"

namespace game {

PlayState::PlayState(core::Game& game, int level)
	: level_(level)
	, player_(new Player(game, core::Vec2f(20, 120)))
	, backgroundTexture_(game.assets()->loadTexture("background.png"))
	, font_(game.assets()->loadFont("nokiafc22.ttf", 8))
	, bigFont_(game.assets()->loadFont("nokiafc22.ttf", 16))
	, music_(game.assets()->loadAudio("variables.mp3"))
{
	core::seed(level);

	static constexpr auto ZONE_WIDTH = 50;
	const auto gameWidth = static_cast<float>(game.width());
	const auto gameHeight = static_cast<float>(game.height());

	const auto homeX = core::random(gameWidth - ZONE_WIDTH, gameWidth);
	const auto homeY = core::random(0.0f, gameHeight);
	homePlanet_ = std::make_unique<Planet>(game, core::Vec2f(homeX, homeY), PlanetType::Earth);

	const auto gridSize = static_cast<int>(std::ceil(std::sqrt(MAX_LEVEL)));
	const auto gridWidth = gameWidth - ZONE_WIDTH - ZONE_WIDTH;
	const auto gridHeight = gameHeight;

	std::vector<bool> occupied(gridSize * gridSize);

	for (int i = 0; i < level_ + 1; ++i)
	{
		const auto randomX = core::random(0, gridSize - 1);
		const auto randomY = core::random(0, gridSize - 1);
		const auto index = randomX + gridSize * randomY;

		if (occupied[index])
			continue;

		occupied[index] = true;

		static constexpr PlanetType types[] =
			{PlanetType::Mars, PlanetType::Jupiter, PlanetType::Saturn, PlanetType::Uranus};
		const auto type = types[core::random(0, std::size(types) - 1)];

		const auto planetX = ZONE_WIDTH + ((randomX + 0.5f) / gridSize) * gridWidth;
		const auto planetY = ((randomY + 0.5f) / gridSize) * gridHeight;

		planets_.push_back(std::make_unique<Planet>(game, core::Vec2f(planetX, planetY), type));
	}

	for (int i = 0; i < level_ + 1; ++i)
	{
		const auto x = core::random(static_cast<float>(ZONE_WIDTH), static_cast<float>(game.width()));
		const auto y = core::random(0.0f, static_cast<float>(game.height()));

		asteroids_.push_back(std::make_unique<Asteroid>(game, core::Vec2f(x, y)));
	}
}

void PlayState::enter(core::Game& game)
{
	SDL_SetCursor(game.assets()->loadCursor("cursor.png", 8, 8));

	game.audio()->playMusic(music_, 0.1f);
}

void PlayState::exit(core::Game& game)
{
	game.audio()->stopMusic();

	SDL_SetCursor(SDL_GetDefaultCursor());
}

void PlayState::update(core::Game& game, float deltaTime)
{
	if (game.input()->isKeyPressed(SDL_SCANCODE_ESCAPE))
	{
		game.state()->pop(game);
		return;
	}

	for (auto& asteroid : asteroids_)
	{
		applyGravity(*asteroid, 10.0f);
		asteroid->update(game, deltaTime);
	}

	switch (player_->state())
	{
	case PlayerState::Flying:
	{
		applyGravity(*player_, 100.0f);
		player_->update(game, deltaTime);

		if (isPlayerCollidingWithWorld(game))
			player_->setState(game, PlayerState::Exploding);
		else if (homePlanet_->contains(player_->position()))
			player_->setState(game, PlayerState::Home);

		return;
	}
	case PlayerState::Exploding: player_->update(game, deltaTime); return;
	case PlayerState::Dead:
	case PlayerState::Home:
	{
		if (game.input()->isMousePressed())
		{
			game.state()->change(
				game,
				std::make_unique<PlayState>(game, player_->state() == PlayerState::Home ? level_ + 1 : level_)
			);
		}
		return;
	}
	}
}

void PlayState::render(core::Renderer& renderer)
{
	renderer.drawTexture(backgroundTexture_, renderer.logicalRect());

	player_->render(renderer);
	homePlanet_->render(renderer);

	for (auto& planet : planets_)
		planet->render(renderer);

	for (auto& asteroid : asteroids_)
		asteroid->render(renderer);

	renderer.drawText(font_, {0, 0}, 1.0f, 1.0f, "Level " + std::to_string(level_ + 1), {255, 255, 255, 255});
	renderer.drawText(
		font_,
		renderer.logicalRect().topRight(),
		-1.0f,
		1.0f,
		std::to_string(static_cast<int>(player_->fuel())) + "%",
		{255, 255, 255, 255}
	);

	const auto center = renderer.logicalRect().center();

	switch (player_->state())
	{
	case PlayerState::Flying:
	case PlayerState::Exploding: break;
	case PlayerState::Dead:
		renderer.drawText(bigFont_, center, 0.0f, -1.0f, "You are dead", {255, 255, 255, 255});
		renderer.drawText(font_, center, 0.0f, 1.0f, "Click to continue", {255, 255, 255, 255});
		break;
	case PlayerState::Home:
		renderer.drawText(bigFont_, center, 0.0f, -1.0f, "You survived!", {255, 255, 255, 255});
		renderer.drawText(font_, center, 0.0f, 1.0f, "Click to continue", {255, 255, 255, 255});
		break;
	}
}

void PlayState::applyGravity(RoundObject& object, float coefficient)
{
	auto gravity = core::Vec2f(0, 0);

	for (const auto& planet : planets_)
	{
		const auto r = object.position() - planet->position();
		const auto dd = r.lengthSquared();

		if (dd > 1.0f)
			gravity += r * coefficient / dd;
	}

	object.setAcceleration(-gravity);
}

bool PlayState::isPlayerCollidingWithWorld(core::Game& game) const
{
	for (const auto& planet : planets_)
	{
		if (planet->contains(player_->position()))
			return true;
	}

	for (const auto& asteroid : asteroids_)
	{
		if (asteroid->intersects(*player_))
			return true;
	}

	return !core::RectF(-50.0f, -50.0f, game.width() + 100.0f, game.height() + 100.0f).contains(player_->position());
}

} // namespace game
