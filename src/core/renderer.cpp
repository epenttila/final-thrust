#include "core/renderer.h"
#include <iostream>
#include <cmath>
#include <algorithm>



Renderer::Renderer() = default;

Renderer::~Renderer() {
    shutdown();
}

bool Renderer::initialize(const std::string& title, int windowWidth, int windowHeight) {
    windowWidth_ = windowWidth;
    windowHeight_ = windowHeight;
    
    window_ = SDL_CreateWindow(
        title.c_str(),
        windowWidth,
        windowHeight,
        SDL_WINDOW_RESIZABLE
    );
    
    if (!window_) {
        std::cerr << "Failed to create window: " << SDL_GetError() << "\n";
        return false;
    }
    
    renderer_ = SDL_CreateRenderer(window_, nullptr);
    if (!renderer_) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << "\n";
        return false;
    }
    
    SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);
    
    return true;
}

void Renderer::shutdown() {
    if (renderer_) {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }
    
    if (window_) {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }
}

void Renderer::beginFrame() {
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    SDL_RenderClear(renderer_);
}

void Renderer::endFrame() {
    SDL_RenderPresent(renderer_);
}

void Renderer::setDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawColor(renderer_, r, g, b, a);
}

void Renderer::clear() {
    SDL_RenderClear(renderer_);
}

void Renderer::drawTexture(SDL_Texture* texture, const Vector2& position, float angle) {
    if (!texture) return;
    
    float w, h;
    SDL_GetTextureSize(texture, &w, &h);
    
    SDL_FRect dst = { position.x, position.y, static_cast<float>(w), static_cast<float>(h) };
    SDL_RenderTextureRotated(renderer_, texture, nullptr, &dst, angle, nullptr, SDL_FLIP_NONE);
}

void Renderer::drawTexture(SDL_Texture* texture, const SDL_FRect* src, const SDL_FRect* dst, float angle) {
    if (!texture) return;
    SDL_RenderTextureRotated(renderer_, texture, src, dst, angle, nullptr, SDL_FLIP_NONE);
}

void Renderer::drawTextureEx(SDL_Texture* texture, const Vector2& position, const Vector2& origin, 
                             float angle, const Vector2& scale) {
    if (!texture) return;
    
    float w, h;
    SDL_GetTextureSize(texture, &w, &h);
    
    SDL_FRect dst = { 
        position.x - origin.x * scale.x,
        position.y - origin.y * scale.y,
        w * scale.x,
        h * scale.y
    };
    
    SDL_FPoint center = { origin.x * scale.x, origin.y * scale.y };
    SDL_RenderTextureRotated(renderer_, texture, nullptr, &dst, angle, &center, SDL_FLIP_NONE);
}

void Renderer::drawText(const std::string& text, const Vector2& position, SDL_Color color) {
    // This would require TTF_Font to be passed in or cached
    // For now, this is a placeholder
    (void)text;
    (void)position;
    (void)color;
}

Vector2 Renderer::windowSize() const {
    int w, h;
    SDL_GetWindowSize(window_, &w, &h);
    return Vector2(static_cast<float>(w), static_cast<float>(h));
}

float Renderer::scaleFactor() const {
    if (logicalWidth_ == 0 || logicalHeight_ == 0)
        return 1.0f;
    
    Vector2 winSize = windowSize();
    float scaleX = winSize.x / logicalWidth_;
    float scaleY = winSize.y / logicalHeight_;
    
    // Use integer scaling, so return the scale factor used by logical presentation
    return std::floor(std::min(scaleX, scaleY));
}

void Renderer::setLogicalSize(int width, int height) {
    logicalWidth_ = width;
    logicalHeight_ = height;
    // Use integer scaling for pixel-perfect rendering
    SDL_SetRenderLogicalPresentation(renderer_, width, height, 
                                    SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);
}

void Renderer::updateViewport() {
    // Viewport is handled by logical presentation
}

Vector2 Renderer::windowToLogical(const Vector2& windowPos) const {
    float logicalX, logicalY;
    SDL_RenderCoordinatesFromWindow(renderer_, windowPos.x, windowPos.y, &logicalX, &logicalY);
    return Vector2(logicalX, logicalY);
}

SDL_Texture* Renderer::createTextureFromSurface(SDL_Surface* surface) {
    if (!surface) return nullptr;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
    if (texture) {
        // Set nearest neighbor filtering for pixel-perfect rendering
        SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
    }
    return texture;
}

