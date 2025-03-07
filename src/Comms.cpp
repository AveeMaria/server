#include "../include/Comms.hpp"

Comms::Comms()
{
    host = "127.0.0.1";
    port = 12345;
    resolveHost();
    openSocket();
	//allocPacket(&recvPacket, nullptr, 512);
}

Comms::Comms(const char* h, Uint16 p)
{
    host = h;
    port = p;
    resolveHost();
    openSocket();
    //allocEmptyPacket(&recvPacket, 256);
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

bool Comms::stupidestSend(const char* data) const {
    UDPpacket* sendPacket;
    if (!allocPacket(&sendPacket, reinterpret_cast<const Uint8*>(data), (int)std::strlen(data) + 1)) {
        std::cerr << "ERROR: cant allocate packet.\n";
        return false;
    }

    sendPacket->address.host = ip.host;
    sendPacket->address.port = ip.port;

    if (SDLNet_UDP_Send(sock, -1, sendPacket) < 1) {
        std::cerr << "ERROR: SDLNet_UDP_Send error: " << SDLNet_GetError() << "\n";
        return false;
    }

    std::cout << "OK: poslan paket.\n";
    return true;
}

bool Comms::stupidSend(Uint8* data, size_t size) const {
    UDPpacket* sendPacket;
    if (!allocPacket(&sendPacket, data, (int)size)) {
        std::cerr << "ERROR: cant allocate packet.\n";
        return false;
    }

    sendPacket->address.host = ip.host;
    sendPacket->address.port = ip.port;

    if (SDLNet_UDP_Send(sock, -1, sendPacket) < 1) {
        std::cerr << "ERROR: SDLNet_UDP_Send error: " << SDLNet_GetError() << "\n";
        SDLNet_FreePacket(sendPacket);
        return false;
    }
    SDLNet_FreePacket(sendPacket);
    std::cout << "OK: poslan paket.\n";
    return true;
}

bool Comms::allocPacket(UDPpacket** packet, const Uint8* data, int size) const {
    *packet = SDLNet_AllocPacket(size + 512);

    if (!*packet) {
        std::cerr << "ERROR: SDLNet_AllocPacket: " << SDLNet_GetError() << "\n";
        return false;
    }

    std::memcpy((*packet)->data, data, size);//nepotrebno?
    (*packet)->len = size;

    std::cout << "OK: Packet allocated.\n";
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

/*
using ReturnType = std::variant<int, float, std::string, Coords>;
ReturnType myFunction(Uint32 option) {
    if (option == 1) {
        return 42;
    }
    else if (option == 2) {
        return 3.14f;
    }
    else if (option == 3) {
        return std::string("Hello, World!");
    }
    else {
        return Coords{ 1, 2 };
    }
}*/

/////////////////
//  recive del //
/////////////////

/*
bool Comms::recieve()
{
    int packetsReceived = 0;

    while (true) {
        UDPpacket* recvPacket;
        if (!allocEmptyPacket(&recvPacket, 256)) {
            std::cerr << "ERROR: Failed to allocate memory for the packet." << std::endl;
            return false;
        }

        if (SDLNet_UDP_Recv(sock, recvPacket) <= 0) {
            SDLNet_FreePacket(recvPacket);
            break;
        }

        packetsReceived++;

        printBytes(reinterpret_cast<char*>(recvPacket->data), (size_t) recvPacket->len);

        std::cout << "Received packet from: " << SDLNet_ResolveIP(&recvPacket->address) << "\n";

        std::cout << reinterpret_cast<char*>(recvPacket->data) << "\n";

        SDLNet_FreePacket(recvPacket);
        break;
    }

    return packetsReceived > 0;
}
*/

bool Comms::recieve()
{
    UDPpacket* recvPacket;
    if (!allocEmptyPacket(&recvPacket, 256)) {
        std::cerr << "ERROR: Failed to allocate memory for the packet." << std::endl;
        return false;
    }

    if (SDLNet_UDP_Recv(sock, recvPacket) <= 0) {
        SDLNet_FreePacket(recvPacket);
        return false;
    }

    size_t size = recvPacket->len;
    printBytes(reinterpret_cast<char*>(recvPacket->data), size);

    std::cout << "Received packet from: " << SDLNet_ResolveIP(&recvPacket->address) << "\n";

    std::cout << reinterpret_cast<char*>(recvPacket->data) << "\n";

    SDLNet_FreePacket(recvPacket);

    return true;
}



bool Comms::recieve(UDPpacket** recvPacket)
{
    if (!allocEmptyPacket(&(*recvPacket), 256)) {
        std::cerr << "ERROR: Failed to allocate memory for the packet." << std::endl;
        return false;
    }

    if (SDLNet_UDP_Recv(sock, *recvPacket) <= 0) {
        SDLNet_FreePacket(*recvPacket);
        return false;
    }

//    size_t size = recvPacket->len;
    size_t size = (*recvPacket)->len;
    printBytes(reinterpret_cast<char*>((*recvPacket)->data), size);

    std::cout << "Received packet from: " << SDLNet_ResolveIP(&(*recvPacket)->address) << "\n";

    std::cout << reinterpret_cast<char*>((*recvPacket)->data) << "\n";

    SDLNet_FreePacket((*recvPacket));

    return true;
}