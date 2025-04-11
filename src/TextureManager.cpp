#include "../include/TextureManager.hpp"

SDL_Texture* TextureManager::LoadTexture(const char* texture) 
{
	SDL_Surface* tempSurface = IMG_Load(texture);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Renderer::renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	if (tempSurface == nullptr) {
		std::cerr << "ERROR: Failed to load surface: " << texture << " SDL_image: " << IMG_GetError() << "\n";
	}

	if (tex == nullptr) {
		std::cerr << "ERROR: Failed to load texture: " << texture << " SDL_image: " << IMG_GetError() << "\n";
	}

	return tex;
}

void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest)
{
	SDL_RenderCopy(Renderer::renderer, tex, &src, &dest);
}

SDL_Surface* TextureManager::LoadSurface(const char* filename)
{
	SDL_Surface* surface = IMG_Load(filename);
	if (surface != nullptr) {
		return surface;
	}
	
	std::cerr << "ERROR: Failed to load surface: " << filename << " SDL_image: " << IMG_GetError() << "\n";
	return nullptr;
}
