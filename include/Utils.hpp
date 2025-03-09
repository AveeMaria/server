#pragma once

///////////////////SDL
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_net.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

///////////////////STD
#include <iostream>
#include <thread>
#include <chrono>
#include <memory>
#include <bitset>
#include <type_traits>
#include <variant>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <map>

struct Coords {
	int x;
	int y;
};

struct EntityPos {
	int id;
	Coords coords;
};

struct Ping {
	Uint32 time;
};

struct Pong {
	Uint32 time;
};


struct SYN {
	Uint32 time;
};

struct SYN_ACK {
	Uint32 time;
};

struct ACK {
	Uint32 time;
};