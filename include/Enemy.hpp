#pragma once

#include "SDL.h"
#include "SDL_image.h"

#include "Map.hpp"
#include "Entity.hpp"

//tega je loh punim
enum class EnemyType {
	GOBLIN,//basic ass
	THIEF,//hitr basic ass
	BANDIT,//hitr low helth
	KNIGHT//ma armor
};

class Enemy : public Entity {
private:
	EnemyType type;

	short hp = 100;

	SDL_Rect prevRect = destRect;

	Direction currDir = Direction::EAST;
	Direction prevDir = Direction::UNDEFINED;

	float step = 1;

public:
	void Move(std::unique_ptr<Map>& m);

	Enemy();
	Enemy(Coords &c);

	void Update() override;

	void takeDamage(short dmg);
	bool alive() const;

	//traSH
	void up();
	void down();
	void left();
	void right();

	EnemyType getType() { return type; }

	~Enemy() {}
};

