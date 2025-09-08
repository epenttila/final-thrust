#include "core/application.h"
#include "core/renderer.h"
#include "core/resource_manager.h"
#include "core/input_manager.h"
#include "core/audio_manager.h"
#include "core/constants.h"
#include "states/state_manager.h"
#include "states/menu_state.h"
#include <SDL3/SDL.h>
#include <iostream>



Application::Application() = default;
Application::~Application() = default;

bool Application::initialize(const std::string& title) {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS)) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << "\n";
        return false;
    }
    
    renderer_ = std::make_unique<Renderer>();
    if (!renderer_->initialize(title, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT)) {
        std::cerr << "Failed to initialize renderer\n";
        return false;
    }
    
    renderer_->setLogicalSize(INTERNAL_WIDTH, INTERNAL_HEIGHT);
    
    resourceManager_ = std::make_unique<ResourceManager>(renderer_.get());
    if (!resourceManager_->initialize()) {
        std::cerr << "Failed to initialize resource manager\n";
        return false;
    }
    
    inputManager_ = std::make_unique<InputManager>();
    
    audioManager_ = std::make_unique<AudioManager>();
    if (!audioManager_->initialize()) {
        std::cerr << "Failed to initialize audio manager\n";
        return false;
    }
    
    stateManager_ = std::make_unique<StateManager>(this);
    
    // Load custom cursor with appropriate scale
    float scale = renderer_->scaleFactor();
    auto cursor = resourceManager_->loadCustomCursor(CURSOR_IMAGE, 8, 8, scale);
    if (cursor)
        SDL_SetCursor(cursor);
    
    // Start with menu state
    stateManager_->pushState(std::make_unique<MenuState>());
    
    running_ = true;
    lastFrameTime_ = SDL_GetTicks();
    
    return true;
}

void Application::run() {
    while (running_) {
        const auto currentTime = SDL_GetTicks();
        const auto deltaTime = (currentTime - lastFrameTime_) / 1000.0f;
        lastFrameTime_ = currentTime;
        
        ProcessEvents();
        update(deltaTime);
        render();
        
        // Cap at 60 FPS
        const auto frameTime = SDL_GetTicks() - currentTime;
        if (frameTime < 16)
            SDL_Delay(16 - frameTime);
    }
}

void Application::ProcessEvents() {
    inputManager_->update();
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        inputManager_->handleEvent(event);
        
        if (event.type == SDL_EVENT_QUIT) {
            running_ = false;
        } else if (event.type == SDL_EVENT_WINDOW_RESIZED) {
            renderer_->updateViewport();
            UpdateCursor();
        }
    }
    
    if (inputManager_->isQuitRequested())
        running_ = false;
}

void Application::update(float deltaTime) {
    if (!stateManager_->IsEmpty())
        stateManager_->update(deltaTime);
}

void Application::render() {
    renderer_->beginFrame();
    renderer_->clear();
    
    if (!stateManager_->IsEmpty())
        stateManager_->render(renderer_.get());
    
    renderer_->endFrame();
}

void Application::UpdateCursor() {
    float scale = renderer_->scaleFactor();
    auto cursor = resourceManager_->loadCustomCursor(CURSOR_IMAGE, 8, 8, scale);
    if (cursor)
        SDL_SetCursor(cursor);
}

void Application::shutdown() {
    stateManager_.reset();
    audioManager_.reset();
    inputManager_.reset();
    resourceManager_.reset();
    renderer_.reset();
    
    SDL_Quit();
}

