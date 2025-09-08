#pragma once

union SDL_Event;

namespace core {

class Game;
class Renderer;

class State
{
public:
	State() = default;
	virtual ~State() = default;
	virtual void update(Game& game, float deltaTime) = 0;
	virtual void render(Renderer& renderer) = 0;
	virtual void enter(Game& game) = 0;
	virtual void exit(Game& game) = 0;
};

} // namespace core
