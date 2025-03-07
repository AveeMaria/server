#pragma once 

#include "Utils.hpp"

class Comms {
private:
    IPaddress ip;
    UDPsocket sock;

	const char* host;
	Uint16 port;
public:
	Comms();
	Comms(const char* h, Uint16 p);
	~Comms();

    //to se klice v konstruktorjih
	bool resolveHost();
	bool openSocket();

    //part za sendat
    template <typename T>
    bool send(T);

    template<typename T>
    bool stack_send(T data);
    
    bool stupidestSend(const char* data) const;
	bool stupidSend(Uint8* data, size_t size) const;
	
    bool allocPacket(UDPpacket** packet, const Uint8* data, int size) const;

    //part za recivevat
    bool allocEmptyPacket(UDPpacket** packet, int size) const;
    bool recieve();
    bool recieve(UDPpacket** recvPacket);
};

//////////////////////////
// POMEBNE STVARI KLELE //
//////////////////////////


static void printBytes(char data[], size_t size) {
    for (size_t i = 0; i < size; ++i) {
        std::cout << std::bitset<8>(data[i]) << " ";
    }
    std::cout << "\n";
}

template<typename T>
std::unique_ptr<Uint8[]> prepareData(T data) {
    size_t packetSize = 1 + sizeof(data);
    std::cout << "data size: " << packetSize << "\n";

    std::unique_ptr<Uint8[]> buffer(new Uint8[packetSize]);
    Uint8 type = checkType(data);

    if (type == 255) {
        return nullptr;
    }
    else {
        buffer[0] = type;
    }

    std::memcpy(&buffer[1], &data, sizeof(data));

    printBytes(reinterpret_cast<char*>(buffer.get()), packetSize);

    return buffer;
}

template<typename T>
bool Comms::send(T data) {
    std::unique_ptr<Uint8[]> packet = prepareData(data);

    if (!packet) {
        return false;
    }
    
    stupidSend(packet.get(), sizeof(data) + 1);
    return true;
}

// STACK ALLOCATION VERZIJA? A JE TO SPLOH KEJ HITREJ?
template<typename T>
bool Comms::stack_send(T data) {
    Uint8 type = checkType(data);
    if (type == 255) return false;

    UDPpacket* sendPacket = SDLNet_AllocPacket(sizeof(T) + 1);
    
    sendPacket->address.host = ip.host;
    sendPacket->address.port = ip.port;
    
    sendPacket->data[0] = type;
    std::memcpy(&sendPacket->data[1], &data, sizeof(T));
    printBytes(reinterpret_cast<char*>(sendPacket->data), sizeof(T) + 1);

    if (SDLNet_UDP_Send(sock, -1, sendPacket) < 1) {
        std::cerr << "ERROR: SDLNet_UDP_Send error: " << SDLNet_GetError() << "\n";
        SDLNet_FreePacket(sendPacket);
        return false;
    }
    //std::cout << "OK: poslan paket.\n";
    
    SDLNet_FreePacket(sendPacket);
    return true;
}

//to je sexy af
/*
using ReturnType = std::variant<int, float, std::string, Coords>;
ReturnType myFunction(Uint32 option);
*/


///////////////////////
// recieve del kode? //
///////////////////////


