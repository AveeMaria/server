#include "../include/Game.hpp"

std::unique_ptr<Timer> timer;

std::unique_ptr<Map> map;

Coords mouse_coords = { 0, 0 };

std::vector<std::unique_ptr<Enemy>> enemies;

std::vector<std::unique_ptr<Tower>> towers;

Game::Game(IPaddress _attacker, IPaddress _defender, Comms& comms)
{
	gameCnt++;
	gameID = gameCnt;

    attacker = _attacker;
    defender = _defender;

    isRunning = true;

	comms.stack_send(InitGame{ gameID }, defender);
    comms.stack_send(InitGame{ gameID }, attacker);

    comms.stack_send(Role{ false }, gameID, attacker);
    comms.stack_send(Role{ true }, gameID, defender);

    map = std::make_unique<Map>(); 
    timer = std::make_unique<Timer>(120);//////////TIMER 180s

	attackerMoney = 300;
    defenderMoney = 300;

    comms.stack_send(MoneyInit{ attackerMoney }, gameID, attacker);
    comms.stack_send(MoneyInit{ defenderMoney }, gameID, defender);

    comms.stack_send(InitTimer{ timer->getSeconds() * 1000 }, gameID, attacker);
    comms.stack_send(InitTimer{ timer->getSeconds() * 1000 }, gameID, defender);
}

void Game::networking(Comms& comms, UDPpacket* recvPacket) {
    //prvo posle da manjsa delay pol brise
    for (int i : deletedEntityIDs) {
        //std::cout << "atacket host: " << attacker.host << " port: " << attacker.port << "\n";
        comms.stack_send<DeleteEntity>(DeleteEntity{ i }, gameID, attacker);

        //std::cout << "defender host: " << defender.host << " port: " << defender.port << "\n";
        comms.stack_send<DeleteEntity>(DeleteEntity{ i }, gameID, defender);
    }
	deletedEntityIDs.clear();//ko use posle use zbrise iz vektorja
    
    for (auto& ms : msUpdateA) {
        comms.stack_send(msUpdateA.back(), gameID, attacker);
        comms.stack_send( msUpdateA.back(), gameID, defender);
    }
    msUpdateA.clear();
    for (auto& ms : msUpdateD) {
        comms.stack_send(msUpdateD.back(), gameID, defender);
        comms.stack_send(msUpdateD.back(), gameID, attacker);
    }
    msUpdateD.clear();

    //send ping evry 10 seconds
    if (Utils::getTimeMs() - lastSentPing >= max_ping_time) {
        comms.stack_send(PING{ 0 }, gameID, defender);
        comms.stack_send(PING{ 0 }, gameID, attacker);
        lastSentPing = Utils::getTimeMs();
    }

    if (lastDefenderPing != 0 && lastAttackerPing != 0) {
        auto currTime = Utils::getTimeMs();
        //po 10 sekundah disconectej clienta
        if (currTime - lastDefenderPing >= 2 * max_ping_time) {
            std::cout << "defender disconected\n";
            attackerScore += 100000;
            isRunning = false;
        }
        if (currTime - lastAttackerPing >= 2 * max_ping_time) {
            std::cout << "attacker disconected\n";
            defenderScore += 100000;
            isRunning = false;
        }
    }

    if (!isRunning) {
        if (defenderScore < attackerScore) {
            comms.stack_send(TerminateGame{ true }, gameID, attacker);
            comms.stack_send(TerminateGame{ true }, gameID, defender);
        }
        else {
            comms.stack_send(TerminateGame{ false }, gameID, attacker);
            comms.stack_send(TerminateGame{ false }, gameID, defender);
        }
    }

    if (recvPacket) {
        if (recvPacket->len == 0) {
            std::cout << "ERROR: EMPTY PACKET";
            return;
        }
        //printBytes(reinterpret_cast<char*>(recvPacket->data), recvPacket->len);
		//std::cout << "recvPacket len: " << recvPacket->len << "\n";

        ///PREVER KER PACKET JE PO PRVEM BYTU
        switch ((Uint8)recvPacket->data[0]) {
        case (int)PacketType::PONG:
            if (recvPacket->address.host == attacker.host) {
                lastAttackerPing = Utils::getTimeMs();
            }
            else if (recvPacket->address.host == defender.host) {
                lastDefenderPing = Utils::getTimeMs();
            }
            else {
                std::cout << "UNRECOGNISED PONG\n";
            }

            break;
        case (int)PacketType::ENEMY_REQUEST:
            std::cout << "type: ENEMY_REQUEST\n";
            
            EnemyRequest er;
            memcpy(&er, &recvPacket->data[2], sizeof(EnemyRequest));
            
			//std::cout << "enemy price " << Enemy::getPrice(er.type) << "\n";

			if (Enemy::getPrice(er.type) > attackerMoney) {
                //kent buy enemy
				break;
			}
            else {
				attackerMoney -= Enemy::getPrice(er.type);
				//std::cout << "attacker money: " << attackerMoney << "\n";

                enemies.emplace_back(std::make_unique<Enemy>(Utils::getCoordsFromTile(map->getSpawnTile()), er.type));
                comms.stack_send(CreateEnemy{ enemies.back()->getID(), enemies.back()->getRect(), er.type }, gameID, defender);
                comms.stack_send(CreateEnemy{ enemies.back()->getID(), enemies.back()->getRect(), er.type }, gameID, attacker);
            }

            break;

        case (int)PacketType::TOWER_REQUEST:
        {
            TowerRequest tr;
            memcpy(&tr, &recvPacket->data[2], sizeof(tr));

            Tile tile = Utils::getTileFromCoords(tr.coords);
            SDL_Rect pr = { tile.col * TILESIZE, tile.row * TILESIZE, TILESIZE, TILESIZE };

            bool allowTower = true;
            for (auto& r : towerRects) {
                if (Utils::areRectsEqual(r, pr)) {
                    allowTower = false;
                    break;
                }
            }
            if (!allowTower) {
                break;
            }

            int price = Tower::getPrice(tr.type);
            if (price > defenderMoney) {
                break;
            }

            towers.emplace_back(std::make_unique<Tower>((TowerType)tr.type, tile));
            towerRects.emplace_back(pr);
            defenderMoney -= price;

            comms.stack_send(CreateTower{ towers.back()->getID(), pr, tr.type }, gameID, attacker);
            comms.stack_send(CreateTower{ towers.back()->getID(), pr, tr.type }, gameID, defender);
            break;
        }
        default:
            std::cout << "Unknown packet type.\n";
            break;
        }
    }
    else {
		std::cout << "ERROR: recvPacket is null\n";
    }
}

void Game::networking(Comms* comms) {
    //send ping evry 10 seconds
    if (Utils::getTimeMs() - lastSentPing >= max_ping_time) {
        comms->stack_send(PING{ 0 }, gameID, defender);
        comms->stack_send(PING{ 0 }, gameID, attacker);
    }

    if (lastDefenderPing != 0 && lastAttackerPing != 0) {
        auto currTime = Utils::getTimeMs();
        //po 10 sekundah disconectej clienta
        if (currTime - lastDefenderPing >= 2 * max_ping_time) {
            std::cout << "defender disconected\n";
            attackerScore += 100000;
            isRunning = false;
        }
        if (currTime - lastAttackerPing >= 2 * max_ping_time) {
            std::cout << "attacker disconected\n";
            defenderScore += 100000;
            isRunning = false;
        }
    }

	if (!isRunning) {
        if (defenderScore < attackerScore) {
            comms->stack_send(TerminateGame{ true }, gameID, attacker);
            comms->stack_send(TerminateGame{ true }, gameID, defender);
        }
        else {
            comms->stack_send(TerminateGame{ false }, gameID, attacker);
            comms->stack_send(TerminateGame{ false }, gameID, defender);
        }
	}

    //prvo posle da manjsa delay pol brise
    for (int i : deletedEntityIDs) {
        //std::cout << "atacket host: " << attacker.host << " port: " << attacker.port << "\n";
        comms->stack_send<DeleteEntity>(DeleteEntity{ i }, gameID, attacker);

        //std::cout << "defender host: " << defender.host << " port: " << defender.port << "\n";
        comms->stack_send<DeleteEntity>(DeleteEntity{ i }, gameID, defender);
    }
    deletedEntityIDs.clear();//ko use posle use zbrise iz vektorja

    for (auto& ms : msUpdateA) {
        //std::cout << "attacker money: " << ms.money << " score: " << ms.score << "\n";

        comms->stack_send(MoneyScoreUpdateA { ms.money, ms.score }, gameID, attacker);
        comms->stack_send(MoneyScoreUpdateA { ms.money, ms.score }, gameID, defender);
    }
    msUpdateA.clear();

    for (auto& ms : msUpdateD) {
        //std::cout << "defender money: " << ms.money << " score: " << ms.score << "\n";

        comms->stack_send(MoneyScoreUpdateD{ ms.money, ms.score }, gameID, defender);
        comms->stack_send(MoneyScoreUpdateD{ ms.money, ms.score }, gameID, defender);
    }
    msUpdateD.clear();

    UDPpacket* recvPacket = SDLNet_AllocPacket(512);
    if (comms->recieve(&recvPacket)) {
        if (recvPacket->len == 0) {
            std::cout << "ERROR: EMPTY PACKET";
            return;
        }
        //printBytes(reinterpret_cast<char*>(recvPacket->data), recvPacket->len);

        std::cout << "recvPacket len: " << recvPacket->len << "\n";

        ///PREVER KER PACKET JE PO PRVEM BYTU
        switch ((Uint8)recvPacket->data[0]) {
        case (int)PacketType::PONG:
            if (recvPacket->address.host == attacker.host) {
                lastAttackerPing = Utils::getTimeMs();
            }
            else if (recvPacket->address.host == defender.host) {
                lastDefenderPing = Utils::getTimeMs();
            }
            else {
                std::cout << "UNRECOGNISED PONG\n";
            }

            break;
        case (int)PacketType::ENEMY_REQUEST:
            std::cout << "type: ENEMY_REQUEST\n";

            EnemyRequest er;
            memcpy(&er, &recvPacket->data[2], sizeof(EnemyRequest));
            enemies.emplace_back(std::make_unique<Enemy>(Utils::getCoordsFromTile(map->getSpawnTile()), er.type));

            comms->stack_send(CreateEnemy{ enemies.back()->getID(), enemies.back()->getRect(), er.type }, gameID, defender);
            comms->stack_send(CreateEnemy{ enemies.back()->getID(), enemies.back()->getRect(), er.type }, gameID, attacker);

            break;
        case (int)PacketType::TOWER_REQUEST:
        {
            TowerRequest tr;
            memcpy(&tr, &recvPacket->data[2], sizeof(tr));

            Tile tile = Utils::getTileFromCoords(tr.coords);
            SDL_Rect pr = { tile.col * TILESIZE, tile.row * TILESIZE, TILESIZE, TILESIZE };

            bool allowTower = true;
            for (auto& r : towerRects) {
                if (Utils::areRectsEqual(r, pr)) {
                    allowTower = false;
                    break;
                }
            }
            if (!allowTower) {
                break;
            }

            int price = Tower::getPrice(tr.type);
            if (price > defenderMoney) {
                break;
            }

            towers.emplace_back(std::make_unique<Tower>((TowerType)tr.type, tile));
            towerRects.emplace_back(pr);
            defenderMoney -= price;

            comms->stack_send(CreateTower{ towers.back()->getID(), pr, tr.type }, gameID, attacker);
            comms->stack_send(CreateTower{ towers.back()->getID(), pr, tr.type }, gameID, defender);
            break;
        }
        //unknown
        default:
            std::cout << "Unknown packet type.\n";
            break;
        }
    }
    SDLNet_FreePacket(recvPacket);
}

void Game::update() {
    
    if(timer) {
        timer->updateTimer();
		if (timer->finished) {
			std::cout << "game over\n";
			isRunning = false;
		}
    }

    for (auto& t : towers) {
        t->updateAllies();
		//std::cout << "tower checking\n";

        for (auto it = enemies.begin(); it != enemies.end();) {
            //std::cout << "checking enemy-projectile collisoins\n";

            std::unique_ptr<Enemy>& e = *it;
            e->Update();
            ++it;

            std::unique_ptr<Projectile> p = std::make_unique<Projectile>(false);
            if (t->moveProjectiles(p)) {
                //uzame dmg glede na tower level in na projectile type
				e->takeDamage(t->calcDmg(p->getType()));
                //std::cout << "enemy took damage: " << t->calcDmg(p->getType()) << "\n";
            }

            //k je enemy v dosegu
            if (Utils::getDistance(t->getCoords(), e->getCoords()) <= t->getRange()) {

                t->setTargetCoords(Utils::rectMiddle(e->getRect()));
                //ce je konc cooldowna
                if (t->canShoot(SDL_GetTicks())) {
                    t->makeProjecitle();
                }

                break;//da strelja samo eneega enemyja na enkat
            }
        }
        if (enemies.size() == 0) {
            t->deleteProjectiles();
        }

        t->updateProjectiles();
        t->Update();
    }

	for (auto& e : enemies) {
        if (e->Move(map)) {
			std::cout << "enemy REACHED KINGDOM\n";
            deletedEntityIDs.emplace_back(e->getID());

			attackerMoney += Enemy::getPrice((int)e->getType()) / 2;
            attackerScore += Enemy::getPrice((int)e->getType()) * 2;

            std::cout << "attacker money: " << attackerMoney << " score: " << attackerScore << "\n";

            msUpdateA.emplace_back(MoneyScoreUpdateA{ attackerMoney, attackerScore });
        }
	}

    for (auto it = enemies.begin(); it != enemies.end(); ) {
        std::unique_ptr<Enemy>& e = *it;
        
        if (!e->alive() || (std::find(deletedEntityIDs.begin(), deletedEntityIDs.end(), e->getID()) != deletedEntityIDs.end())) {
            int id = e->getID();
            deletedEntityIDs.emplace_back(id);

            defenderMoney += Enemy::getPrice((int)e->getType()) / 2;
			defenderScore += Enemy::getPrice((int)e->getType()) * 2;

            msUpdateD.emplace_back(MoneyScoreUpdateD{ defenderMoney, defenderScore });

            it = enemies.erase(it);
        }
        else {
            e->Update();
            ++it;
        }
    }
}

void Game::recive_inputs(Comms* comms)
{
}


void Game::send_data(Comms* comms)
{

}

void Game::clean() {
    
    std::cout << "game cleaned\n";
}

Game::Game()
{
}


Game::~Game()
{
}