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

    //game data
    PLAYER_NAME = 30,//string
    ENTITY_POS = 40,

    //input data
    

    
    UNDEFINED = 255//smeti?
};


template<typename T>
Uint8 checkType(const T& data);

template<typename T>
Uint8 checkType(const T& data)
{
    if (true) {
        if (std::is_same<T, Ping>::value) {
            return static_cast<Uint8>(PacketType::PING);
        }

        else if (std::is_same<T, Pong>::value) {
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
    


    ////////////SAM ZA DEBUG K MA SE SPOROCILA!
    if (std::is_same<T, Ping>::value) {
        std::cout << "The type is ping.\n";
        return static_cast<Uint8>(PacketType::PING);
	}
    
    else if (std::is_same<T, Pong>::value) {
        std::cout << "The type is pong.\n";
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