#include "../include/Enemy.hpp"

void Enemy::Move(std::unique_ptr<Map>& m) {
    short row = m->getRow(ypos);
    short col = m->getCol(xpos);

    bool moved = false;
    for (int i = 0; i < 4; ++i) {
        switch (currDir) {
        case Direction::NORTH:
            if (m->getMapValue(row - 1, col) == 1) {
                up();
                moved = true;
            }
            break;
        case Direction::SOUTH:
            if (m->getMapValue(row + 1, col) == 1) {
                down();
                moved = true;
            }
            break;
        case Direction::EAST:
            if (m->getMapValue(row, col + 1) == 1) {
                right();
                moved = true;
            }
            break;
        case Direction::WEST:
            if (m->getMapValue(row, col - 1) == 1) {
                left();
                moved = true;
            }
            break;
        default:
            char c = 'a';
        }

        if (moved) {
            break;
        }
        else {
            currDir = static_cast<Direction>(std::rand() % 4);
        }
    }

    if (!moved) {
        //probi najdt direction k ni previous direction
        for (short i = 0; i < 4; ++i) {
            if (currDir != prevDir) {
                switch (currDir) {
                case Direction::NORTH:
                    if (m->getMapValue(row - 1, col) == 1) {
                        up();
                        moved = true;
                    }
                    break;
                case Direction::SOUTH:
                    if (m->getMapValue(row + 1, col) == 1) {
                        down();
                        moved = true;
                    }
                    break;
                case Direction::EAST:
                    if (m->getMapValue(row, col + 1) == 1) {
                        right();
                        moved = true;
                    }
                    break;
                case Direction::WEST:
                    if (m->getMapValue(row, col - 1) == 1) {
                        left();
                        moved = true;
                    }
                    break;
                default:
                    char c = 'a';//da default ne joka
                    //nemore se premikat, nej crkne al neki?
                }

                if (moved) {
                    break;
                }
            }
            currDir = static_cast<Direction>(std::rand() % 4);
        }
    }

    if (!moved) {
        currDir = Direction::UNDEFINED;
    }
}

Enemy::Enemy() : Entity(0, 0) {
    type = static_cast<EnemyType>(std::rand() % 4);
    Update();
}

Enemy::Enemy(Coords& c) : Entity(c) {
    type = static_cast<EnemyType>(std::rand() % 4);
    Update();
}

Enemy::Enemy(Coords& c, int& _type) : Entity(c) {
    type = static_cast<EnemyType>(_type);
    Update();
}

void Enemy::Update() 
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

//returna true ce je umru da ga deleta
void Enemy::takeDamage(short dmg)
{
    if (hp >= 0) {
        hp -= dmg;
    }
}

bool Enemy::alive() const {
	return hp > 0;
}

void Enemy::up()
{
    if (ypos - step < 0) {
        return;
    }
    ypos -= (short)step;
}
void Enemy::down()
{
    if (ypos + step > SCREEN_HEIGHT) {
        return;
    }
    ypos += (short)step;
}
void Enemy::left()
{
    if (xpos - step < 0) {
        return;
    }
    xpos -= (short)step;
}
void Enemy::right()
{
    if (xpos + step > SCREEN_WIDTH - destRect.w) {
        return;
    }
    xpos += (short)step;
}
