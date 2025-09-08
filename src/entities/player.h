#pragma once

#include "entities/round_object.h"
#include <vector>



class ResourceManager;
class InputManager;

class Player : public RoundObject {
public:
    Player(const Vector2& position, ResourceManager* resources);
    ~Player() = default;
    
    void update(float deltaTime) override;
    void render(Renderer* renderer) override;
    
    void handleInput(InputManager* input, Renderer* renderer);
    void accelerate(float deltaTime);
    void rotate(float direction, float deltaTime);
    
    void explode();
    bool isDead() const { return dead_; }
    bool isAccelerating() const { return accelerating_; }
    float fuel() const { return fuel_; }
    
    void reset();

private:
    
    struct ExhaustParticle {
        Vector2 position;
        Vector2 velocity;
        float lifetime;
        float alpha;
    };
    
    ResourceManager* resources_;
    SDL_Texture* exhaustTexture_ = nullptr;
    SDL_Texture* explosionTexture_ = nullptr;
    
    float fuel_ = 100.0f;
    float rotation_ = 0.0f;
    bool accelerating_ = false;
    bool dead_ = false;
    bool exploding_ = false;
    float explosionTimer_ = 0.0f;
    
    // Animation properties
    float animationTimer_ = 0.0f;
    int currentFrame_ = 0;
    static constexpr int FRAMES_COUNT = 2;
    static constexpr int FRAME_WIDTH = 8;
    static constexpr int FRAME_HEIGHT = 8;
    static constexpr float ANIMATION_SPEED = 2.0f; // 2 FPS
    
    std::vector<ExhaustParticle> exhaustParticles_;
};

