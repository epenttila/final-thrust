#include "core/resource_manager.h"
#include "core/renderer.h"
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <iostream>
#include <algorithm>



ResourceManager::ResourceManager(Renderer* renderer) : renderer_(renderer) {}

ResourceManager::~ResourceManager() {
    shutdown();
}

bool ResourceManager::initialize() {
    // SDL3_image doesn't require explicit initialization
    
    // Initialize SDL_ttf
    if (!TTF_Init()) {
        std::cerr << "Failed to initialize SDL_ttf: " << SDL_GetError() << "\n";
        return false;
    }
    
    // TODO: Initialize SDL3_mixer with new API
    // if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    //     std::cerr << "Failed to initialize SDL_mixer: " << Mix_GetError() << "\n";
    //     return false;
    // }
    
    return true;
}

void ResourceManager::shutdown() {
    clearAll();
    
    // TODO: Close SDL3_mixer with new API
    // Mix_CloseAudio();
    TTF_Quit();
    // SDL3_image doesn't require explicit cleanup
}

SDL_Texture* ResourceManager::loadTexture(const std::string& path) {
    // Check if already loaded
    auto it = textures_.find(path);
    if (it != textures_.end())
        return it->second.get();
    
    // Load the surface
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "Failed to load image " << path << ": " << SDL_GetError() << "\n";
        return nullptr;
    }
    
    // Create texture from surface
    auto* texture = renderer_->createTextureFromSurface(surface);
    SDL_DestroySurface(surface);
    
    if (!texture) {
        std::cerr << "Failed to create texture from " << path << ": " << SDL_GetError() << "\n";
        return nullptr;
    }
    
    // Set nearest neighbor filtering for pixel-perfect rendering
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
    
    // Store and return
    textures_[path] = std::unique_ptr<SDL_Texture, TextureDeleter>(texture);
    return texture;
}

SDL_Texture* ResourceManager::texture(const std::string& path) {
    auto it = textures_.find(path);
    if (it != textures_.end())
        return it->second.get();
    return loadTexture(path);
}

void ResourceManager::unloadTexture(const std::string& path) {
    textures_.erase(path);
}

TTF_Font* ResourceManager::loadFont(const std::string& path, int size) {
    const auto key = path + "_" + std::to_string(size);
    
    // Check if already loaded
    auto it = fonts_.find(key);
    if (it != fonts_.end())
        return it->second.get();
    
    // Load the font
    TTF_Font* font = TTF_OpenFont(path.c_str(), size);
    if (!font) {
        std::cerr << "Failed to load font " << path << ": " << SDL_GetError() << "\n";
        return nullptr;
    }
    
    // Disable font hinting for sharper pixel-perfect text
    TTF_SetFontHinting(font, TTF_HINTING_NONE);
    
    // Store and return
    fonts_[key] = std::unique_ptr<TTF_Font, FontDeleter>(font);
    return font;
}

TTF_Font* ResourceManager::font(const std::string& key) {
    auto it = fonts_.find(key);
    if (it != fonts_.end())
        return it->second.get();
    return nullptr;
}

MIX_Audio* ResourceManager::loadAudio(const std::string& path, MIX_Mixer* mixer) {
    if (!mixer) return nullptr;
    
    auto it = audioFiles_.find(path);
    if (it != audioFiles_.end())
        return it->second.get();
    
    MIX_Audio* audio = MIX_LoadAudio(mixer, path.c_str(), false);  // false = don't predecode
    if (!audio) {
        std::cerr << "Failed to load audio " << path << ": " << SDL_GetError() << "\n";
        return nullptr;
    }
    
    audioFiles_[path] = std::unique_ptr<MIX_Audio, AudioDeleter>(audio);
    return audio;
}

MIX_Audio* ResourceManager::audio(const std::string& path, MIX_Mixer* mixer) {
    auto it = audioFiles_.find(path);
    if (it != audioFiles_.end())
        return it->second.get();
    return loadAudio(path, mixer);
}

SDL_Cursor* ResourceManager::loadCustomCursor(const std::string& path, int hotX, int hotY, float scale) {
    if (customCursor_ && cursorScale_ == scale)
        return customCursor_.get();
    
    // Clear existing cursor if scale changed
    if (cursorScale_ != scale)
        customCursor_.reset();
    
    SDL_Surface* rawSurface = IMG_Load(path.c_str());
    if (!rawSurface) {
        std::cerr << "Failed to load cursor image " << path << ": " << SDL_GetError() << "\n";
        return nullptr;
    }
    
    // Convert to RGBA8888 format for cursor creation
    SDL_Surface* surface = SDL_ConvertSurface(rawSurface, SDL_PIXELFORMAT_RGBA8888);
    SDL_DestroySurface(rawSurface);
    
    if (!surface) {
        std::cerr << "Failed to convert cursor surface format: " << SDL_GetError() << "\n";
        return nullptr;
    }
    
    SDL_Surface* scaledSurface = surface;
    int scaledHotX = hotX;
    int scaledHotY = hotY;
    
    // Scale the surface if needed
    if (scale != 1.0f) {
        int newWidth = static_cast<int>(surface->w * scale);
        int newHeight = static_cast<int>(surface->h * scale);
        
        // Ensure minimum size of 1x1
        newWidth = std::max(1, newWidth);
        newHeight = std::max(1, newHeight);
        
        scaledSurface = SDL_CreateSurface(newWidth, newHeight, SDL_PIXELFORMAT_RGBA8888);
        if (scaledSurface) {
            // Use nearest neighbor scaling to preserve pixel art look
            SDL_BlitSurfaceScaled(surface, nullptr, scaledSurface, nullptr, SDL_SCALEMODE_NEAREST);
            scaledHotX = static_cast<int>(hotX * scale);
            scaledHotY = static_cast<int>(hotY * scale);
        } else {
            std::cerr << "Failed to create scaled cursor surface\n";
            scaledSurface = surface; // Fall back to original
        }
    }
    
    SDL_Cursor* cursor = SDL_CreateColorCursor(scaledSurface, scaledHotX, scaledHotY);
    
    if (scaledSurface != surface)
        SDL_DestroySurface(scaledSurface);
    SDL_DestroySurface(surface);
    
    if (!cursor) {
        std::cerr << "Failed to create cursor: " << SDL_GetError() << "\n";
        return nullptr;
    }
    
    customCursor_ = std::unique_ptr<SDL_Cursor, CursorDeleter>(cursor);
    cursorScale_ = scale;
    return cursor;
}

void ResourceManager::clearAll() {
    textures_.clear();
    fonts_.clear();
    audioFiles_.clear();
    customCursor_.reset();
}

