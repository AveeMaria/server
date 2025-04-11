#pragma once

#include "Utils.hpp"

//struct GenericPacket

//pazi kako to sranje includas!!!!! ful sketchy error je ce narobe
enum class PacketType : Uint8 {
    //networking
    PING = 0,
    PONG = 5,

    SYN = 10,
    SYN_ACK = 15,
    ACK = 20,

    //game data transmission
    GAME_DATA_START = 25,
    GAME_DATA_END = 30,

    //gamedata

    CREATE_TOWER = 35,//id?, destrect, type
    CREATE_ENEMY = 40,//id, destrect, type
    
    
    PLAYER_NAME = 100,//string
    ENTITY_POS = 140,
    //input data
    
    UNDEFINED = 255//smeti?
};

template<typename T>
Uint8 checkType(const T& data)
{
    if (true) {
        if (std::is_same<T, PING>::value) {
            return static_cast<Uint8>(PacketType::PING);
        }

        else if (std::is_same<T, PONG>::value) {
            return static_cast<Uint8>(PacketType::PONG);
        }

        else if (std::is_same<T, SYN>::value) {
            return static_cast<Uint8>(PacketType::SYN);
        }
        else if (std::is_same<T, SYN_ACK>::value) {
            return static_cast<Uint8>(PacketType::SYN_ACK);
        }
        else if (std::is_same<T, ACK>::value) {
            return static_cast<Uint8>(PacketType::ACK);
        }
        ////////
		else if (std::is_same<T, CreateTower>::value) {
			return static_cast<Uint8>(PacketType::CREATE_TOWER);
		}
        else if (std::is_same<T, CreateEnemy>::value) {
            return static_cast<Uint8>(PacketType::CREATE_ENEMY);
        }
        

        else if (std::is_same<T, std::string>::value) {
            return static_cast<Uint8>(PacketType::PLAYER_NAME);
        }
        else if (std::is_same<T, EntityPos>::value) {
            return static_cast<Uint8>(PacketType::ENTITY_POS);
        }
        else {
            return static_cast<Uint8>(PacketType::UNDEFINED);
        }
    }
    else {
        ////////////SAM ZA DEBUG K MA SE SPOROCILA!
        if (std::is_same<T, PING>::value) {
            std::cout << "The type is PING.\n";
            return static_cast<Uint8>(PacketType::PING);
        }

        else if (std::is_same<T, PONG>::value) {
            std::cout << "The type is PONG.\n";
            return static_cast<Uint8>(PacketType::PONG);
        }

        else if (std::is_same<T, SYN>::value) {
            std::cout << "The type is syn.\n";
            return static_cast<Uint8>(PacketType::SYN);
        }
        else if (std::is_same<T, SYN_ACK>::value) {
            std::cout << "The type is syn_ack.\n";
            return static_cast<Uint8>(PacketType::SYN_ACK);
        }
        else if (std::is_same<T, ACK>::value) {
            std::cout << "The type is ack.\n";
            return static_cast<Uint8>(PacketType::ACK);
        }
        ////////
        else if (std::is_same<T, std::string>::value) {
            std::cout << "The type is std::string.\n";
            return static_cast<Uint8>(PacketType::PLAYER_NAME);
        }
        else if (std::is_same<T, EntityPos>::value) {
            std::cout << "The type is std::string.\n";
            return static_cast<Uint8>(PacketType::ENTITY_POS);
        }
        else {
            std::cout << "The type is undefined.\n";
            return static_cast<Uint8>(PacketType::UNDEFINED);
        }
    }
}