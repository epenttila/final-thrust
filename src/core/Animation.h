#pragma once
#include <vector>
#include "Rect.h"

struct SDL_Texture;

namespace core {

class Renderer;

class Animation
{
public:
	Animation(SDL_Texture* texture, std::vector<int> frames, float frameRate, bool loop = true);
	void update(float deltaTime);
	void render(Renderer& renderer, const RectF& dst, float angle = 0.0f);
	void render(Renderer& renderer, const Vec2f& position, float angle = 0.0f);
	void setVisible(bool visible) { visible_ = visible; }
	bool isVisible() const { return visible_; }
	float frameWidth() const { return frameWidth_; }
	float frameHeight() const { return frameHeight_; }
	bool isFinished() const { return finished_; }

private:
	SDL_Texture* texture_ = nullptr;
	std::vector<int> frames_;
	float frameRate_ = 0.0f;
	bool loop_ = false;
	float frameWidth_ = 0.0f;
	float frameHeight_ = 0.0f;
	float currentFrame_ = 0.0f;
	bool visible_ = true;
	bool finished_ = false;
};

} // namespace core
