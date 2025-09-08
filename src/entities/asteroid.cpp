#include "entities/asteroid.h"
#include "core/renderer.h"
#include "core/resource_manager.h"
#include "utils/utils.h"



Asteroid::Asteroid(const Vector2& position, const Vector2& velocity, ResourceManager* resources)
    : RoundObject(position, 3.0f), resources_(resources) { // 6x6 texture, so radius = 3
    
    velocity_ = velocity;
    rotationSpeed_ = (utils::random(0.0f, 1.0f) - 0.5f) * 90.0f; // Flash: (random() - 0.5) * 90
    texture_ = resources_->texture("assets/images/asteroid.png");
}

void Asteroid::update(float deltaTime) {
    // Update physics
    velocity_ += acceleration_ * deltaTime;
    position_ += velocity_ * deltaTime;
    
    // Rotate
    angle_ += rotationSpeed_ * deltaTime;
    angle_ = utils::normalizeAngle(angle_);
    
    // Reset acceleration for next frame
    acceleration_ = Vector2(0, 0);
}

void Asteroid::render(Renderer* renderer) {
    if (!texture_) return;
    
    renderer->drawTextureEx(texture_, position_, Vector2(radius_, radius_), angle_);
}

