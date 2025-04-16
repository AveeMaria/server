#pragma once

#include <iostream>

#include "config.hpp"
#include "renderer.hpp"
#include "TextureManager.hpp"

/////////////////

struct Color {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
};

struct Tile {
	short row;
	short col;
};

struct Coords {
	int x;
	int y;
};


struct EntityPos {
	int id;
	Coords coords;
};


enum class ProjType {
	ARROW,
	CANNONBALL,
	MAGEBALL
};

enum class Direction : short {
	UNDEFINED,
	NORTH,
	SOUTH,
	EAST,
	WEST
};

struct PING {
	Uint32 time;
};

struct PONG {
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

struct CreateTower {
	int id;
	SDL_Rect destRect;
	int type;
};

struct CreateEnemy {
	int id;
	SDL_Rect destRect;
	int type;
};

struct DeleteEntity {
	int id;
};

struct InitTimer {
	Uint32 time;
};

struct Role {
	bool defender;//true = defender, false = attacker
};

struct InitGame {
	Uint8 gameID;
};

struct EnemyRequest {
	int type;
};

struct TowerRequest {
	int type;
	Coords coords;
};

struct MoneyInit {
	int money;
};

struct TerminateGame {
	bool attackerWon = false;
};

struct MoneyScoreUpdateA {
	int money;
	int score;
};

struct MoneyScoreUpdateD {
	int money;
	int score;
};

class Utils {
public:
	static void print(const char* msg) {
		std::cout << msg << "\n";
	}

	static Tile getTileFromCoords(const Coords& c) {
		return Tile{ (short)(c.y / TILESIZE), (short)(c.x / TILESIZE) };
	}

	static Coords getCoordsFromTile(const Tile& t) {
		return Coords{ t.col * TILESIZE, t.row * TILESIZE };
	}

	static void drawCircle(int& centerX, int& centerY, uint16_t& radius) {
		//90 stopinj * 4 kvadranti = 360 stopinj
		for (int i = 0; i < 90; ++i) {
			double angle = i * M_PI / 180;

			int x = (int)(radius * cos(angle));
			int y = (int)(radius * sin(angle));

			SDL_RenderDrawPoint(Renderer::renderer, centerX + x, centerY + y);//1.kvadrant (x, y)
			SDL_RenderDrawPoint(Renderer::renderer, centerX - x, centerY + y);//2.kvadrant (-x, y)
			SDL_RenderDrawPoint(Renderer::renderer, centerX + x, centerY - y);//3.kvadrant (x, -y)
			SDL_RenderDrawPoint(Renderer::renderer, centerX - x, centerY - y);//4.kvadrant (-x, -y)
		}
	}

	static void drawColoredCircle(int centerX, int centerY, uint16_t radius, const Color& color) {
		SDL_SetRenderDrawColor(Renderer::renderer, color.red, color.green, color.blue, 255);

		drawCircle(centerX, centerY, radius);

		SDL_SetRenderDrawColor(Renderer::renderer, 255, 255, 255, 255);
	}

	static Coords getCoords(const Tile& t) {
		return Coords{ t.col * TILESIZE, t.row * TILESIZE };
	}

	static Coords getCoords(const short& x, const short& y) {
		return Coords{ x, y };
	}

	static Coords getCoords(const int& x, const int& y) {
		return Coords{ x, y };
	}

	static void convertCoords(int& x, int& y, const Coords& c) {
		x = c.x;
		y = c.y;
	}

	static void convertCoords(short& x, short& y, const Coords& c) {
		x = c.x;
		y = c.y;
	}

	static Coords rectMiddle(const SDL_Rect& r) {
		return Coords{ r.x + r.w / 2, r.y + r.h / 2 };
	}

	static Coords getTileMiddle(const Tile& t) {
		return Coords{ t.col * TILESIZE + TILESIZE / 2, t.row * TILESIZE + TILESIZE / 2 };
	}

	static uint16_t getDistance(Coords c1, Coords c2) {
		return static_cast<uint16_t>(sqrt(pow(c2.x - c1.x, 2) + pow(c2.y - c1.y, 2)));
	}

	//prever ce je koordinata v rectanglu
	static bool coordInRect(const Coords& c, const SDL_Rect& rect) {
		return (c.x >= rect.x && c.x <= rect.x + rect.w && c.y >= rect.y && c.y <= rect.y + rect.h);
	}

	static void drawLoadingScreen() {
		SDL_Rect srcRect{ 0, 0, 1920, 1080 };
		SDL_Rect destRect{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

		SDL_RenderClear(Renderer::renderer);
		SDL_RenderCopy(Renderer::renderer, TextureManager::LoadTexture("../../../assets/loading.png"), &srcRect, &destRect);
		SDL_RenderPresent(Renderer::renderer);
	}

	static void drawStartupScreen() {
		SDL_Rect srcRect{ 0, 0, 1920, 1080 };
		SDL_Rect destRect{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

		SDL_RenderClear(Renderer::renderer);
		SDL_RenderCopy(Renderer::renderer, TextureManager::LoadTexture("../../../assets/start_menu.png"), &srcRect, &destRect);
		SDL_RenderPresent(Renderer::renderer);
	}

	static uint32_t getTimeMs() {
		return static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
	}
};

/*IDEJE ZA KASNEJE
class Circle : Entity {
public:
	Circle() : Entity() {}
	Circle(short x, short y) : Entity(x, y) {}
	void Update() override {

	}
	void Render() override {
	}
	static void drawCircle(SDL_Renderer::renderer* Renderer::renderer, int& centerX, int& centerY, uint16_t& radius) {}
};*/

