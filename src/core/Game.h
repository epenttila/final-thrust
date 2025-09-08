#pragma once
#include <memory>
#include <string>

namespace core {

class Renderer;
class AssetManager;
class InputManager;
class AudioManager;
class StateManager;
class State;

class Game
{
public:
	explicit Game(const std::string& title, int width, int height, int scale = 1);
	~Game();
	void run(std::unique_ptr<State> state);
	int width() const;
	int height() const;
	Renderer* renderer() const { return renderer_.get(); }
	AssetManager* assets() const { return assets_.get(); }
	InputManager* input() const { return input_.get(); }
	AudioManager* audio() const { return audio_.get(); }
	StateManager* state() const { return state_.get(); }

private:
	std::unique_ptr<Renderer> renderer_;
	std::unique_ptr<AssetManager> assets_;
	std::unique_ptr<InputManager> input_;
	std::unique_ptr<AudioManager> audio_;
	std::unique_ptr<StateManager> state_;
	uint64_t lastFrameTime_ = 0;
	int fpsLimiter_ = 60;
};

} // namespace core
