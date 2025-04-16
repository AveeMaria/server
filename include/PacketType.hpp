#pragma once

#include "Utils.hpp"

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

    //game data

    CREATE_TOWER = 35,//id, destrect, type
    CREATE_ENEMY = 40,// id, destrect, type 

    DELETE_ENTITY = 45,//id

    ENEMY_REQUEST = 50,//id, coords
    TOWER_REQUEST = 55,//id, coords

    PLAYER_NAME = 100,//string
    ROLE = 105,//bool false = attacker, true = defender

    ENTITY_POS = 140,
    //input data

    INIT_TIMER = 200,//uni32t time
    MONEY_INIT = 205,//int money

    MONEY_SCORE_UPDATE_A = 210,//int,int,int,int
    MONEY_SCORE_UPDATE_D = 215,//int,int,int,int

    INIT_GAME = 250,//game id Uint8_t

	TERMINATE = 254,//END GAME

    UNDEFINED = 255//smeti?
};

template<typename T>
Uint8 checkType(const T& data)
{
    //vecji typname safety
    using U = typename std::decay<T>::type;

    if (std::is_same<U, PING>::value) {
        return static_cast<Uint8>(PacketType::PING);
    }

    else if (std::is_same<U, PONG>::value) {
        return static_cast<Uint8>(PacketType::PONG);
    }

    else if (std::is_same<U, SYN>::value) {
        return static_cast<Uint8>(PacketType::SYN);
    }
    else if (std::is_same<U, SYN_ACK>::value) {
        return static_cast<Uint8>(PacketType::SYN_ACK);
    }
    else if (std::is_same<U, ACK>::value) {
        return static_cast<Uint8>(PacketType::ACK);
    }
    ////////
	else if (std::is_same<U, CreateTower>::value) {
		return static_cast<Uint8>(PacketType::CREATE_TOWER);
	}
    else if (std::is_same<U, CreateEnemy>::value) {
        return static_cast<Uint8>(PacketType::CREATE_ENEMY);
    }
    else if (std::is_same<U, DeleteEntity>::value) {
        return static_cast<Uint8>(PacketType::DELETE_ENTITY);
    }
    ////
    else if (std::is_same<U, Role>::value) {
        return static_cast<Uint8>(PacketType::ROLE);
    }
    else if (std::is_same<U, InitTimer>::value) {
        return static_cast<Uint8>(PacketType::INIT_TIMER);
    }
    ////
    else if (std::is_same<U, EnemyRequest>::value) {
        return static_cast<Uint8>(PacketType::ENEMY_REQUEST);
    }
    else if (std::is_same<U, TowerRequest>::value) {
        return static_cast<Uint8>(PacketType::TOWER_REQUEST);
    }
    /////
    else if (std::is_same<U, MoneyInit>::value) {
        return static_cast<Uint8>(PacketType::MONEY_INIT);
    }
    else if (std::is_same<U, MoneyScoreUpdateA>::value) {
        return static_cast<Uint8>(PacketType::MONEY_SCORE_UPDATE_A);
    }
    else if (std::is_same<U, MoneyScoreUpdateD>::value) {
        return static_cast<Uint8>(PacketType::MONEY_SCORE_UPDATE_D);
    }

	else if (std::is_same<U, TerminateGame>::value) {
		return static_cast<Uint8>(PacketType::TERMINATE);
	}
    
    else if (std::is_same<U, std::string>::value) {
        return static_cast<Uint8>(PacketType::PLAYER_NAME);
    }
    else if (std::is_same<U, EntityPos>::value) {
        return static_cast<Uint8>(PacketType::ENTITY_POS);
	}

	else if (std::is_same<U, InitGame>::value) {
		return static_cast<Uint8>(PacketType::INIT_GAME);
	}
    
    else {
        return static_cast<Uint8>(PacketType::UNDEFINED);
    }
}