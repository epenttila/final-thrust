#include <exception>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_log.h>
#include "core/Game.h"
#include "core/StateManager.h"
#include "game/MenuState.h"

int main(int, char**)
{
	try
	{
		auto game = core::Game("Final Thrust", 320, 240, 2);
		game.run(std::make_unique<game::MenuState>(game));
	}
	catch (const std::exception& e)
	{
		SDL_Log("Exception: %s", e.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
