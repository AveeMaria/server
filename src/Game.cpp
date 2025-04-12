#include "../include/Game.hpp"



std::unique_ptr<Timer> timer;

std::unique_ptr<Map> map;

Coords mouse_coords = { 0, 0 };

std::vector<std::unique_ptr<Enemy>> enemies;

std::vector<std::unique_ptr<Tower>> towers;

Game::Game(IPaddress _attacker, IPaddress _defender, Comms& comms)
{
    attacker = _attacker;
    defender = _defender;

    isRunning = true;

    map = std::make_unique<Map>();

    towers.emplace_back(std::make_unique<Tower>(TowerType::MAGE, Tile{ 3, 3 }));
    towers.emplace_back(std::make_unique<Tower>(TowerType::ARCHER, Tile{ 1, 3 }));
    towers.emplace_back(std::make_unique<Tower>(TowerType::BARRACKS, Tile{ 1, 4 }));
    towers.emplace_back(std::make_unique<Tower>(TowerType::MORTAR, Tile{ 3, 5 }));

    enemies.emplace_back(std::make_unique<Enemy>(Utils::getTileMiddle(Tile{ 1, 0 })));
    enemies.emplace_back(std::make_unique<Enemy>(Utils::getTileMiddle(Tile{ 2, 2 })));
    enemies.emplace_back(std::make_unique<Enemy>(Utils::getTileMiddle(Tile{ 1, 1 })));

    //std::cout << "ent_cnt: " << Entity::ent_cnt << "\n";

    comms.stack_send(Role{ false }, attacker);
    comms.stack_send(Role{ true }, defender);
    
    for (auto& t : towers) {
        comms.stack_send(CreateTower{ t->getID(), t->getRect(), (int)t->getType() }, attacker);
        comms.stack_send(CreateTower{ t->getID(), t->getRect(), (int)t->getType() }, defender);
    }

    for (auto& e : enemies) {
        comms.stack_send(CreateEnemy{ e->getID(), e->getRect(), (int)e->getType() }, defender);
        comms.stack_send(CreateEnemy{ e->getID(), e->getRect(), (int)e->getType() }, attacker);
    }

    timer = std::make_unique<Timer>(90);
    comms.stack_send(InitTimer{ timer->getSeconds() }, attacker);
    comms.stack_send(InitTimer{ timer->getSeconds() }, attacker);
}


void Game::networking(Comms& comms, UDPpacket* recvPacket) {
    //prvo posle da manjsa delay pol brise
    for (int i : deletedEntityIDs) {
        //std::cout << "atacket host: " << attacker.host << " port: " << attacker.port << "\n";
        comms.stack_send<DeleteEntity>(DeleteEntity{ i }, attacker);

        //std::cout << "defender host: " << defender.host << " port: " << defender.port << "\n";
        comms.stack_send<DeleteEntity>(DeleteEntity{ i }, defender);
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
			memcpy(&er, &recvPacket->data[1], sizeof(EnemyRequest));
			enemies.emplace_back(std::make_unique<Enemy>(er.coords, er.type));
            
            comms.stack_send(CreateEnemy{ enemies.back()->getID(), enemies.back()->getRect(), er.type }, defender);
            comms.stack_send(CreateEnemy{ enemies.back()->getID(), enemies.back()->getRect(), er.type }, attacker);

            break;
        case (int)PacketType::TOWER_REQUEST:
            std::cout << "type: TOWER_REQUEST\n";

            TowerRequest tr;
            memcpy(&tr, &recvPacket->data[1], sizeof(TowerRequest));
            //towers.emplace_back(std::make_unique<Tower>( Utils::getTileFromCoords(tr.coords) , tr.type));

            towers.emplace_back(std::make_unique<Tower>((TowerType)tr.type, Utils::getTileFromCoords(tr.coords)));

            comms.stack_send(CreateTower{ towers.back()->getID(), towers.back()->getRect(), tr.type }, attacker);
            comms.stack_send(CreateTower{ towers.back()->getID(), towers.back()->getRect(), tr.type }, defender);

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
    //prvo posle da manjsa delay pol brise
    for (int i : deletedEntityIDs) {
        //std::cout << "atacket host: " << attacker.host << " port: " << attacker.port << "\n";
        comms->stack_send<DeleteEntity>(DeleteEntity{ i }, attacker);

        //std::cout << "defender host: " << defender.host << " port: " << defender.port << "\n";
        comms->stack_send<DeleteEntity>(DeleteEntity{ i }, defender);
    }
    deletedEntityIDs.clear();//ko use posle use zbrise iz vektorja

    UDPpacket* recvPacket = SDLNet_AllocPacket(512);
    if (comms->recieve(&recvPacket)) {
        if (recvPacket->len == 0) {
            std::cout << "ERROR: EMPTY PACKET";
            //continue;
            return;
        }
        //printBytes(reinterpret_cast<char*>(recvPacket->data), recvPacket->len);

        ///PREVER KER PACKET JE PO PRVEM BYTU
        switch ((Uint8)recvPacket->data[0]) {
        case (int)PacketType::ENEMY_REQUEST:
            std::cout << "type: ENEMY_REQUEST\n";

            EnemyRequest er;
            memcpy(&er, &recvPacket->data[1], sizeof(EnemyRequest));
            enemies.emplace_back(std::make_unique<Enemy>(er.coords, er.type));

            comms->stack_send(CreateEnemy{ enemies.back()->getID(), enemies.back()->getRect(), er.type }, defender);
            comms->stack_send(CreateEnemy{ enemies.back()->getID(), enemies.back()->getRect(), er.type }, attacker);

            break;
        case (int)PacketType::TOWER_REQUEST:
            std::cout << "type: TOWER_REQUEST\n";

            TowerRequest tr;
            memcpy(&tr, &recvPacket->data[1], sizeof(TowerRequest));
            //towers.emplace_back(std::make_unique<Tower>( Utils::getTileFromCoords(tr.coords) , tr.type));

            towers.emplace_back(std::make_unique<Tower>((TowerType)tr.type, Utils::getTileFromCoords(tr.coords)));

            comms->stack_send(CreateTower{ towers.back()->getID(), towers.back()->getRect(), er.type }, attacker);
            comms->stack_send(CreateTower{ towers.back()->getID(), towers.back()->getRect(), er.type }, defender);

            break;
        default:
            std::cout << "Unknown packet type.\n";
            break;
        }
    }
}

void Game::update() {
    if (timer->done()) {
        //std::cout << "timer finished\n";
    }
    else {
        timer->updateTimer();
    }

    for (auto& t : towers) {
        t->updateAllies();
        
        for (auto it = enemies.begin(); it != enemies.end();) {
            std::unique_ptr<Enemy>& e = *it;
            e->Update();
            ++it;

            std::unique_ptr<Projectile> p = std::make_unique<Projectile>(false);
            if (t->moveProjectiles(p)) {
                //uzame dmg glede na tower level in na projectile type
				e->takeDamage(t->calcDmg(p->getType()));
            }

            //k je enemy v dosegu
            if (Utils::getDistance(t->getCoords(), e->getCoords()) <= t->getRange()) {

                t->setTargetCoords(Utils::rectMiddle(e->getRect()));
                //ce je konc cooldowna
                if (t->canShoot(SDL_GetTicks())) {
                    t->makeProjecitle();
                }

                SDL_RenderDrawLine(Renderer::renderer, Utils::rectMiddle(t->getRect()).x, Utils::rectMiddle(t->getRect()).y, Utils::rectMiddle(e->getRect()).x, Utils::rectMiddle(e->getRect()).y);

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
            std::cout << "delete enemy id: " << id << " deleteVector size: " << deletedEntityIDs.size() << "\n";
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