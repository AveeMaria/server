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

	void networking(Comms& comms, UDPpacket* recvPacket);

	~Game();

	void networking(Comms* comms);

	bool isPaused() const { return paused; }

	void recive_inputs(Comms* comms);//recive inputs from clients
	void update();//caulate game mechanics
	void send_data(Comms* comms);//send data to clients

	void clean();
	bool running() const { return isRunning; }

	IPaddress getAttacker() const { return attacker; }
	IPaddress getDefender() const { return defender; }

	static uint8_t gameCnt;
	uint8_t getGameID() { return gameID; }
private:

	uint8_t gameID = 0;

	IPaddress attacker = { 0,0 };
	IPaddress defender = { 0,0 };

	//last time server sendt ping to cleint
	Uint32 lastAttackerPing = 0;
	Uint32 lastDefenderPing = 0;
	Uint32 lastSentPing = 0;

	std::vector<int> deletedEntityIDs;
	std::vector<MoneyScoreUpdateA> msUpdateA;
	std::vector<MoneyScoreUpdateD> msUpdateD;

	std::vector<SDL_Rect> towerRects;//dodaja rectangle towerjev da jih ne postavla na isto mesto

	bool isRunning = false;
	bool paused = false;
	bool mouse_down = false;
	bool map_editor_mode = false;

	int attackerMoney = 0;
	int defenderMoney = 0;

	int attackerScore = 0, defenderScore = 0;

	Uint32 lastMoveTime = 0;
	Uint32 lastclick = 0;

	uint32_t cnt = 0;
};