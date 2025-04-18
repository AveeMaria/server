#pragma once 

#include "Utils.hpp"
#include "PacketType.hpp"

#include <sstream>
#include <bitset>

class Comms {
private:
    IPaddress ip;
    UDPsocket sock;

	const char* host;
	Uint16 port;
public:
	Comms();
	Comms(const char* h, Uint16 p);
    Comms(Uint16 p);
	~Comms();

    //to se klice v konstruktorjih
	bool resolveHost();
	bool openSocket();

    template<typename T>
    bool stack_send(T data);

    template<typename T>
    bool stack_send(T data, IPaddress _ip);

    template<typename T>
    bool stack_send(T data, Uint8 gameID, IPaddress _ip);
    
	//bool stupidSend(Uint8* data, size_t size) const;
	
    //bool allocPacket(UDPpacket** packet, const Uint8* data, int size) const;

    //part za recivevat
    bool allocEmptyPacket(UDPpacket** packet, int size) const;
    bool recieve();

    bool recieve(UDPpacket** recvPacket);

    static bool ipEquals(const IPaddress& a, const IPaddress& b) {
        Uint32 hostA = SDLNet_Read32(&a.host);
        Uint32 hostB = SDLNet_Read32(&b.host);
        return (hostA == hostB) && (a.port == b.port);
    }

    IPaddress Comms::getRemoteIP() const {
        return ip;
    }

    static const char* ipAddressToString(const IPaddress& ip) {
        static char ipStr[64];
        Uint32 ipAddress = ip.host;
        snprintf(ipStr, sizeof(ipStr), "%d.%d.%d.%d", ipAddress & 0xFF, (ipAddress >> 8) & 0xFF, (ipAddress >> 16) & 0xFF, (ipAddress >> 24) & 0xFF);
        return ipStr;
    }
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

static std::stringstream byteToString(char data[], size_t size) {
    std::stringstream ss;
	return ss;
}


template<typename T>
bool Comms::stack_send(T data) {
    Uint8 type = checkType(data);
    if (type == 255) return false;


    UDPpacket* sendPacket = SDLNet_AllocPacket(static_cast<int>(sizeof(T)) + 1);
    if (sendPacket == nullptr) {
        std::cout << "ERROR: No packet\n";
        return false;
    }

    std::cout << "size of T: " << sizeof(T) << "\n";
    std::cout << "Packet len: " << sendPacket->len << "\n";

    //if (sendPacket->len == 0) {
    sendPacket->len = (static_cast<int>(sizeof(T) + 1));
    //}

    sendPacket->address.host = ip.host;
    sendPacket->address.port = ip.port;

    std::cout << "size of T: " << sizeof(T) << "\n";
    std::cout << "Packet len: " << sendPacket->len << "\n";

    sendPacket->data[0] = type;
    std::memcpy(&sendPacket->data[1], &data, sizeof(T));
    //printBytes(reinterpret_cast<char*>(sendPacket->data), sizeof(T) + 1);

    if (sock == nullptr) {
        std::cout << "ERROR: null socket\n";
        return false;
    }

    if (SDLNet_UDP_Send(sock, -1, sendPacket) < 1) {
        std::cerr << "ERROR: SDLNet_UDP_Send error: " << byteToString((char*)SDLNet_GetError(), strlen(SDLNet_GetError())).str() << "\n";
        std::cerr << "ERROR: SDLNet_UDP_Send error: " << std::string(SDLNet_GetError()) << "\n";

        //printBytes((char*)SDLNet_GetError(), 200);
        SDLNet_FreePacket(sendPacket);
        return false;
    }
    //std::cout << "OK: poslan paket.\n";

    SDLNet_FreePacket(sendPacket);
    return true;
}

template<typename T>
bool Comms::stack_send(T data, IPaddress _ip) {
    Uint8 type = checkType(data);
    if (type == 255) return false;

    UDPpacket* sendPacket = SDLNet_AllocPacket(sizeof(T) + 1);

    if (sendPacket == nullptr) {
        std::cout << "ERROR: No packet\n";
        return false;
    }
    if (!sendPacket) {
        std::cout << "ERROR: No packet\n";
        return false;
    }

    sendPacket->address.host = _ip.host;
    sendPacket->address.port = _ip.port;

    std::cout << "Send packet size: " << sizeof(T) + 1 << "\n";
    sendPacket->len = (sizeof(T) + 1);

    
    //printBytes(reinterpret_cast<char*>(sendPacket->data), sizeof(T) + 1);

    sendPacket->data[0] = type;
    std::memcpy(&sendPacket->data[1], &data, sizeof(T));

    if (SDLNet_UDP_Send(sock, -1, sendPacket) < 1) {
        std::cerr << "ERROR: SDLNet_UDP_Send error: " << SDLNet_GetError() << "\n";
        SDLNet_FreePacket(sendPacket);
        return false;
    }
    //std::cout << "OK: poslan paket.\n";

    SDLNet_FreePacket(sendPacket);
    return true;
}

template<typename T>
bool Comms::stack_send(T data, Uint8 gameID, IPaddress _ip) {
    Uint8 type = checkType(data);
    if (type == 255) return false;
	if (gameID == 0) return false;//default game id

    UDPpacket* sendPacket = SDLNet_AllocPacket(sizeof(T) + 2);

    if (sendPacket == nullptr) {
        std::cout << "ERROR: No packet\n";
        return false;
    }
    if (!sendPacket) {
        std::cout << "ERROR: No packet\n";
        return false;
    }

    sendPacket->address.host = _ip.host;
    sendPacket->address.port = _ip.port;

    //std::cout << "Send packet size: " << sizeof(T) + 2 << "\n";
    sendPacket->len = (sizeof(T) + 2);

    //printBytes(reinterpret_cast<char*>(sendPacket->data), sizeof(T) + 2);

    sendPacket->data[0] = type;
    sendPacket->data[1] = gameID;
    std::memcpy(&sendPacket->data[2], &data, sizeof(T));

    if (SDLNet_UDP_Send(sock, -1, sendPacket) < 1) {
        std::cerr << "ERROR: SDLNet_UDP_Send error: " << SDLNet_GetError() << "\n";
        SDLNet_FreePacket(sendPacket);
        return false;
    }
    //std::cout << "OK: poslan paket.\n";

    SDLNet_FreePacket(sendPacket);
    return true;
}