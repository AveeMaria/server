#include "include/Comms.hpp"
#include "include/Utils.hpp"
#include <vector>
#include <thread>
#include "include/Game.hpp"
#include "include/LoggerSQL.hpp"
//#include <cstdlib>

std::vector<IPaddress> clients;//connected clients

std::vector<Game> games;//matches that happen now

//std::map<IPaddress, std::string> playerNames;>

int Entity::ent_cnt = 100;
uint8_t Game::gameCnt = 0;

const int frameDelay = 1000 / FPS;

int main() {
	//start MySQL
	//LoggerSQL::startMySQL();

	std::cout << "SERVERSIDE\n";

	if (SDLNet_Init() < 0) {
		std::cerr << "ERROR: SDLNet_Init failed: " << SDLNet_GetError() << "\n";
		SDLNet_Quit();
	}
	
	//std::unique_ptr<Match> match(std::make_unique<Match>());

	Comms comms(12345);

	UDPpacket* recvPacket;
	recvPacket = SDLNet_AllocPacket(512);

	//games.push_back(Game());

	while (true) {
		while (comms.recieve(&recvPacket)) {
			std::cout << "paketek.\n";
			///PREVER KER PACKET JE PO PRVEM BYTU
			switch ((int)recvPacket->data[0]) {
			case (int)PacketType::PING:
				//std::cout << "type: PING\n";
				break;
			case (int)PacketType::PONG:
				//std::cout << "type: PONG\n";
				break;
			case (int)PacketType::SYN:
				//std::cout << "type: SYN\n";

				if (!comms.stack_send(SYN_ACK{ SDL_GetTicks() }, recvPacket->address)) {
					std::cerr << "ERROR: SYN_ACK not sent.\n";
				}
				else {
					//std::cout << "SYN_ACK sent.\n";
				}
				break;

			case (int)PacketType::SYN_ACK:
				std::cout << "ERROR: type: SYN_ACK\n";//tega server ne sprejema ker poslje
				break;

			case (int)PacketType::ACK:
				//std::cout << "type: ACK\n";
				// 
				//doda clienta v seznam clientov, ne duplicated
				{
					auto it = std::find_if(clients.begin(), clients.end(), [&](const IPaddress& addr) {
						return addr.host == recvPacket->address.host && addr.port == recvPacket->address.port;
						});
					if (it == clients.end()) {
						clients.emplace_back(recvPacket->address);
					}
					else {
						std::cout << "client " << Comms::ipAddressToString(recvPacket->address) << " already connected\n";
					}
				}

 				std::cout << "client " << Comms::ipAddressToString(recvPacket->address) << " on port: " << recvPacket->address.port << " connected\n";

				if (clients.size() == 2) {
					games.emplace_back(Game(clients[0], clients[1], comms));
					//tezave z crashanim MySQLom k se ne zarunna vec u XAMPPu
					//LoggerSQL::logGameSafe(Comms::ipAddressToString(games.back().getAttacker()), Comms::ipAddressToString(games.back().getDefender()));
					std::cout << "Game Started\n";
					clients.clear();
				}

				break;
			default:
				for (auto& g : games) {
					if (g.getGameID() == recvPacket->data[1]) {
						std::cout << "game id: " << g.getGameID() << "\n";
						g.networking(comms, recvPacket);
					}
				}

				break;
			}
		}
		if (clients.size() == 2) {
			games.emplace_back(Game(clients[0], clients[1], comms));
			std::cout << "Game Started\n";
			clients.clear();
			//break;//koncej loop
		}

		auto frameStart = std::chrono::high_resolution_clock::now();

		for (auto it = games.begin(); it != games.end();) {
			if (!it->running()) {
				it = games.erase(it);
				std::cout << "Game ended.\n";
			}
			else {
				it->update();
				it->networking(&comms);

				++it;
			}
		}

		auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - frameStart).count();

		if (frameDuration < frameDelay) {
			std::this_thread::sleep_for(std::chrono::milliseconds(frameDelay - frameDuration));
		}
	}

	std::cout << "Shutting down in 25 seconds...\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(25000));

	SDLNet_FreePacket(recvPacket);
	SDLNet_Quit();
	return 0;
}