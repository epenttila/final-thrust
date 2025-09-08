#pragma once
#include <memory>
#include <vector>

union SDL_Event;

namespace core {

class Game;
class Renderer;
class State;

class StateManager
{
public:
	explicit StateManager();
	~StateManager();
	void push(Game& game, std::unique_ptr<State> state);
	void pop(Game& game);
	void change(Game& game, std::unique_ptr<State> state);
	void update(Game& game, float deltaTime);
	void render(Renderer& renderer);
	bool isEmpty() const { return states_.empty(); }

private:
	std::vector<std::unique_ptr<State>> states_;
};

} // namespace core
