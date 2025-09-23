#include "Renderer.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>

namespace core {

Renderer::Renderer(const std::string& title, int windowWidth, int windowHeight)
	: windowWidth_(windowWidth)
	, windowHeight_(windowHeight)
{
	if (window_ = SDL_CreateWindow(
			title.c_str(),
			windowWidth,
			windowHeight,
			(fullscreen_ ? SDL_WINDOW_FULLSCREEN : 0) | SDL_WINDOW_RESIZABLE
		);
		!window_)
		throw std::runtime_error(std::string("Failed to create window: ") + SDL_GetError());

	if (renderer_ = SDL_CreateRenderer(window_, nullptr); !renderer_)
		throw std::runtime_error(std::string("Failed to create renderer: ") + SDL_GetError());

	SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);
}

Renderer::~Renderer()
{
	if (renderer_)
	{
		SDL_DestroyRenderer(renderer_);
		renderer_ = nullptr;
	}

	if (window_)
	{
		SDL_DestroyWindow(window_);
		window_ = nullptr;
	}
}

void Renderer::beginFrame()
{
	SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
	SDL_RenderClear(renderer_);
}

void Renderer::endFrame()
{
	SDL_RenderPresent(renderer_);
}

void Renderer::drawTexture(SDL_Texture* texture, const Vec2f& position)
{
	float width = 0;
	float height = 0;
	SDL_GetTextureSize(texture, &width, &height);

	const auto dst = SDL_FRect{position.x() - width / 2.0f, position.y() - height / 2.0f, width, height};

	SDL_RenderTexture(renderer_, texture, nullptr, &dst);
}

void Renderer::drawTexture(SDL_Texture* texture, const RectF& dst)
{
	const auto d = SDL_FRect{dst.x(), dst.y(), dst.width(), dst.height()};

	SDL_RenderTexture(renderer_, texture, nullptr, &d);
}

void Renderer::drawTexture(SDL_Texture* texture, const Vec2f& position, float angle, float scale)
{
	float width = 0;
	float height = 0;
	SDL_GetTextureSize(texture, &width, &height);

	const auto dst = SDL_FRect{
		position.x() - scale * width / 2.0f,
		position.y() - scale * height / 2.0f,
		scale * width,
		scale * height
	};
	SDL_RenderTextureRotated(renderer_, texture, nullptr, &dst, angle, nullptr, SDL_FLIP_NONE);
}

void Renderer::drawTexture(SDL_Texture* texture, const RectF& src, const Vec2f& position, float angle, float scale)
{
	const auto s = SDL_FRect{src.x(), src.y(), src.width(), src.height()};
	const auto d = SDL_FRect{
		position.x() - scale * src.width() / 2.0f,
		position.y() - scale * src.height() / 2.0f,
		scale * src.width(),
		scale * src.height()
	};
	SDL_RenderTextureRotated(renderer_, texture, &s, &d, angle, nullptr, SDL_FLIP_NONE);
}

void Renderer::drawTexture(SDL_Texture* texture, const RectF* src, const RectF* dst, float angle, const Vec2f* center)
{
	const auto s = src ? SDL_FRect{src->x(), src->y(), src->width(), src->height()} : SDL_FRect{};
	const auto d = dst ? SDL_FRect{dst->x(), dst->y(), dst->width(), dst->height()} : SDL_FRect{};
	const auto c = center ? SDL_FPoint{center->x(), center->y()} : SDL_FPoint{};

	SDL_RenderTextureRotated(
		renderer_,
		texture,
		src ? &s : nullptr,
		dst ? &d : nullptr,
		angle,
		center ? &c : nullptr,
		SDL_FLIP_NONE
	);
}

void Renderer::setLogicalSize(int width, int height)
{
	logicalWidth_ = width;
	logicalHeight_ = height;

	SDL_SetRenderLogicalPresentation(renderer_, width, height, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);
}

Vec2f Renderer::windowToLogical(const Vec2f& position) const
{
	float logicalX = 0;
	float logicalY = 0;

	SDL_RenderCoordinatesFromWindow(renderer_, position.x(), position.y(), &logicalX, &logicalY);

	return Vec2f(logicalX, logicalY);
}

SDL_Texture* Renderer::createTextureFromSurface(SDL_Surface* surface)
{
	auto* texture = SDL_CreateTextureFromSurface(renderer_, surface);

	if (!texture)
		throw std::runtime_error(std::string("Failed to create texture from surface: ") + SDL_GetError());

	return texture;
}

namespace {

SDL_Texture* makeTextureFromText(Renderer& renderer, TTF_Font* font, const std::string& text, SDL_Color color)
{
	auto* surface = TTF_RenderText_Solid(font, text.c_str(), 0, color);

	if (!surface)
		throw std::runtime_error(std::string("Failed to render text: ") + SDL_GetError());

	auto* texture = renderer.createTextureFromSurface(surface);

	SDL_DestroySurface(surface);
	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

	return texture;
}

} // namespace

void Renderer::drawText(
	TTF_Font* font,
	Vec2f position,
	float horizontal,
	float vertical,
	const std::string& text,
	SDL_Color color
)
{
	if (text.empty())
		return;

	auto* texture = makeTextureFromText(*this, font, text, color);

	float w = 0;
	float h = 0;
	SDL_GetTextureSize(texture, &w, &h);

	const auto r =
		RectF(position.x() + (horizontal - 1.0f) / 2.0f * w, position.y() + (vertical - 1.0f) / 2.0f * h, w, h);

	drawTexture(texture, nullptr, &r);

	SDL_DestroyTexture(texture);
}

void Renderer::drawText(TTF_Font* font, const RectF& rect, const std::string& text, SDL_Color color)
{
	if (text.empty())
		return;

	auto* texture = makeTextureFromText(*this, font, text, color);

	float w = 0;
	float h = 0;
	SDL_GetTextureSize(texture, &w, &h);

	const auto r = RectF(rect.center().x() - w / 2.0f, rect.center().y() - h / 2.0f, w, h);

	drawTexture(texture, nullptr, &r);

	SDL_DestroyTexture(texture);
}

void Renderer::toggleFullscreen()
{
	fullscreen_ = !fullscreen_;
	SDL_SetWindowFullscreen(window_, fullscreen_);
}

void Renderer::drawRectangle(const RectF& rectangle, SDL_Color color)
{
	const auto r = SDL_FRect{rectangle.x(), rectangle.y(), rectangle.width(), rectangle.height()};

	SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
	SDL_RenderRect(renderer_, &r);
}


void Renderer::fillRectangle(const RectF& rectangle, SDL_Color color)
{
	const auto r = SDL_FRect{rectangle.x(), rectangle.y(), rectangle.width(), rectangle.height()};

	SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer_, &r);
}

} // namespace core
