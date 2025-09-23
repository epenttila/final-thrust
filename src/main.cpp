#include <exception>
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_log.h>
#include "core/Game.h"
#include "core/StateManager.h"
#include "game/MenuState.h"

SDL_AppResult SDL_AppInit(void** appstate, int, char**)
{
	try
	{
		auto* game = new core::Game("Final Thrust", 320, 180, 4);
		*appstate = reinterpret_cast<void*>(game);
		game->setFramerateLimit("60");
		game->init(std::make_unique<game::MenuState>(*game));
	}
	catch (const std::exception& e)
	{
		SDL_Log("Exception: %s", e.what());
		return SDL_APP_FAILURE;
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
	try
	{
		if (!reinterpret_cast<core::Game*>(appstate)->update())
			return SDL_APP_SUCCESS;
	}
	catch (const std::exception& e)
	{
		SDL_Log("Exception: %s", e.what());
		return SDL_APP_FAILURE;
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	try
	{
		reinterpret_cast<core::Game*>(appstate)->event(event);
	}
	catch (const std::exception& e)
	{
		SDL_Log("Exception: %s", e.what());
		return SDL_APP_FAILURE;
	}

	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult)
{
	delete reinterpret_cast<core::Game*>(appstate);
}
