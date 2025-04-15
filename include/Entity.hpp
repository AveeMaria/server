#pragma once

#include "renderer.hpp"
#include "Utils.hpp"
#include "TextureManager.hpp"
#include "config.hpp"

class Entity {
protected:
	int id;

	short xpos = 0;
	short ypos = 0;

	SDL_Texture* objTexture = nullptr;

	SDL_Rect srcRect = {0, 0, TILESIZE, TILESIZE };
	SDL_Rect destRect = {0, 0, TILESIZE, TILESIZE };
public:
	static int ent_cnt;

	Entity(const char* texturesheet, short x, short y);

	Entity() {
		++ent_cnt;
		id = ent_cnt;
		//Update();
	}

	//x,y,tex,srcR,destR
	Entity(short x, short y) : xpos(x), ypos(y), objTexture(nullptr), srcRect{ 0, 0, TILESIZE, TILESIZE }, destRect{ 0, 0, TILESIZE, TILESIZE } {
		++ent_cnt;
		id = ent_cnt;
		//std::cout << "WARNING: Entity constructor called.\n";
		//Update();
	}

	Entity(Coords c);
	Entity(Tile t);

	virtual ~Entity() { --ent_cnt; }

	static void printEntCnt() { std::cout << "entity count: " << ent_cnt << "\n"; }

	virtual void Update();

	void setPos(const Coords &c) { this->xpos = c.x; this->ypos = c.y; }
	void setPos(const int &x, const int &y) { this->xpos = x; this->ypos = y; }

	short getX() const { return xpos; }
	short getY() const { return ypos; }

	int getID() const { return id; }

	Coords getCoords() const;
	SDL_Rect getRect() const;
	

    void setTilePos(short row, short col) { this->xpos = row * TILESIZE; this->ypos = col * TILESIZE; }
};