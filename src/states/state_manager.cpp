#include "states/state_manager.h"
#include "core/application.h"



StateManager::StateManager(Application* app) : app_(app) {}

StateManager::~StateManager() {
    while (!states_.empty()) {
        states_.top()->onExit();
        states_.pop();
    }
}

void StateManager::pushState(GameStatePtr state) {
    if (state) {
        state->onEnter(app_);
        states_.push(std::move(state));
    }
}

void StateManager::popState() {
    if (!states_.empty()) {
        states_.top()->onExit();
        states_.pop();
    }
}

void StateManager::changeState(GameStatePtr state) {
    if (!states_.empty()) {
        states_.top()->onExit();
        states_.pop();
    }
    pushState(std::move(state));
}

void StateManager::update(float deltaTime) {
    // Handle pending state change
    if (pendingState_) {
        changeState(std::move(pendingState_));
        pendingState_.reset();
    }
    
    if (shouldPop_) {
        popState();
        shouldPop_ = false;
    }
    
    if (!states_.empty())
        states_.top()->update(deltaTime);
}

void StateManager::render(Renderer* renderer) {
    if (!states_.empty())
        states_.top()->render(renderer);
}

