#pragma once

#include "entities/round_object.h"
#include <string>



class ResourceManager;

enum class PlanetType {
    Earth,
    Mars,
    Jupiter,
    Saturn,
    Uranus,
    Home
};

class Planet : public RoundObject {
public:
    Planet(const Vector2& position, PlanetType type, ResourceManager* resources);
    ~Planet() = default;
    
    void update(float deltaTime) override;
    void render(Renderer* renderer) override;
    
    PlanetType type() const { return type_; }
    float mass() const { return mass_; }
    
    static PlanetType randomType();
    static float radiusForType(PlanetType type);

private:
    PlanetType type_;
    float mass_;
    float rotationSpeed_ = 0.0f;
    ResourceManager* resources_;
};

