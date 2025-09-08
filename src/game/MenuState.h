#pragma once
#include "../core/State.h"
#include <memory>
#include "../core/Vec2.h"

struct SDL_Texture;
struct MIX_Audio;
struct TTF_Font;

namespace core {

class Renderer;

} // namespace core

namespace game {

class Button;

class MenuState : public core::State
{
public:
	MenuState(core::Game& game);
	~MenuState();
	void update(core::Game& game, float deltaTime) override;
	void render(core::Renderer& renderer) override;
	void enter(core::Game& game) override;
	void exit(core::Game& game) override;

private:
	int currentLevel_ = 0;
	bool playPressed_ = false;
	SDL_Texture* backgroundTexture_ = nullptr;
	SDL_Texture* titleTexture_ = nullptr;
	TTF_Font* font_ = nullptr;
	MIX_Audio* music_ = nullptr;
	std::unique_ptr<Button> playButton_;
	std::unique_ptr<Button> leftButton_;
	std::unique_ptr<Button> rightButton_;
};

} // namespace game
