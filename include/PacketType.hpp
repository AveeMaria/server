#pragma once

#include "Utils.hpp"

//struct GenericPacket

//pazi kako to sranje includas!!!!! ful sketchy error je ce narobe
enum class PacketType : Uint8 {
  
    //networking
    PING = 0,

    SYN = 10,
    SYN_ACK = 15,
    ACK = 20,

    //game data
    PLAYER_NAME = 30,
    ENTITY_POS = 40,

    //input data
    



    UNDEFINED = 255//smeti?
};



template<typename T>
Uint8 checkType(const T& data) {
    if (std::is_same<T, int>::value) {
        std::cout << "The type is int.\n";
        return static_cast<Uint8>(PacketType::INT);
    }
    else if (std::is_same<T, float>::value) {
        std::cout << "The type is float.\n";
        return static_cast<Uint8>(PacketType::FLOAT);
    }
    else if (std::is_same<T, Coords>::value) {
        std::cout << "The type is Coords.\n";
        return static_cast<Uint8>(PacketType::COORDS);
    }
    else if (std::is_same<T, std::string>::value) {
        std::cout << "The type is std::string.\n";
        return static_cast<Uint8>(PacketType::STRING);
    }
    else {
        std::cout << "The type is undefined.\n";
        return static_cast<Uint8>(PacketType::UNDEFINED);
    }
}
