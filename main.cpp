#include "include/Comms.hpp"
#include "include/Utils.hpp"


std::vector<IPaddress> clients;//connected clients

//std::map<IPaddress, std::string> playerNames;>

int main() {
	std::cout << "SERVERSIDE\n";

	if (SDLNet_Init() < 0) {
		std::cerr << "ERROR: SDLNet_Init failed: " << SDLNet_GetError() << "\n";
		SDLNet_Quit();
	}

	//std::unique_ptr<Match> match(std::make_unique<Match>());

	Comms comms("", (Uint16)12345);

	UDPpacket* recvPacket;

	recvPacket = SDLNet_AllocPacket(512);
	/*
	if (!comms.allocEmptyPacket(&recvPacket, 256)) {
		std::cerr << "ERROR: Failed to allocate memory for the packet." << std::endl;
		return false;
	}*/

	while (true) {
		if (comms.recieve(&recvPacket)) {
			printBytes(reinterpret_cast<char*>(recvPacket->data), recvPacket->len);

			///PREVER KER PACKET JE PO PRVEM BYTU
			
			switch ((Uint8)recvPacket->data[0]) {
			case (int)PacketType::PING:
					//std::cout << "type: PING\n";
					break;
				case (int)PacketType::PONG:
					//std::cout << "type: PONG\n";
					break;
				///////
				case (int)PacketType::SYN:
					std::cout << "type: SYN\n";

					//std::cout << "----------------------------\n";

					if (!comms.stack_send( SYN_ACK{ SDL_GetTicks() }, recvPacket->address)) {
						std::cerr << "ERROR: SYN_ACK not sent.\n";
					}
					else {
						std::cout << "SYN_ACK sent.\n";
					}
					break;
				
				case (int)PacketType::SYN_ACK:
					std::cout << "ERROR: type: SYN_ACK\n";//tega server ne sprejema ker poslje
					break;

				case (int)PacketType::ACK:
					std::cout << "type: ACK\n";

					//dodamo clienta v seznam clientov
					clients.emplace_back(recvPacket->address);
					
					std::cout << "CLIENTS: " << clients.size() << "\n";

					break;
				///////
				case 40:
					//std::cout << "type: ENTITY_POS\n";
					break;
				default:
					std::cout << "Unknown packet type.\n";
					break;
			}
		}
	}

	std::cout << "Sleeping for 25 seconds...\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(25000));

	SDLNet_FreePacket(recvPacket);
	SDLNet_Quit();
	return 0;
}


//int main() {
	//sdl stvari
	//comse setup

	/*MAIN LOOP
	* Recive (player join request/inputs)
	* Send (gamestate updates)
	* Calculate game mechanics
	*/
//}
