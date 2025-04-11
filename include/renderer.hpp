#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_Net.h>

/*
class Renderer
{
public:
	static SDL_Renderer* renderer;
	static SDL_Window* window;
};
*/

namespace Renderer
{
	extern SDL_Renderer* renderer;
	extern SDL_Window* window;
}