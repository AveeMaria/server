#include "../include/Projectile.hpp"
/*
Projectile::Projectile()
{
    type = ProjType::ARROW;
    std::cout << "WARNING: Nezeljen nacin konstukcije projektila.\n";
}

Projectile::Projectile(Coords start, ProjType p) {
    type = p;
    Utils::convertCoords(xpos, ypos, start);
}

Projectile::Projectile(bool b) {
	type = ProjType::ARROW;
}
*/

Projectile::Projectile() : Entity() {
    type = ProjType::ARROW;
    std::cout << "WARNING: Unwanted projectile constructor.\n";
}

Projectile::Projectile(Coords start, ProjType p)
    : Entity(start), type(p) {
    Utils::convertCoords(xpos, ypos, start);
}

Projectile::Projectile(bool b) : Entity() {
    type = ProjType::ARROW;
}

Projectile::~Projectile()
{
}

bool Projectile::moveToTarget(const Coords& t) {
    
    //ce je zadel tarco da true da ga deleta iz vektorja
    if ((int)Utils::getDistance(Utils::rectMiddle(destRect), t) <= 16) {
        //std::cout << "projektil zadel tarco!\n";
        return true;
    }

    float dx = static_cast<float>(t.x - xpos);
    float dy = static_cast<float>(t.y - ypos);
    float angle = atan2(dy, dx);

    xpos += static_cast<short>(cos(angle) * speed);
    ypos += static_cast<short>(sin(angle) * speed);

    return false;
}

void Projectile::Update()
{
    srcRect.h = 32;
    srcRect.w = 32; 
    srcRect.x = 0;
    srcRect.y = 0;
    destRect.x = xpos;
    destRect.y = ypos;
    destRect.w = 16;
    destRect.h = 16;
}
