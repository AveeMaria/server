#include "../include/Tower.hpp"

uint32_t typeShootDelay(const TowerType &t) {
	switch (t) {
	case TowerType::ARCHER:
		return 250;
	case TowerType::MAGE:
		return 500;
	case TowerType::BARRACKS:
		return 750;//ta nima shootanja sploh
	case TowerType::MORTAR:
		return 1000;
	default:
		return 500;
	}
}

//give korrekt projectile type based on tower type
ProjType projTypeFromTower(const TowerType &t) {
	switch (t) {
		case TowerType::ARCHER:
			return ProjType::ARROW;
			break;
		case TowerType::MAGE:
			return ProjType::MAGEBALL;
			break;
		case TowerType::BARRACKS:
			//ERROR barracks nc ne strelja!
			return ProjType::ARROW;
			break;
		case TowerType::MORTAR:
			return ProjType::CANNONBALL;
			break;
		default:
			return ProjType::ARROW;
			break;
	}
}

Tower::Tower() : Entity() {
	srcRect = { 0, 0, TEXTURE_SIZE, TEXTURE_SIZE };
	type = static_cast<TowerType>(rand() % 4);

	if (type == TowerType::BARRACKS) {
		allies.emplace_back(std::make_unique<Ally>(xpos, ypos + TILESIZE / 2));
	}

	Update();
}

Tower::Tower(short r, short c) : Entity(r* TILESIZE, c* TILESIZE) {
	srcRect = { 0, 0, TEXTURE_SIZE, TEXTURE_SIZE };
	type = static_cast<TowerType>(rand() % 4);
	shootdelay = typeShootDelay(type);

	if (type == TowerType::BARRACKS) {
		allies.emplace_back(new Ally(100, 500));
	}

	Update();
}

Tower::Tower(TowerType t, Tile tile) : Entity(tile.row * TILESIZE, tile.col * TILESIZE), type(t)
{
	xpos = tile.col * TILESIZE;
	ypos = tile.row * TILESIZE;

	srcRect = { 0, 0, TEXTURE_SIZE, TEXTURE_SIZE };
	shootdelay = typeShootDelay(type);

	if (type == TowerType::BARRACKS) {
		allies.emplace_back(std::make_unique<Ally>(xpos + TILESIZE / 2, ypos + TILESIZE));
		allies.emplace_back(std::make_unique<Ally>(xpos + TILESIZE / 2, ypos + TILESIZE * 3 / 2));
		allies.emplace_back(std::make_unique<Ally>(xpos, ypos + TILESIZE));
		allies.emplace_back(std::make_unique<Ally>(xpos, ypos + TILESIZE * 3 / 2));
	}

	Update();
}
/*
Tower::Tower(TowerType t, short r, short c)
	: Entity(r* TILESIZE, c* TILESIZE), type(t) {
	srcRect = { 0, 0, TEXTURE_SIZE, TEXTURE_SIZE };
	shootdelay = typeShootDelay(type);

	if (type == TowerType::BARRACKS) {
		allies.emplace_back(std::make_unique<Ally>(xpos + TILESIZE / 2, ypos + TILESIZE));
		allies.emplace_back(std::make_unique<Ally>(xpos + TILESIZE / 2, ypos + TILESIZE * 3 / 2));
		allies.emplace_back(std::make_unique<Ally>(xpos, ypos + TILESIZE));
		allies.emplace_back(std::make_unique<Ally>(xpos, ypos + TILESIZE * 3 / 2));
	}

	Update();
}*/

Tower::Tower(TowerType t, SDL_Rect r)
	: Entity(r.x, r.y), type(t) {
	srcRect = { 0, 0, TEXTURE_SIZE, TEXTURE_SIZE };
	shootdelay = typeShootDelay(type);

	if (type == TowerType::BARRACKS) {
		allies.emplace_back(std::make_unique<Ally>(xpos + TILESIZE / 2, ypos + TILESIZE));
		allies.emplace_back(std::make_unique<Ally>(xpos + TILESIZE / 2, ypos + TILESIZE * 3 / 2));
		allies.emplace_back(std::make_unique<Ally>(xpos, ypos + TILESIZE));
		allies.emplace_back(std::make_unique<Ally>(xpos, ypos + TILESIZE * 3 / 2));
	}

	Update();
}


Tower::~Tower()
{
}

bool Tower::canShoot(uint32_t time)
{
	if (time - lastShootTime >= shootdelay) {
		//kr barracksi nimajo kej strelat
		if (type != TowerType::BARRACKS) {
			lastShootTime = time;
			return true;
		}
	}

	return false;
}

void Tower::makeProjecitle()
{
	projectiles.emplace_back(std::make_unique<Projectile>(Utils::rectMiddle(destRect), projTypeFromTower(type)));
}

void Tower::deleteProjectiles()
{
	projectiles.clear();
}

void Tower::setTargetCoords(Coords tc)
{
	target_coords = tc;
}

bool Tower::updateAllies() {
	if (type == TowerType::BARRACKS) {
		for (auto it = allies.begin(); it != allies.end(); ) {
			std::unique_ptr<Ally>& a = *it;
			a->Update();
			++it;
		}
	}
	return false;
}

// updata ALLY IN PROJEKTILE sam je DEPRECATED
bool Tower::moveProjectiles() {
	for (auto it = projectiles.begin(); it != projectiles.end(); ) {
		std::unique_ptr<Projectile>& p = *it;

		if (p->moveToTarget(target_coords)) {
			it = projectiles.erase(it);
			return true;//zadel prasca
		}
		else {
			++it;
		}
	}
	return false;
}

short Tower::calcDmg(ProjType t)
{
	switch (t) {
	case ProjType::ARROW:
		return 5;
		break;
	case ProjType::MAGEBALL:
		return 10;
		break;
	case ProjType::CANNONBALL:
		return 15;
		break;
	default:
		return 0;
		break;
	}

	return 0;
}

bool Tower::moveProjectiles(std::unique_ptr<Projectile>& proj) {
	for (auto it = projectiles.begin(); it != projectiles.end(); ) {
		std::unique_ptr<Projectile>& p = *it;

		if (p->moveToTarget(target_coords)) {
			proj = std::move(p);
			it = projectiles.erase(it);
			return true;
		}
		else {
			++it;
		}
	}
	return false;
}

void Tower::updateProjectiles() {
	for(auto& p : projectiles) {
		p->Update();
	}
}

void Tower::Update()
{
	destRect.x = xpos;
	destRect.y = ypos;
	destRect.w = TILESIZE;
	destRect.h = TILESIZE;
}

int Tower::getPrice(int _type) {
	switch (static_cast<TowerType>(_type)) {
	case TowerType::ARCHER:
		return 70;
		break;
	case TowerType::MAGE:
		return 70;
		break;
	case TowerType::BARRACKS:
		return 90;
		break;
	case TowerType::MORTAR:
		return 100;
		break;
	}
	return 0;
}
