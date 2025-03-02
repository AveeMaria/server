#include "include/Comms.hpp"
#include "include/Utils.hpp"


struct entity {
	int id;
	Coords c;
	void move(){
		if (c.x >= 500) {
			c.x = 0; c.y = 0;
		}
		c.x++;
		c.y++;
	}
	void print() const {
		std::cout << "ID: " << id << ", x: " << c.x << " y: " << c.y << "\n";
	}
};

int main() {
	if (SDLNet_Init() < 0) {
		std::cerr << "ERROR: SDLNet_Init failed: " << SDLNet_GetError() << "\n";
		SDL_Quit();
	}

	
	Comms comms("", (Uint16)12345);
	//Comms comms("", (Uint16)12345);
	
	entity e{ 15, Coords{0,0} };


	int x = 19;
	float y = 5.6f;
	Coords c = { 100, 50 };
	std::string s;

	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	while (true) {
		if (comms.recieve()) {
			std::cout << "OK: recieved\n";
			break;
		}
	}

	while (true) {
		e.move();
		e.print();

		if (comms.send(e)) {
			std::cout << "OK: sent\n";
			//break;//sam 1x
		}
		else {
			//std::cout << "OK: not recieved\n";
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	/*
	if (comms.send(c)) {
		std::cout << "OK: sent\n";
	}
	else {
		std::cout << "ERROR: not sent\n";
	}

	if (comms.recieve()) {
		std::cout << "OK: recieved\n";
	}
	else {
		std::cout << "OK: not recieved\n";
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));


	if (comms.send(y)) {
		std::cout << "OK: sent\n";
	}
	else {
		std::cout << "ERROR: not sent\n";
	}


	if (comms.recieve()) {
		std::cout << "OK: recieved\n";
	}
	else {
		std::cout << "OK: not recieved\n";
	}
	*/


	std::this_thread::sleep_for(std::chrono::milliseconds(25000));
	return 0;
}