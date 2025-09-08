#pragma once

#include <memory>



class Application;
class Renderer;
class ResourceManager;
class InputManager;
class AudioManager;

class IGameState {
public:
    virtual ~IGameState() = default;
    
    virtual void onEnter(Application* app) = 0;
    virtual void onExit() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(Renderer* renderer) = 0;
    
protected:
    Application* app_ = nullptr;
};

using GameStatePtr = std::unique_ptr<IGameState>;

