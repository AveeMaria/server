#include "../include/Map.hpp"

short lvl1[5][9] = {
    { 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 1, 1, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 1, 1, 1, 1, 1, 1, 1 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

Map::Map() {
    /*
    water = TextureManager::LoadTexture("../../../assets/water.png");
    sand = TextureManager::LoadTexture("../../../assets/sand.png");
    grass = TextureManager::LoadTexture("../../../assets/grass.png");
    forest = TextureManager::LoadTexture("../../../assets/forest.png");
    corrupted = TextureManager::LoadTexture("../../../assets/corrupted.png");

    water = std::make_unique<SDL_Texture*>(TextureManager::LoadTexture("../../../assets/water.png"));
    sand = std::make_unique<SDL_Texture*>(TextureManager::LoadTexture("../../../assets/sand.png"));
    grass = std::make_unique<SDL_Texture*>(TextureManager::LoadTexture("../../../assets/grass.png"));
    forest = std::make_unique<SDL_Texture*>(TextureManager::LoadTexture("../../../assets/forest.png"));
    corrupted = std::make_unique<SDL_Texture*>(TextureManager::LoadTexture("../../../assets/corrupted.png"));
*/  
    LoadMap(lvl1);
    
    //za drawmap da ve kam risat kvadratke
    src.x = src.y = 0;
    dest.x = dest.y = 0;
    src.w = 64; dest.w = TILESIZE;
    src.h = 64; dest.h = TILESIZE;
}

Map::~Map() {
    
}

short Map::getMapValue(short r, short c)
{
    return map[r][c];
    return 0;
}

void Map::LoadMap(short arr[MAP_ROWS][MAP_COLS]) {

    for (short row = 0; row < MAP_ROWS; row++) {
        for (short column = 0; column < MAP_COLS; column++) {
            map[row][column] = arr[row][column];
        }
    }
}


void Map::paintValue(short x, short y)
{
    //std::cout << "x: " << x << " y: " << y << "\n";

    x /= TILESIZE;
    y /= TILESIZE;
    
    //std::cout << "col: " << x << " row: " << y << "\n";

    //da negre izven tabele
    if (x < 0 || y < 0) { return; }
    if (x > MAP_COLS - 1 || y > MAP_ROWS - 1) { return; }

    lvl1[y][x] = value;

    LoadMap(lvl1);
}

short Map::getCol(short x)
{
	return x / TILESIZE;
}

short Map::getRow(short y)
{
	return y / TILESIZE;
}
