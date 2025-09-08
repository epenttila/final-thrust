#pragma once

#include "states/game_state.h"
#include "utils/vector2.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>



class MenuState : public IGameState {
public:
    MenuState(int level = 0);
    ~MenuState() = default;
    
    void onEnter(Application* app) override;
    void onExit() override;
    void update(float deltaTime) override;
    void render(Renderer* renderer) override;

private:
    void handleInput();
    void updateButtons();
    void startGame();
    void levelUp();
    void levelDown();
    void drawRetroTitle(Renderer* renderer);
    
    struct Button {
        SDL_FRect rect;
        SDL_Texture* normalTexture = nullptr;
        SDL_Texture* hoverTexture = nullptr;
        bool hovered = false;
        
        bool contains(const Vector2& point) const {
            return point.x >= rect.x && point.x <= rect.x + rect.w &&
                   point.y >= rect.y && point.y <= rect.y + rect.h;
        }
    };
    
    int currentLevel_ = 0;
    float playTimer_ = 0.0f;
    bool playPressed_ = false;
    
    SDL_Texture* backgroundTexture_ = nullptr;
    SDL_Texture* titleTexture_ = nullptr;
    TTF_Font* font_ = nullptr;
    
    Button playButton_;
    Button leftButton_;
    Button rightButton_;
    
    SDL_Texture* levelTextTexture_ = nullptr;
    SDL_FRect levelTextRect_;
};

