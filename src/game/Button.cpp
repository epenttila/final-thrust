#include "Button.h"
#include <SDL3/SDL.h>
#include "../core/Renderer.h"
#include "../core/Game.h"
#include "../core/InputManager.h"
#include "../core/Utils.h"

namespace game {

Button::Button(const std::string& text, SDL_Texture* normalTexture, SDL_Texture* hoverTexture)
	: text_(text)
	, normalTexture_(normalTexture)
	, hoverTexture_(hoverTexture)
{
	float w = 0;
	float h = 0;
	SDL_GetTextureSize(normalTexture_, &w, &h);

	rect_ = core::RectF(0.0f, 0.0f, w, h);
}

void Button::update(core::Game& game, float)
{
	clicked_ = false;
	hovered_ = rect_.contains(game.input()->mousePosition());

	if (hovered_ && game.input()->isMousePressed())
		pressed_ = true;

	if (game.input()->isMouseReleased())
	{
		if (hovered_ && pressed_)
			clicked_ = true;

		pressed_ = false;
	}
}

void Button::render(core::Renderer& renderer)
{
	renderer.drawTexture(hovered_ && !pressed_ ? hoverTexture_ : normalTexture_, nullptr, &rect_);
	renderer.drawText(font_, rect_, text_, {255, 255, 255, 0});
}

} // namespace game
