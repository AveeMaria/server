#pragma once

#include <iostream>
#include <vector>

#include "config.hpp"
#include "Entity.hpp"
#include "Utils.hpp"
#include "Projectile.hpp"
#include "Ally.hpp"

// : uint8_t
enum class TowerType : uint8_t {
	ARCHER,
	MAGE,
	BARRACKS,
	MORTAR
};

class Tower : public Entity {
private:
	TowerType type;
	uint16_t range = (uint16_t)(TILESIZE * 1.5);

	std::vector<std::unique_ptr<Projectile>> projectiles;
	std::vector<std::unique_ptr<Ally>> allies;

	uint32_t lastShootTime = 0;
	uint32_t shootdelay = 500;

	Coords target_coords = { 0, 0 };
public:
	Tower();
	Tower(short r, short c);
	Tower(TowerType t, Tile tile);
	Tower(TowerType t, SDL_Rect r);

	~Tower();
	
	bool canShoot(uint32_t);

	void makeProjecitle();

	void print() { 
		std::cout << "xpos: " << xpos << " ypos: " << ypos << "\n";
		std::cout << "destRect: " << destRect.x << " " << destRect.y << "\n";
		std::cout << "destRect: " << destRect.w << " " << destRect.h << "\n";
	}
	
	void deleteProjectiles();

	void setTargetCoords(Coords tc);

	bool updateAllies();

	bool moveProjectiles();

	short calcDmg(ProjType t);

	//bool moveProjectiles(Projectile* proj);

	bool moveProjectiles(std::unique_ptr<Projectile>& proj);

	void updateProjectiles();

	TowerType getType() const { return type; }

	uint16_t getRange() const { return range; }

	int projCount() { return (int)projectiles.size(); }

	void Update() override;
};