#pragma once

#include "renderer.hpp"

#include <iostream>
#include <time.h>
#include <vector>
#include <string>
#include <cstring>
#include <cmath>
#include <fstream>
#include <thread>
#include <queue>
#include <mutex>
#include <memory>
#include <chrono>
///////////DIY///////////
#include "TextureManager.hpp"
#include "Map.hpp"
#include "Utils.hpp"
#include "Entity.hpp"
#include "Enemy.hpp"
#include "Tower.hpp"
#include "Timer.hpp"
#include "Comms.hpp"

class Game
{
public:
	Game();
	Game(IPaddress attacker, IPaddress defender, Comms &comms);

	~Game();

	void networking(Comms* comms);

	bool isPaused() const { return paused; }

	void recive_inputs(Comms* comms);//recive inputs from clients
	void update();//caulate game mechanics
	void send_data(Comms* comms);//send data to clients

	void clean();
	bool running() const { return isRunning; }
private:
	IPaddress attacker = { 0,0 };
	IPaddress defender = { 0,0 };

	bool isRunning = false;
	bool paused = false;
	bool mouse_down = false;
	bool map_editor_mode = false;

	Uint32 lastMoveTime = 0;
	Uint32 lastclick = 0;

	uint32_t cnt = 0;
};