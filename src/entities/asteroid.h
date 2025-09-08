#pragma once

#include "entities/round_object.h"



class ResourceManager;

class Asteroid : public RoundObject {
public:
    Asteroid(const Vector2& position, const Vector2& velocity, ResourceManager* resources);
    ~Asteroid() = default;
    
    void update(float deltaTime) override;
    void render(Renderer* renderer) override;

private:
    float rotationSpeed_;
    ResourceManager* resources_;
};

