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
    timer = std::make_unique<Timer>(90);//////////TIMER 180s

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
    
    if (recvPacket) {
        if (recvPacket->len == 0) {
            std::cout << "ERROR: EMPTY PACKET";
            return;
        }
        //printBytes(reinterpret_cast<char*>(recvPacket->data), recvPacket->len);

		std::cout << "recvPacket len: " << recvPacket->len << "\n";

        ///PREVER KER PACKET JE PO PRVEM BYTU
        switch ((Uint8)recvPacket->data[0]) {
        case (int)PacketType::ENEMY_REQUEST:
            std::cout << "type: ENEMY_REQUEST\n";
            
            EnemyRequest er;
            memcpy(&er, &recvPacket->data[2], sizeof(EnemyRequest));
            
			std::cout << "enemy price " << Enemy::getPrice(er.type) << "\n";

			if (Enemy::getPrice(er.type) > attackerMoney) {
                //kent buy enemy
				break;
			}
            else {
				attackerMoney -= Enemy::getPrice(er.type);
				std::cout << "attacker money: " << attackerMoney << "\n";

                enemies.emplace_back(std::make_unique<Enemy>(Utils::getCoordsFromTile(map->getSpawnTile()), er.type));
                comms.stack_send(CreateEnemy{ enemies.back()->getID(), enemies.back()->getRect(), er.type }, gameID, defender);
                comms.stack_send(CreateEnemy{ enemies.back()->getID(), enemies.back()->getRect(), er.type }, gameID, attacker);
            }

            break;
        case (int)PacketType::TOWER_REQUEST:
            std::cout << "type: TOWER_REQUEST\n";

            TowerRequest tr;
            memcpy(&tr, &recvPacket->data[2], sizeof(TowerRequest));

            std::cout << "recieved coords: " << tr.coords.x << " " << tr.coords.y << " type: " << tr.type << "\n";
            towers.emplace_back(std::make_unique<Tower>((TowerType)tr.type, Utils::getTileFromCoords(tr.coords)));

			if (Tower::getPrice(tr.type) > defenderMoney) {
				//kent buy tower
				break;
			}
			else {
                defenderMoney -= Tower::getPrice(tr.type);
				std::cout << "defender money: " << defenderMoney << "\n";

                comms.stack_send(CreateTower{ towers.back()->getID(), towers.back()->getRect(), tr.type }, gameID, attacker);
                comms.stack_send(CreateTower{ towers.back()->getID(), towers.back()->getRect(), tr.type }, gameID, defender);
			}
            break;
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
	if (!isRunning) {
        comms->stack_send(TerminateGame{ true }, gameID, attacker);
        comms->stack_send(TerminateGame{ true }, gameID, defender);
	}
    
    //prvo posle da manjsa delay pol brise
    for (int i : deletedEntityIDs) {
        //std::cout << "atacket host: " << attacker.host << " port: " << attacker.port << "\n";
        comms->stack_send<DeleteEntity>(DeleteEntity{ i }, gameID, attacker);

        //std::cout << "defender host: " << defender.host << " port: " << defender.port << "\n";
        comms->stack_send<DeleteEntity>(DeleteEntity{ i }, gameID, defender);
    }
    deletedEntityIDs.clear();//ko use posle use zbrise iz vektorja

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
        case (int)PacketType::ENEMY_REQUEST:
            std::cout << "type: ENEMY_REQUEST\n";

            EnemyRequest er;
            memcpy(&er, &recvPacket->data[2], sizeof(EnemyRequest));
            enemies.emplace_back(std::make_unique<Enemy>(Utils::getCoordsFromTile(map->getSpawnTile()), er.type));

            comms->stack_send(CreateEnemy{ enemies.back()->getID(), enemies.back()->getRect(), er.type }, gameID, defender);
            comms->stack_send(CreateEnemy{ enemies.back()->getID(), enemies.back()->getRect(), er.type }, gameID, attacker);

            break;
        case (int)PacketType::TOWER_REQUEST:
            std::cout << "type: TOWER_REQUEST\n";

            TowerRequest tr;
            memcpy(&tr, &recvPacket->data[2], sizeof(TowerRequest));
            //towers.emplace_back(std::make_unique<Tower>( Utils::getTileFromCoords(tr.coords) , tr.type));

            std::cout << "recieved coords: " << tr.coords.x << " " << tr.coords.y << " type: " << tr.type << "\n";
            towers.emplace_back(std::make_unique<Tower>((TowerType)tr.type, Utils::getTileFromCoords(tr.coords)));

            comms->stack_send(CreateTower{ towers.back()->getID(), towers.back()->getRect(), tr.type }, gameID, attacker);
            comms->stack_send(CreateTower{ towers.back()->getID(), towers.back()->getRect(), tr.type }, gameID, defender);

            break;
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

    for (auto it = enemies.begin(); it != enemies.end(); ) {
        std::unique_ptr<Enemy>& e = *it;
        
        if (!e->alive()) {
            int id = e->getID();
            deletedEntityIDs.emplace_back(id);
            it = enemies.erase(it);
        }
        else {
            e->Move(map);
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