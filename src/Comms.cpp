#include "../include/Comms.hpp"
#include <fstream>
#include <filesystem>

Comms::Comms()
{
    host = "127.0.0.1";
    port = 12345;
    resolveHost();
    openSocket();
	std::cout << "----------------\n";
}

//KONSTURKTOR SAMO ZA CLIENTSIDE
Comms::Comms(const char* h, Uint16 p)
{
    host = h;
    port = p;
    resolveHost();
    openSocket();
    std::cout << "----------------\n";
}

//KONSTRUKTOR SAMO ZA SERVERSIDE
Comms::Comms(Uint16 p) {
    port = p;

    std::ifstream file("../../../port.txt");
    std::cout << "[DEBUG]: Attempting to open file ../../../port.txt\n";
    if (!file.is_open()) {
        std::cerr << "[ERROR]: Cannot open port.txt\n";
    }
    
    else {
        std::cout << "[DEBUG]: port.txt opened successfully\n";
        std::string line;
        
        if (std::getline(file, line)) {
            port = std::stoi(line);
            std::cout << "[INFO]: port from port.txt: " << line << "\n";
        }
        else {
            std::cout << "[DEBUG]: Current working directory: " << std::filesystem::current_path() << "\n";
            std::cerr << "[ERROR]: Failed to read from port.txt\n";
        }

        file.close();
    }

    if (!openSocket()) { 
        std::cout << "ERROR CANT OPEN SOCKET\n"; 
    }
    std::cout << "Server socket opened on port " << port << "\n";
    std::cout << "----------------\n";
}

Comms::~Comms()
{
    if (sock) {
        SDLNet_UDP_Close(sock);
    }
}

bool Comms::resolveHost() {
    if (SDLNet_ResolveHost(&ip, host, 12345) == -1) {
        std::cerr << "ERROR: SDLNet_ResolveHost: " << SDLNet_GetError() << "\n";
        return false;
    }

    std::cout << "OK: Host " << host << " resolved.\n";
    return true;
}

bool Comms::openSocket() {
    sock = SDLNet_UDP_Open(port);//0 al pa 12345
    if (!sock) {
        std::cerr << "ERROR: SDLNet_UDP_Open: " << SDLNet_GetError() << "\n";
        return false;
    }

    if (sock == nullptr) {
        std::cerr << "ERROR: Socket null.\n";
        return false;
    }

    std::cout << "OK: Socket opened on " << port << ".\n";
    return true;
}

bool Comms::allocEmptyPacket(UDPpacket** packet, int size) const {
    *packet = SDLNet_AllocPacket(size + 256);//udp overhead?

    if (!*packet) {
        std::cerr << "ERROR: SDLNet_AllocPacket: " << SDLNet_GetError() << "\n";
        return false;
    }

    //std::cout << "OK: Packet allocated.\n";
    return true;
}

/////////////////
//  recive del //
/////////////////

bool Comms::recieve()
{
    UDPpacket* recvPacket;
    if (!allocEmptyPacket(&recvPacket, 256)) {
        std::cerr << "ERROR: Failed to allocate memory for the packet." << std::endl;
        return false;
    }

    if (SDLNet_UDP_Recv(sock, recvPacket) <= 0) {
        //SDLNet_FreePacket(recvPacket);
        return false;
    }

    //printBytes(reinterpret_cast<char*>((*recvPacket)->data), (*recvPacket)->len);

    return true;
}

bool Comms::recieve(UDPpacket** recvPacket)
{
    if (!recvPacket) {
        std::cout << "WARNING: null recvPacket";
        return false;
    }
    if (SDLNet_UDP_Recv(sock, *recvPacket) <= 0) {
        return false;
    }

    //printBytes(reinterpret_cast<char*>((*recvPacket)->data), (*recvPacket)->len);

    return true;
}

/*
bool Comms::allocPacket(UDPpacket** packet, const Uint8* data, int size) const {
    *packet = SDLNet_AllocPacket(size + 512);

    if (!*packet) {
        std::cerr << "ERROR: SDLNet_AllocPacket: " << SDLNet_GetError() << "\n";
        return false;
    }

    std::memcpy((*packet)->data, data, size);//nepotrebno?
    (*packet)->len = size;

    //dont dilet sam debug
    //std::cout << "OK: Packet allocated.\n";
    return true;
}*/
