#include "../include/Ally.hpp"

Ally::Ally() : Entity() {
    std::cout << "ERROR: don't use default constructor\n";
}

Ally::Ally(const short x, const short y) : Entity(x, y) {
    srcRect = { 0, 0, TEXTURE_SIZE, TEXTURE_SIZE };
}

Ally::Ally(const Coords c) : Entity(c) {
    srcRect = { 0, 0, TEXTURE_SIZE, TEXTURE_SIZE };
}

Ally::~Ally() {

}

void Ally::Update()
{
    srcRect.h = TEXTURE_SIZE;
    srcRect.w = TEXTURE_SIZE;
    srcRect.x = 0;
    srcRect.y = 0;

    destRect.x = xpos;
    destRect.y = ypos;
    destRect.w = TILESIZE / 2;
    destRect.h = TILESIZE / 2;
}
