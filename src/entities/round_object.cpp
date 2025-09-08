#include "entities/round_object.h"
#include "core/renderer.h"
#include <cmath>
#include <cstdio>



RoundObject::RoundObject(const Vector2& position, float radius)
    : position_(position), radius_(radius) {}

void RoundObject::update(float deltaTime) {
    // Update velocity with acceleration
    velocity_ += acceleration_ * deltaTime;
    
    // Update position with velocity
    position_ += velocity_ * deltaTime;
    
    // Update rotation
    angle_ += deltaTime * 30.0f; // Default rotation speed
}

void RoundObject::render(Renderer* renderer) {
    if (!texture_) return;
    
    renderer->drawTextureEx(texture_, position_, Vector2(radius_, radius_), angle_);
}

bool RoundObject::isInside(const Vector2& point, float margin) const {
    const auto centerPos = center();
    const float dx = point.x - centerPos.x;
    const float dy = point.y - centerPos.y;
    const float dd = dx * dx + dy * dy;
    const float totalRadius = radius_ + margin;
    // Use mathematically correct collision formula
    return dd <= (totalRadius * totalRadius);
}

bool RoundObject::collidesWith(const RoundObject& other) const {
    const auto distance = Vector2::distance(center(), other.center());
    return distance < (radius_ + other.radius_);
}

