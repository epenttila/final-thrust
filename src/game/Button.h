#pragma once
#include "../core/Entity.h"
#include "../core/Vec2.h"
#include "../core/Rect.h"
#include <string>

struct SDL_Texture;
struct TTF_Font;

namespace core {

class Renderer;
class Game;

} // namespace core

namespace game {

class Button
{
public:
	Button(const std::string& text, SDL_Texture* normalTexture, SDL_Texture* hoverTexture);
	void update(core::Game& game, float deltaTime);
	void render(core::Renderer& renderer);
	const core::RectF& rect() const { return rect_; }
	void setRect(const core::RectF& rect) { rect_ = rect; }
	const TTF_Font* font() const { return font_; }
	void setFont(TTF_Font* font) { font_ = font; }
	bool hovered() const { return hovered_; }
	bool pressed() const { return pressed_; }
	bool clicked() const { return clicked_; }

private:
	TTF_Font* font_ = nullptr;
	std::string text_;
	core::RectF rect_;
	SDL_Texture* normalTexture_ = nullptr;
	SDL_Texture* hoverTexture_ = nullptr;
	bool hovered_ = false;
	bool pressed_ = false;
	bool clicked_ = false;
};

} // namespace game
