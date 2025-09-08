#pragma once

#include "states/game_state.h"
#include "utils/vector2.h"
#include "entities/round_object.h"
#include <vector>
#include <memory>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>



class Player;
class Planet;
class Asteroid;

class PlayState : public IGameState {
public:
    PlayState(int level, int maxLevel);
    ~PlayState() = default;
    
    void onEnter(Application* app) override;
    void onExit() override;
    void update(float deltaTime) override;
    void render(Renderer* renderer) override;

private:
    void initWorld(int seed);
    void applyGravity(RoundObject* object, float coefficient);
    bool isPlayerCollidingWithWorld();
    void nextLevel();
    void restartLevel();
    
    int level_;
    int maxLevel_;
    int maxAsteroids_;
    int maxPlanets_;
    
    std::unique_ptr<Player> player_;
    std::unique_ptr<Planet> homePlanet_;
    std::vector<std::unique_ptr<Planet>> planets_;
    std::vector<std::unique_ptr<Asteroid>> asteroids_;
    
    SDL_Texture* backgroundTexture_ = nullptr;
    TTF_Font* font_ = nullptr;
    SDL_Texture* fuelTextTexture_ = nullptr;
    SDL_FRect fuelTextRect_;
    
    // Sound effects
    MIX_Audio* thrustSound_ = nullptr;
    MIX_Audio* explosionSound_ = nullptr;
    bool playerAccelerating_ = false;
    bool playerExploding_ = false;
    int thrustSoundFrameCounter_ = 0;
    
    bool levelComplete_ = false;
    bool levelFailed_ = false;
    float transitionTimer_ = 0.0f;
};

