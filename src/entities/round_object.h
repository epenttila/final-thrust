#pragma once

#include "utils/vector2.h"
#include <SDL3/SDL.h>



class Renderer;

class RoundObject {
public:
    RoundObject(const Vector2& position, float radius);
    virtual ~RoundObject() = default;
    
    virtual void update(float deltaTime);
    virtual void render(Renderer* renderer);
    
    Vector2 center() const { return position_; }  // position_ is already the center
    Vector2 position() const { return position_; }
    float radius() const { return radius_; }
    
    void setPosition(const Vector2& pos) { position_ = pos; }
    
    bool isInside(const Vector2& point, float margin = 0.0f) const;
    bool collidesWith(const RoundObject& other) const;
    
    Vector2 velocity_;
    Vector2 acceleration_;

protected:
    Vector2 position_;
    float radius_;
    float angle_ = 0.0f;
    SDL_Texture* texture_ = nullptr;
};

