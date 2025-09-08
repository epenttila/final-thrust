#pragma once

#include <memory>
#include <string>



class Renderer;
class ResourceManager;
class InputManager;
class AudioManager;
class StateManager;

class Application {
public:
    Application();
    ~Application();
    
    bool initialize(const std::string& title);
    void run();
    void shutdown();
    
    void quit() { running_ = false; }
    
    Renderer* renderer() const { return renderer_.get(); }
    ResourceManager* resourceManager() const { return resourceManager_.get(); }
    InputManager* inputManager() const { return inputManager_.get(); }
    AudioManager* audioManager() const { return audioManager_.get(); }
    StateManager* stateManager() const { return stateManager_.get(); }

private:
    void ProcessEvents();
    void update(float deltaTime);
    void render();
    
    void UpdateCursor();
    
    std::unique_ptr<Renderer> renderer_;
    std::unique_ptr<ResourceManager> resourceManager_;
    std::unique_ptr<InputManager> inputManager_;
    std::unique_ptr<AudioManager> audioManager_;
    std::unique_ptr<StateManager> stateManager_;
    
    bool running_ = false;
    uint64_t lastFrameTime_ = 0;
};

