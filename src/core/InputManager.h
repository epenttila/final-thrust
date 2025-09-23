#pragma once
#include <unordered_set>
#include <array>
#include "Vec2.h"

union SDL_Event;

namespace core {

class Renderer;

class InputManager
{
public:
	InputManager(Renderer& renderer);
	~InputManager() = default;
	void update(SDL_Event* event);
	bool isKeyDown(int key) const { return keyDown_[key]; }
	bool isKeyPressed(int key) const { return keyPressed_.contains(key); }
	bool isKeyReleased(int key) const { return keyReleased_.contains(key); }
	bool isMouseDown() const { return mouseDown_; }
	bool isMousePressed() const { return mousePressed_; }
	bool isMouseReleased() const { return mouseReleased_; }
	Vec2f mousePosition() const { return mousePosition_; }
	Vec2f mouseDelta() const { return mouseDelta_; }
	bool isQuit() const { return quit_; }
	void endFrame();

private:
	Renderer& renderer_;
	bool quit_ = false;
	std::array<bool, 512> keyDown_;
	std::unordered_set<int> keyPressed_;
	std::unordered_set<int> keyReleased_;
	bool mouseDown_ = false;
	bool mousePressed_ = false;
	bool mouseReleased_ = false;
	Vec2f mousePosition_;
	Vec2f mouseDelta_;
};

} // namespace core
