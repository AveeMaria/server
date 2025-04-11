#include "../include/Game.hpp"



std::unique_ptr<Timer> timer;

std::unique_ptr<Map> map;

Coords mouse_coords = { 0, 0 };

std::vector<std::unique_ptr<Enemy>> enemies;

std::vector<std::unique_ptr<Tower>> towers;

Game::Game(IPaddress attacker, IPaddress defender, Comms& comms)
{
    isRunning = true;

    map = std::make_unique<Map>();
    timer = std::make_unique<Timer>(90);

    towers.emplace_back(std::make_unique<Tower>(TowerType::ARCHER, Tile{ 3, 1 }));
    /*
    towers.emplace_back(std::make_unique<Tower>(TowerType::ARCHER, 1, 2));
    towers.emplace_back(std::make_unique<Tower>(TowerType::MAGE, 5, 1));
    towers.emplace_back(std::make_unique<Tower>(TowerType::BARRACKS, 3, 1));
    towers.emplace_back(std::make_unique<Tower>(TowerType::MORTAR, 3, 3));
    */

    enemies.emplace_back(std::make_unique<Enemy>(Utils::getTileMiddle(Tile{ 1, 0 })));
    //enemies.emplace_back(std::make_unique<Enemy>(Utils::getTileMiddle(Tile{ 4, 2 })));
    //enemies.emplace_back(std::make_unique<Enemy>(Utils::getTileMiddle(Tile{ 3, 1 })));

    //std::cout << "ent_cnt: " << Entity::ent_cnt << "\n";

    for (auto& t : towers) {
        comms.stack_send(CreateTower{ t->getID(), t->getRect(), (int)t->getType() }, attacker);
        comms.stack_send(CreateTower{ t->getID(), t->getRect(), (int)t->getType() }, defender);
    }

    std::cout << "ENBEMY COUNT:T:T: " << enemies.size() << "\n";

    for (auto& e : enemies) {
        std::cout << "ENEMY STUFF: " << e->getID() << " " << e->getRect().x << " " << e->getRect().y << " " << (int)e->getType() << "\n";
        comms.stack_send(CreateEnemy{ e->getID(), e->getRect(), (int)e->getType() }, defender);
        comms.stack_send(CreateEnemy{ e->getID(), e->getRect(), (int)e->getType() }, attacker);
    }
}

void Game::networking(Comms* comms) {
    //comms->stack_send(, attacker);

    UDPpacket* recvPacket;
    if (comms->recieve(&recvPacket)) {
        if (recvPacket->len == 0) {
            std::cout << "ERROR: EMPTY PACKET";
            //continue;
            return;
        }
        printBytes(reinterpret_cast<char*>(recvPacket->data), recvPacket->len);

        ///PREVER KER PACKET JE PO PRVEM BYTU

        switch ((Uint8)recvPacket->data[0]) {
        case (int)PacketType::PING:
            std::cout << "type: PING\n";
            break;
        case (int)PacketType::PONG:
            std::cout << "type: PONG\n";
            break;
            ///////
        case (int)PacketType::SYN:
            std::cout << "ERROR: type: SYN\n";//TEGA CLIENT NE SPREJEMA KER POSLJE
            break;
        case (int)PacketType::SYN_ACK:
            std::cout << "type: SYN_ACK\n";

            if (!comms->stack_send(ACK{ SDL_GetTicks() }, recvPacket->address)) {
                std::cerr << "ERROR: ACK not sent.\n";
            }
            break;
        case (int)PacketType::ACK:
            std::cout << "ERROR: type: ACK\n";//TEGA CLIENT NE SPREJEMA KER POSLJE
            break;
        
        default:
            std::cout << "Unknown packet type.\n";
            break;
        }
    }
}

void Game::recive_inputs(Comms* comms)
{
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
        e->Move(map);
        e->Update();

        if (!e->alive()){
            it = enemies.erase(it); break;
        }
        else {
			++it;
        }
    }
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