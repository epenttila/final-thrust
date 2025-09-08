#pragma once

#include <memory>
#include <stack>
#include "states/game_state.h"



class Application;
class Renderer;

class StateManager {
public:
    StateManager(Application* app);
    ~StateManager();
    
    void pushState(GameStatePtr state);
    void popState();
    void changeState(GameStatePtr state);
    
    void update(float deltaTime);
    void render(Renderer* renderer);
    
    bool IsEmpty() const { return states_.empty(); }

private:
    Application* app_;
    std::stack<GameStatePtr> states_;
    GameStatePtr pendingState_;
    bool shouldPop_ = false;
};

