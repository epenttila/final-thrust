#pragma once

#include <SDL3/SDL.h>
#include <unordered_set>
#include "utils/vector2.h"



class InputManager {
public:
    InputManager() = default;
    ~InputManager() = default;
    
    void update();
    void handleEvent(const SDL_Event& event);
    
    // Keyboard
    bool isKeyPressed(SDL_Scancode key) const;
    bool isKeyJustPressed(SDL_Scancode key) const;
    bool isKeyJustReleased(SDL_Scancode key) const;
    
    // Mouse
    bool isMouseButtonPressed(int button) const;
    bool isMouseButtonJustPressed(int button) const;
    bool isMouseButtonJustReleased(int button) const;
    Vector2 mousePosition() const;
    Vector2 mouseDelta() const;
    
    // Quit request
    bool isQuitRequested() const { return quitRequested_; }
    void ClearQuitRequest() { quitRequested_ = false; }

private:
    std::unordered_set<SDL_Scancode> keysPressed_;
    std::unordered_set<SDL_Scancode> keysJustPressed_;
    std::unordered_set<SDL_Scancode> keysJustReleased_;
    
    uint32_t mouseState_ = 0;
    uint32_t mouseStateJustPressed_ = 0;
    uint32_t mouseStateJustReleased_ = 0;
    
    Vector2 mousePosition_;
    Vector2 mouseDelta_;
    
    bool quitRequested_ = false;
};

