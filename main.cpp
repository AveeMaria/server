#include "include/Comms.hpp"
#include "include/Utils.hpp"

int main() {
	if (SDLNet_Init() < 0) {
		std::cerr << "ERROR: SDLNet_Init failed: " << SDLNet_GetError() << "\n";
		SDLNet_Quit();
	}

	//std::unique_ptr<Match> match(std::make_unique<Match>());

	Comms comms("", (Uint16)12345);

	while (true) {
		if (comms.recieve()) {

		}
		
	}

	std::cout << "Sleeping for 25 seconds...\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(25000));
	
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
