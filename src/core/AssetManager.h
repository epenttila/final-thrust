#pragma once
#include <unordered_map>
#include <memory>
#include <string>

struct TTF_Font;
struct MIX_Audio;
struct SDL_Cursor;
struct SDL_Texture;

namespace core {

class Renderer;

class AssetManager
{
public:
	AssetManager(Renderer& renderer);
	~AssetManager();

	TTF_Font* loadFont(const std::string& filename, float size);
	MIX_Audio* loadAudio(const std::string& filename);
	SDL_Cursor* loadCursor(const std::string& filename, int hotX, int hotY);
	SDL_Texture* loadTexture(const std::string& filename);

private:
	struct FontDeleter
	{
		void operator()(TTF_Font* font) const;
	};
	struct AudioDeleter
	{
		void operator()(MIX_Audio* audio) const;
	};
	struct CursorDeleter
	{
		void operator()(SDL_Cursor* cursor) const;
	};
	struct TextureDeleter
	{
		void operator()(SDL_Texture* texture) const;
	};

	Renderer& renderer_;
	std::unordered_map<std::string, std::unique_ptr<TTF_Font, FontDeleter>> fonts_;
	std::unordered_map<std::string, std::unique_ptr<MIX_Audio, AudioDeleter>> audio_;
	std::unordered_map<std::string, std::unique_ptr<SDL_Cursor, CursorDeleter>> cursors_;
	std::unordered_map<std::string, std::unique_ptr<SDL_Texture, TextureDeleter>> textures_;
};

} // namespace core
