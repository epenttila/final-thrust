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

void Game::init(std::unique_ptr<State> state)
{
	state_->push(*this, std::move(state));
}

bool Game::update()
{
	if (state_->isEmpty())
		return false;

	if (input_->isQuit())
		return false;

	if ((input_->isKeyDown(SDL_SCANCODE_LALT) || input_->isKeyDown(SDL_SCANCODE_RALT))
		&& input_->isKeyPressed(SDL_SCANCODE_RETURN))
		renderer_->toggleFullscreen();

	const auto currentTime = SDL_GetTicks();
	const auto deltaTime = (currentTime - lastFrameTime_) / 1000.0f;
	lastFrameTime_ = currentTime;

	state_->update(*this, deltaTime);
	renderer_->beginFrame();
	state_->render(*renderer_);
	renderer_->endFrame();
	input_->endFrame();

	return true;
}

void Game::event(SDL_Event* event)
{
	input_->update(event);
}

int Game::width() const
{
	return renderer_->logicalWidth();
}

int Game::height() const
{
	return renderer_->logicalHeight();
}

void Game::setFramerateLimit(std::string_view limit)
{
	SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE, limit.data());
}

} // namespace core
