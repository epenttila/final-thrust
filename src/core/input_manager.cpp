#include "core/input_manager.h"



void InputManager::update() {
    // Clear just pressed/released states
    keysJustPressed_.clear();
    keysJustReleased_.clear();
    mouseStateJustPressed_ = 0;
    mouseStateJustReleased_ = 0;
    
    // Update mouse position
    float mouseX, mouseY;
    const auto mouseButtons = SDL_GetMouseState(&mouseX, &mouseY);
    
    const auto oldMouseX = mousePosition_.x;
    const auto oldMouseY = mousePosition_.y;
    mousePosition_ = Vector2(mouseX, mouseY);
    mouseDelta_ = Vector2(mousePosition_.x - oldMouseX, mousePosition_.y - oldMouseY);
    
    // Update mouse button states
    const auto oldMouseState = mouseState_;
    mouseState_ = mouseButtons;
    mouseStateJustPressed_ = mouseState_ & ~oldMouseState;
    mouseStateJustReleased_ = ~mouseState_ & oldMouseState;
}

void InputManager::handleEvent(const SDL_Event& event) {
    switch (event.type) {
        case SDL_EVENT_KEY_DOWN:
            if (!event.key.repeat) {
                keysPressed_.insert(event.key.scancode);
                keysJustPressed_.insert(event.key.scancode);
            }
            break;
            
        case SDL_EVENT_KEY_UP:
            keysPressed_.erase(event.key.scancode);
            keysJustReleased_.insert(event.key.scancode);
            break;
            
        case SDL_EVENT_QUIT:
            quitRequested_ = true;
            break;
    }
}

bool InputManager::isKeyPressed(SDL_Scancode key) const {
    return keysPressed_.find(key) != keysPressed_.end();
}

bool InputManager::isKeyJustPressed(SDL_Scancode key) const {
    return keysJustPressed_.find(key) != keysJustPressed_.end();
}

bool InputManager::isKeyJustReleased(SDL_Scancode key) const {
    return keysJustReleased_.find(key) != keysJustReleased_.end();
}

bool InputManager::isMouseButtonPressed(int button) const {
    return (mouseState_ & SDL_BUTTON_MASK(button)) != 0;
}

bool InputManager::isMouseButtonJustPressed(int button) const {
    return (mouseStateJustPressed_ & SDL_BUTTON_MASK(button)) != 0;
}

bool InputManager::isMouseButtonJustReleased(int button) const {
    return (mouseStateJustReleased_ & SDL_BUTTON_MASK(button)) != 0;
}

Vector2 InputManager::mousePosition() const {
    return mousePosition_;
}

Vector2 InputManager::mouseDelta() const {
    return mouseDelta_;
}

