#pragma once
#include "RoundObject.h"
#include <memory>

struct MIX_Audio;
struct MIX_Track;
struct SDL_Texture;

namespace core {

class Game;
class Animation;

} // namespace core

namespace game {

enum class PlayerState
{
	Flying,
	Exploding,
	Dead,
	Home,
};

class Player : public RoundObject
{
public:
	Player(core::Game& game, const core::Vec2f& position);
	~Player();
	void update(core::Game& game, float deltaTime) override;
	void render(core::Renderer& renderer) override;
	bool isAccelerating() const { return accelerating_; }
	float fuel() const { return fuel_; }
	PlayerState state() const { return state_; }
	void setState(core::Game& game, PlayerState state);

private:
	static constexpr int FRAMES_COUNT = 2;
	static constexpr int FRAME_WIDTH = 8;
	static constexpr int FRAME_HEIGHT = 8;

	void accelerate(core::Game& game, float deltaTime);
	void stopThrustSound();

	SDL_Texture* exhaustTexture_ = nullptr;
	SDL_Texture* explosionTexture_ = nullptr;
	MIX_Audio* thrustSound_ = nullptr;
	MIX_Track* thrustTrack_ = nullptr;
	MIX_Audio* explosionSound_ = nullptr;
	float fuel_ = 100.0f;
	bool accelerating_ = false;
	PlayerState state_ = PlayerState::Flying;
	std::unique_ptr<core::Animation> explosion_;
	std::unique_ptr<core::Animation> idle_;
};

} // namespace game
