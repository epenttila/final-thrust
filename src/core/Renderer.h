#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "Vec2.h"
#include "Rect.h"

struct SDL_Texture;
struct SDL_Window;
struct SDL_Renderer;
struct SDL_Surface;
struct TTF_Font;
struct SDL_Color;

namespace core {

class Renderer
{
public:
	Renderer(const std::string& title, int windowWidth, int windowHeight);
	~Renderer();
	void beginFrame();
	void endFrame();
	void drawTexture(SDL_Texture* texture, const Vec2f& position);
	void drawTexture(SDL_Texture* texture, const RectF& dst);
	void drawTexture(SDL_Texture* texture, const Vec2f& position, float angle, float scale = 1.0f);
	void drawTexture(
		SDL_Texture* texture,
		const RectF& src,
		const Vec2f& position,
		float angle = 0.0f,
		float scale = 1.0f
	);
	void drawTexture(
		SDL_Texture* texture,
		const RectF* src,
		const RectF* dst,
		float angle = 0.0f,
		const Vec2f* center = nullptr
	);
	void drawText(
		TTF_Font* font,
		Vec2f position,
		float horizontal,
		float vertical,
		const std::string& text,
		SDL_Color color
	);
	void drawText(TTF_Font* font, RectF rectangle, const std::string& text, SDL_Color color);
	void setLogicalSize(int width, int height);
	Vec2f windowToLogical(const Vec2f& windowPos) const;
	int windowWidth() const { return windowWidth_; }
	int windowHeight() const { return windowHeight_; }
	int logicalWidth() const { return logicalWidth_; }
	int logicalHeight() const { return logicalHeight_; }
	RectF windowRect() const
	{
		return RectF(0.0f, 0.0f, static_cast<float>(windowWidth_), static_cast<float>(windowHeight_));
	}
	RectF logicalRect() const
	{
		return RectF(0.0f, 0.0f, static_cast<float>(logicalWidth_), static_cast<float>(logicalHeight_));
	}
	SDL_Texture* createTextureFromSurface(SDL_Surface* surface);

private:
	SDL_Window* window_ = nullptr;
	SDL_Renderer* renderer_ = nullptr;
	int windowWidth_ = 0;
	int windowHeight_ = 0;
	int logicalWidth_ = 0;
	int logicalHeight_ = 0;
};

} // namespace core
