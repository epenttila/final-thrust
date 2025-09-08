#include "Animation.h"
#include <set>
#include <SDL3/SDL.h>
#include "Renderer.h"

namespace core {

Animation::Animation(SDL_Texture* texture, std::vector<int> frames, float frameRate, bool loop)
	: texture_(texture)
	, frames_(frames)
	, frameRate_(frameRate)
	, loop_(loop)
{
	float width = 0;
	float height = 0;
	SDL_GetTextureSize(texture, &width, &height);

	const auto frameCount = std::set(frames.begin(), frames.end()).size();
	frameWidth_ = width / frameCount;
	frameHeight_ = height;
}

void Animation::update(float deltaTime)
{
	currentFrame_ += deltaTime * frameRate_;

	if (currentFrame_ >= frames_.size())
	{
		if (loop_)
			currentFrame_ = 0;
		else
			finished_ = true;
	}
}

void Animation::render(Renderer& renderer, const RectF& rect, float angle)
{
	if (!visible_)
		return;

	const auto frame = frames_[std::min(frames_.size() - 1, static_cast<std::size_t>(currentFrame_))];
	const auto src = RectF(frame * frameWidth_, 0.0f, frameWidth_, frameHeight_);

	renderer.drawTexture(texture_, &src, &rect, angle);
}

void Animation::render(Renderer& renderer, const Vec2f& position, float angle)
{
	if (!visible_)
		return;

	const auto frame = frames_[std::min(frames_.size() - 1, static_cast<std::size_t>(currentFrame_))];
	const auto src = RectF(frame * frameWidth_, 0.0f, frameWidth_, frameHeight_);

	renderer.drawTexture(texture_, src, position, angle);
}

} // namespace core
