#include "InputManager.h"
#include <SDL3/SDL.h>
#include "Renderer.h"

namespace core {

InputManager::InputManager(Renderer& renderer)
	: renderer_(renderer)
{
	keyDown_.fill(false);
}

void InputManager::update()
{
	quit_ = false;
	keyPressed_.clear();
	keyReleased_.clear();
	mousePressed_ = false;
	mouseReleased_ = false;

	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_EVENT_QUIT: quit_ = true; break;
		case SDL_EVENT_KEY_DOWN:
			if (!keyDown_[event.key.scancode])
				keyPressed_.insert(event.key.scancode);
			keyDown_[event.key.scancode] = true;
			break;
		case SDL_EVENT_KEY_UP:
			keyDown_[event.key.scancode] = false;
			keyReleased_.insert(event.key.scancode);
			break;
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			if (!mouseDown_)
				mousePressed_ = true;
			mouseDown_ = true;
			break;
		case SDL_EVENT_MOUSE_BUTTON_UP:
			mouseDown_ = false;
			mouseReleased_ = true;
			break;
		case SDL_EVENT_MOUSE_MOTION:
		{
			const auto pos = renderer_.windowToLogical(Vec2f(event.motion.x, event.motion.y));
			mouseDelta_ = mousePosition_ - pos;
			mousePosition_ = pos;
			break;
		}
		}
	}
}

} // namespace core
