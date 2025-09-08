#include "Player.h"
#include <algorithm>
#include <numbers>
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include "../core/Renderer.h"
#include "../core/InputManager.h"
#include "../core/Utils.h"
#include "../core/Animation.h"
#include "../core/Game.h"
#include "../core/AssetManager.h"
#include "../core/AudioManager.h"

namespace game {

Player::Player(core::Game& game, const core::Vec2f& position)
	: RoundObject(game, position, 4.0f)
	, exhaustTexture_(game.assets()->loadTexture("exhaust.png"))
	, explosionTexture_(game.assets()->loadTexture("explosion.png"))
	, thrustSound_(game.assets()->loadAudio("thrust.mp3"))
	, explosionSound_(game.assets()->loadAudio("explosion.mp3"))
	, explosion_(new core::Animation(explosionTexture_, {0, 1, 2, 3, 4, 5, 6}, 7, false))
	, idle_(new core::Animation(game.assets()->loadTexture("ship.png"), {0, 1}, 2))
{
}

Player::~Player() {}

void Player::update(core::Game& game, float deltaTime)
{
	switch (state_)
	{
	case PlayerState::Flying:
	{
		accelerating_ = false;

		if (game.input()->isMouseDown())
		{
			accelerate(game, deltaTime);
		}
		else
		{
			MIX_StopTrack(thrustTrack_, 0);
			thrustTrack_ = nullptr;
		}

		idle_->update(deltaTime);

		RoundObject::update(game, deltaTime);

		return;
	}
	case PlayerState::Exploding:
	{
		explosion_->update(deltaTime);

		if (explosion_->isFinished())
			state_ = PlayerState::Dead;

		return;
	}
	case PlayerState::Dead:
	case PlayerState::Home: return;
	}
}

void Player::render(core::Renderer& renderer)
{
	switch (state_)
	{
	case PlayerState::Flying:
	{
		if (accelerating_)
		{
			float exhaustWidth = 0.0f;
			SDL_GetTextureSize(exhaustTexture_, &exhaustWidth, nullptr);

			const auto d = (idle_->frameWidth() + exhaustWidth) / 2.0f;
			const auto rad = core::radians(angle());
			const auto pos = core::Vec2f(position().x() - std::cos(rad) * d, position().y() - std::sin(rad) * d);

			renderer.drawTexture(exhaustTexture_, pos, angle());
		}

		idle_->render(renderer, position());

		return;
	}
	case PlayerState::Exploding:
	{
		explosion_->render(renderer, position());
		return;
	}
	case PlayerState::Dead:
	case PlayerState::Home: return;
	}
}

void Player::accelerate(core::Game& game, float deltaTime)
{
	if (fuel_ <= 0)
	{
		accelerating_ = false;
		return;
	}

	accelerating_ = true;

	const auto v = (game.input()->mousePosition() - position()).normalized();

	addAcceleration(v * 90.0f);
	setAngle(core::degrees(v.angle()));

	fuel_ -= 60.0f * deltaTime;

	if (fuel_ < 0)
		fuel_ = 0;

	if (!thrustTrack_)
		thrustTrack_ = game.audio()->playSound(thrustSound_, 0.1f, -1, 150, 400);
}

void Player::setState(core::Game& game, PlayerState state)
{
	state_ = state;

	if (state_ == PlayerState::Exploding)
		game.audio()->playSound(explosionSound_, 0.1f);
}

} // namespace game
