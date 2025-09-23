#include "Game.h"
#include <stdexcept>
#include <SDL3/SDL.h>
#include "Renderer.h"
#include "AssetManager.h"
#include "InputManager.h"
#include "AudioManager.h"
#include "StateManager.h"
#include "State.h"

namespace core {

Game::Game(const std::string& title, int width, int height, int scale)
{
	SDL_Log("Starting %s", title.c_str());

	if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS))
		throw std::runtime_error(std::string("Failed to initialize SDL: ") + SDL_GetError());

	renderer_ = std::make_unique<Renderer>(title, width * scale, height * scale);
	renderer_->setLogicalSize(width, height);

	assets_ = std::make_unique<AssetManager>(*renderer_);
	input_ = std::make_unique<InputManager>(*renderer_);
	audio_ = std::make_unique<AudioManager>();
	state_ = std::make_unique<StateManager>();

	lastFrameTime_ = SDL_GetTicks();
}

Game::~Game()
{
	SDL_Log("Shutting down");

	state_.reset();
	audio_.reset();
	input_.reset();
	assets_.reset();
	renderer_.reset();

	SDL_Quit();
}

void Game::run(std::unique_ptr<State> state)
{
	state_->push(*this, std::move(state));

	while (!state_->isEmpty())
	{
		const auto currentTime = SDL_GetTicks();
		const auto deltaTime = (currentTime - lastFrameTime_) / 1000.0f;
		lastFrameTime_ = currentTime;

		input_->update();

		if (input_->isQuit())
			return;

		if ((input_->isKeyDown(SDL_SCANCODE_LCTRL) || input_->isKeyDown(SDL_SCANCODE_RCTRL))
			&& input_->isKeyPressed(SDL_SCANCODE_RETURN))
			renderer_->toggleFullscreen();

		state_->update(*this, deltaTime);
		renderer_->beginFrame();
		state_->render(*renderer_);
		renderer_->endFrame();

		const auto frameTime = SDL_GetTicks() - currentTime;
		const auto frameTimeLimit = static_cast<std::uint64_t>(1000 / fpsLimiter_);

		if (frameTime < frameTimeLimit)
			SDL_Delay(static_cast<std::uint32_t>(frameTimeLimit - frameTime));
	}
}

int Game::width() const
{
	return renderer_->logicalWidth();
}

int Game::height() const
{
	return renderer_->logicalHeight();
}

} // namespace core
