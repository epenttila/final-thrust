#include "MenuState.h"
#include <string>
#include <iostream>
#include <cmath>
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "../core/Game.h"
#include "../core/Renderer.h"
#include "../core/AssetManager.h"
#include "../core/InputManager.h"
#include "../core/AudioManager.h"
#include "../core/StateManager.h"
#include "PlayState.h"
#include "Player.h"
#include "Planet.h"
#include "Asteroid.h"
#include "Button.h"

namespace game {

MenuState::MenuState(core::Game& game)
	: backgroundTexture_(game.assets()->loadTexture("menu.png"))
	, titleTexture_(game.assets()->loadTexture("title.png"))
	, font_(game.assets()->loadFont("nokiafc22.ttf", 8))
	, music_(game.assets()->loadAudio("solitude.mp3"))
{
	const auto buttonY = game.height() - 60.0f;
	const auto buttonSpacing = 3;

	playButton_ = std::make_unique<Button>(
		"PLAY",
		game.assets()->loadTexture("button.png"),
		game.assets()->loadTexture("buttonhover.png")
	);
	playButton_->setRect(playButton_->rect().moveCenter(core::Vec2f(game.width() / 2.0f, buttonY)));
	playButton_->setFont(font_);

	leftButton_ = std::make_unique<Button>(
		"",
		game.assets()->loadTexture("buttonleftoff.png"),
		game.assets()->loadTexture("buttonlefton.png")
	);
	leftButton_->setRect(leftButton_->rect().moveTopLeft(
		core::Vec2f(playButton_->rect().x(), playButton_->rect().y() + playButton_->rect().height() + buttonSpacing)
	));

	rightButton_ = std::make_unique<Button>(
		"",
		game.assets()->loadTexture("buttonrightoff.png"),
		game.assets()->loadTexture("buttonrighton.png")
	);
	rightButton_->setRect(rightButton_->rect().moveTopRight(core::Vec2f(
		playButton_->rect().x() + playButton_->rect().width(),
		playButton_->rect().y() + playButton_->rect().height() + buttonSpacing
	)));
}

MenuState::~MenuState() {}

void MenuState::enter(core::Game& game)
{
	game.audio()->playMusic(music_, 0.1f);
}

void MenuState::exit(core::Game& game)
{
	game.audio()->stopMusic();
}

void MenuState::update(core::Game& game, float deltaTime)
{
	if (game.input()->isKeyPressed(SDL_SCANCODE_ESCAPE))
	{
		game.state()->pop(game);
		return;
	}

	if (playPressed_)
	{
		if (!game.audio()->isMusicPlaying())
		{
			playPressed_ = false;
			game.state()->push(game, std::make_unique<PlayState>(game, currentLevel_));
		}

		return;
	}

	playButton_->update(game, deltaTime);
	leftButton_->update(game, deltaTime);
	rightButton_->update(game, deltaTime);

	if (playButton_->clicked())
	{
		playPressed_ = true;
		game.audio()->stopMusic(500);
	}
	if (leftButton_->clicked())
	{
		--currentLevel_;

		if (currentLevel_ < 0)
			currentLevel_ = 0;
	}
	if (rightButton_->clicked())
	{
		++currentLevel_;

		if (currentLevel_ >= PlayState::MAX_LEVEL)
			currentLevel_ = PlayState::MAX_LEVEL - 1;
	}
}

void MenuState::render(core::Renderer& renderer)
{
	renderer.drawTexture(backgroundTexture_, renderer.logicalRect());

	float titleWidth = 0;
	float titleHeight = 0;
	SDL_GetTextureSize(titleTexture_, &titleWidth, &titleHeight);

	const auto titleX = (renderer.logicalWidth() - titleWidth) / 2.0f;
	renderer.drawTexture(titleTexture_, core::RectF(titleX, 16.0f, titleWidth, titleHeight));

	playButton_->render(renderer);
	leftButton_->render(renderer);
	rightButton_->render(renderer);

	renderer.drawText(
		font_,
		core::RectF(
			leftButton_->rect().right() + 3,
			playButton_->rect().bottom() + 3,
			rightButton_->rect().left() - leftButton_->rect().right() - 3 - 3,
			leftButton_->rect().height()
		),
		"LVL " + std::to_string(currentLevel_ + 1),
		{255, 255, 255, 255}
	);

	renderer.drawText(
		font_,
		core::RectF(0, renderer.logicalHeight() - 16, renderer.logicalWidth(), 16),
		"Click to accelerate, ESC to quit",
		{128, 128, 128, 255}
	);
}

} // namespace game
