#include "StateManager.h"
#include "State.h"

namespace core {

StateManager::StateManager() {}

StateManager::~StateManager() {}

void StateManager::push(Game& game, std::unique_ptr<State> state)
{
	if (!isEmpty())
		states_.back()->exit(game);

	states_.push_back(std::move(state));
	states_.back()->enter(game);
}

void StateManager::pop(Game& game)
{
	if (isEmpty())
		return;

	states_.back()->exit(game);
	states_.pop_back();

	if (!isEmpty())
		states_.back()->enter(game);
}

void StateManager::change(Game& game, std::unique_ptr<State> state)
{
	pop(game);
	push(game, std::move(state));
}

void StateManager::update(Game& game, float deltaTime)
{
	if (isEmpty())
		return;

	states_.back()->update(game, deltaTime);
}

void StateManager::render(Renderer& renderer)
{
	if (isEmpty())
		return;

	states_.back()->render(renderer);
}

} // namespace core
