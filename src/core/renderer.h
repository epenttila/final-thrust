#pragma once

#include <SDL3/SDL.h>
#include <memory>
#include <string>
#include "utils/vector2.h"



class Renderer {
public:
    Renderer();
    ~Renderer();
    
    bool initialize(const std::string& title, int windowWidth, int windowHeight);
    void shutdown();
    
    void beginFrame();
    void endFrame();
    
    void setDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
    void clear();
    
    void drawTexture(SDL_Texture* texture, const Vector2& position, float angle = 0.0f);
    void drawTexture(SDL_Texture* texture, const SDL_FRect* src, const SDL_FRect* dst, float angle = 0.0f);
    void drawTextureEx(SDL_Texture* texture, const Vector2& position, const Vector2& origin, 
                       float angle = 0.0f, const Vector2& scale = {1.0f, 1.0f});
    
    void drawText(const std::string& text, const Vector2& position, SDL_Color color);
    
    SDL_Window* window() const { return window_; }
    SDL_Renderer* sdlRenderer() const { return renderer_; }
    
    Vector2 windowSize() const;
    Vector2 logicalSize() const { return Vector2(logicalWidth_, logicalHeight_); }
    float scaleFactor() const;
    
    void setLogicalSize(int width, int height);
    void updateViewport();
    
    // Convert window coordinates to logical coordinates
    Vector2 windowToLogical(const Vector2& windowPos) const;
    
    SDL_Texture* createTextureFromSurface(SDL_Surface* surface);

private:
    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;
    
    int windowWidth_ = 0;
    int windowHeight_ = 0;
    int logicalWidth_ = 0;
    int logicalHeight_ = 0;
};

