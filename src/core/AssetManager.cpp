#include "AssetManager.h"
#include "Renderer.h"
#include <stdexcept>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

namespace core {

namespace {

std::string makeAssetPath(const std::string& dir, const std::string& filename)
{
	return
#if !defined(SDL_PLATFORM_ANDROID)
		std::string(SDL_GetBasePath()) + "../share/" +
#endif
		dir + "/" + filename;
}

} // namespace

AssetManager::AssetManager(Renderer& renderer)
	: renderer_(renderer)
{
	if (!TTF_Init())
		throw std::runtime_error(std::string("Failed to initialize SDL_ttf: ") + SDL_GetError());
}

AssetManager::~AssetManager()
{
	fonts_.clear();

	TTF_Quit();
}

TTF_Font* AssetManager::loadFont(const std::string& filename, float size)
{
	const auto path = makeAssetPath("fonts", filename);
	const auto key = path + "@" + std::to_string(size);

	auto it = fonts_.find(key);

	if (it != fonts_.end())
		return it->second.get();

	auto* font = TTF_OpenFont(path.c_str(), size);

	if (!font)
		throw std::runtime_error("Failed to load font " + path + ": " + SDL_GetError());

	return fonts_.insert({key, std::unique_ptr<TTF_Font, FontDeleter>(font)}).first->second.get();
}

MIX_Audio* AssetManager::loadAudio(const std::string& filename)
{
	const auto path = makeAssetPath("sounds", filename);

	auto it = audio_.find(path);

	if (it != audio_.end())
		return it->second.get();

	auto* audio = MIX_LoadAudio(nullptr, path.c_str(), false);

	if (!audio)
		throw std::runtime_error("Failed to load audio " + path + ": " + SDL_GetError());

	return audio_.insert({path, std::unique_ptr<MIX_Audio, AudioDeleter>(audio)}).first->second.get();
}

SDL_Cursor* AssetManager::loadCursor(const std::string& filename, int hotX, int hotY)
{
	const auto path = makeAssetPath("images", filename);

	auto it = cursors_.find(path);

	if (it != cursors_.end())
		return it->second.get();

	auto* surface = IMG_Load(path.c_str());

	if (!surface)
		throw std::runtime_error("Failed to load image " + path + ": " + SDL_GetError());

	auto* cursor = SDL_CreateColorCursor(surface, hotX, hotY);
	SDL_DestroySurface(surface);

	if (!cursor)
		throw std::runtime_error("Failed to create cursor from " + path + ": " + SDL_GetError());

	return cursors_.insert({path, std::unique_ptr<SDL_Cursor, CursorDeleter>(cursor)}).first->second.get();
}

SDL_Texture* AssetManager::loadTexture(const std::string& filename)
{
	const auto path = makeAssetPath("images", filename);

	auto it = textures_.find(path);

	if (it != textures_.end())
		return it->second.get();

	auto* surface = IMG_Load(path.c_str());

	if (!surface)
		throw std::runtime_error("Failed to load image " + path + ": " + SDL_GetError());

	auto* texture = renderer_.createTextureFromSurface(surface);

	SDL_DestroySurface(surface);
	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

	if (!texture)
		throw std::runtime_error("Failed to create texture from " + path + ": " + SDL_GetError());

	return textures_.insert({path, std::unique_ptr<SDL_Texture, TextureDeleter>(texture)}).first->second.get();
}

void AssetManager::FontDeleter::operator()(TTF_Font* font) const
{
	if (font)
		TTF_CloseFont(font);
}

void AssetManager::AudioDeleter::operator()(MIX_Audio* audio) const
{
	if (audio)
		MIX_DestroyAudio(audio);
}

void AssetManager::CursorDeleter::operator()(SDL_Cursor* cursor) const
{
	if (cursor)
		SDL_DestroyCursor(cursor);
}

void AssetManager::TextureDeleter::operator()(SDL_Texture* texture) const
{
	if (texture)
		SDL_DestroyTexture(texture);
}

} // namespace core
