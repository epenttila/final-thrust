#include "entities/planet.h"
#include "core/renderer.h"
#include "core/resource_manager.h"
#include "utils/utils.h"



Planet::Planet(const Vector2& position, PlanetType type, ResourceManager* resources)
    : RoundObject(position, radiusForType(type)), type_(type), resources_(resources) {
    
    // Set mass based on radius
    mass_ = radius_ * radius_ * 0.1f;
    
    // Set collision radius to match visual size (half of texture size)
    // We'll update this after loading the texture
    
    // Planets don't rotate (except maybe visually, but not for physics)
    rotationSpeed_ = 0.0f;
    
    // Load appropriate texture
    switch (type) {
        case PlanetType::Earth:
            texture_ = resources_->texture("assets/images/home.png"); // Flash uses home.png for Earth
            break;
        case PlanetType::Mars:
            texture_ = resources_->texture("assets/images/mars.png");
            break;
        case PlanetType::Jupiter:
            texture_ = resources_->texture("assets/images/jupiter.png");
            break;
        case PlanetType::Saturn:
            texture_ = resources_->texture("assets/images/saturn.png");
            break;
        case PlanetType::Uranus:
            texture_ = resources_->texture("assets/images/uranus.png");
            break;
        case PlanetType::Home:
            texture_ = resources_->texture("assets/images/home.png");
            rotationSpeed_ = 0; // Home planet doesn't rotate
            break;
    }
    // Keep the radius as passed in - it's for collision, not visual size
}

void Planet::update(float) {
    // Planets are static - they don't move or rotate
}

void Planet::render(Renderer* renderer) {
    if (!texture_) return;
    
    // Get actual texture dimensions
    float textureWidth, textureHeight;
    SDL_GetTextureSize(texture_, &textureWidth, &textureHeight);
    
    // Draw planet at its natural size (textures should already be the right size)
    // The radius is for collision, not necessarily for visual size
    renderer->drawTextureEx(texture_, position_, 
                           Vector2(textureWidth / 2.0f, textureHeight / 2.0f), // Center of texture
                           angle_, 
                           Vector2(1.0f, 1.0f)); // No scaling
}

PlanetType Planet::randomType() {
    // Exclude Earth type from random selection (Earth is only for home planet)
    // Flash: Utils.getRandomInt(1, Planet.PLANET_TYPES - 1) excludes size 0 (Earth)
    const int typeIndex = utils::random(0, 3);
    const PlanetType types[] = { 
        PlanetType::Mars, PlanetType::Jupiter, 
        PlanetType::Saturn, PlanetType::Uranus 
    };
    return types[typeIndex];
}

float Planet::radiusForType(PlanetType type) {
    // Match Flash values exactly
    switch(type) {
        case PlanetType::Earth:
            return 8.0f;   // Flash: radius = 8
        case PlanetType::Jupiter:
            return 15.0f;  // Flash: radius = 15
        case PlanetType::Uranus:
            return 12.0f;  // Flash: radius = 12
        case PlanetType::Saturn:
            return 10.0f;  // Flash: radius = 10
        case PlanetType::Mars:
            return 8.0f;   // Flash: radius = 8
        case PlanetType::Home:
            return 8.0f;   // Home planet uses Earth image (size 0)
        default:
            return 8.0f;
    }
}

