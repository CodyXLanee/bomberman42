# include "PlayerManager.hpp"

PlayerManager::PlayerManager() : _human_player(nullptr), _players(), _spawned_bomb(false){	
    SEventManager & em = SEventManager::getInstance();
	em.registerEvent(Event::PLAYER_LEFT, MEMBER_CALLBACK(PlayerManager::newDirLeft));
	em.registerEvent(Event::PLAYER_RIGHT, MEMBER_CALLBACK(PlayerManager::newDirRight));
	em.registerEvent(Event::PLAYER_UP, MEMBER_CALLBACK(PlayerManager::newDirUp));
	em.registerEvent(Event::PLAYER_DOWN, MEMBER_CALLBACK(PlayerManager::newDirDown));
	em.registerEvent(Event::END_PLAYER_LEFT, MEMBER_CALLBACK(PlayerManager::endNewDirLeft));
	em.registerEvent(Event::END_PLAYER_RIGHT, MEMBER_CALLBACK(PlayerManager::endNewDirRight));
	em.registerEvent(Event::END_PLAYER_UP, MEMBER_CALLBACK(PlayerManager::endNewDirUp));
	em.registerEvent(Event::END_PLAYER_DOWN, MEMBER_CALLBACK(PlayerManager::endNewDirDown));

    em.registerEvent(Event::HUMAN_PLAYER_LEFT, MEMBER_CALLBACK(PlayerManager::humanNewDirLeft));
	em.registerEvent(Event::HUMAN_PLAYER_RIGHT, MEMBER_CALLBACK(PlayerManager::humanNewDirRight));
	em.registerEvent(Event::HUMAN_PLAYER_UP, MEMBER_CALLBACK(PlayerManager::humanNewDirUp));
	em.registerEvent(Event::HUMAN_PLAYER_DOWN, MEMBER_CALLBACK(PlayerManager::humanNewDirDown));
	em.registerEvent(Event::END_HUMAN_PLAYER_LEFT, MEMBER_CALLBACK(PlayerManager::humanEndNewDirLeft));
	em.registerEvent(Event::END_HUMAN_PLAYER_RIGHT, MEMBER_CALLBACK(PlayerManager::humanEndNewDirRight));
	em.registerEvent(Event::END_HUMAN_PLAYER_UP, MEMBER_CALLBACK(PlayerManager::humanEndNewDirUp));
	em.registerEvent(Event::END_HUMAN_PLAYER_DOWN, MEMBER_CALLBACK(PlayerManager::humanEndNewDirDown));

	em.registerEvent(Event::HUMAN_SPAWN_BOMB, MEMBER_CALLBACK(PlayerManager::humanSpawnBomb));
	em.registerEvent(Event::END_HUMAN_SPAWN_BOMB, MEMBER_CALLBACK(PlayerManager::humanEndSpawnBomb));
}
PlayerManager::~PlayerManager(){
}

static bool      can_place_bomb(Player *p){
    return (p->getBombCount() < 1);
}

static glm::vec2 go_to_dir(glm::vec2 pos, Event::Enum dir){
    switch (dir) {
        case Event::PLAYER_LEFT:       pos.x -= 1.0; break;
        case Event::PLAYER_RIGHT:      pos.x += 1.0; break;
        case Event::PLAYER_UP:         pos.y += 1.0; break;
        case Event::PLAYER_DOWN:       pos.y -= 1.0; break;
        default: break;
    }
    return pos;
}

static bool     is_safe(glm::vec2 pos, Map const & map, std::vector<IGameEntity *> &entityList){
    (void)map;
    for (auto it : entityList){
        if (it->getType() == Type::BOMB && (it->getPosition().x == pos.x || it->getPosition().y == pos.y))
            return false;
        if (it->getType() == Type::FLAME && (it->getPosition().x == pos.x && it->getPosition().y == pos.y))
            return false;
    }
    return true;
}

static int      dist_to_safety(Event::Enum dir, glm::vec2 pos, Player *p, Map const & map, std::vector<IGameEntity *> &entityList, int rec){
    std::map<Event::Enum, int> dir_dist;
    
    if (CollisionsManager::hasObstacle(map, pos, entityList, p))
        return -1;
    if (is_safe(glm::round(pos), map, entityList))
        return 1;
    if (rec > 0 && dir != Event::PLAYER_RIGHT)
        dir_dist.insert(std::pair<Event::Enum, int>(Event::PLAYER_LEFT, 1 +dist_to_safety(Event::PLAYER_LEFT, go_to_dir(pos, Event::PLAYER_LEFT), p, map, entityList, --rec)));
    if (rec > 0 && dir != Event::PLAYER_LEFT)
        dir_dist.insert(std::pair<Event::Enum, int>(Event::PLAYER_RIGHT, 1 + dist_to_safety(Event::PLAYER_RIGHT, go_to_dir(pos, Event::PLAYER_RIGHT), p, map, entityList, --rec)));
    if (rec > 0 && dir != Event::PLAYER_DOWN)
        dir_dist.insert(std::pair<Event::Enum, int>(Event::PLAYER_UP, 1 + dist_to_safety(Event::PLAYER_UP, go_to_dir(pos, Event::PLAYER_UP), p, map, entityList, --rec)));
    if (rec > 0 && dir != Event::PLAYER_UP)
        dir_dist.insert(std::pair<Event::Enum, int>(Event::PLAYER_DOWN, 1 + dist_to_safety(Event::PLAYER_DOWN, go_to_dir(pos, Event::PLAYER_DOWN), p, map, entityList, --rec)));
    std::pair<Event::Enum, int> a(Event::PLAYER_MOVE, 10);
    for (auto it : dir_dist){
        if (a.first == Event::PLAYER_MOVE || (a.second > it.second && it.second > 0) || (a.second < 0 && it.second > 0) ){
            a = it;
        }
    }
    return a.second;
}

static void      run_to_objective(Player *p, Map const & map, std::vector<IGameEntity *> &entityList){
    static Event::Enum  lol = Event::PLAYER_DOWN;
    std::map<Event::Enum, int> dir_dist;
    int max_rec = 5;
    if (is_safe(p->getPosition(), map, entityList))
        return;
    dir_dist.insert(std::pair<Event::Enum, int>(Event::PLAYER_LEFT, dist_to_safety(Event::PLAYER_LEFT, go_to_dir(glm::round(p->getPosition()), Event::PLAYER_LEFT), p, map, entityList, max_rec)));
    dir_dist.insert(std::pair<Event::Enum, int>(Event::PLAYER_RIGHT, dist_to_safety(Event::PLAYER_RIGHT, go_to_dir(glm::round(p->getPosition()), Event::PLAYER_RIGHT), p, map, entityList, max_rec)));
    dir_dist.insert(std::pair<Event::Enum, int>(Event::PLAYER_UP, dist_to_safety(Event::PLAYER_UP, go_to_dir(glm::round(p->getPosition()), Event::PLAYER_UP), p, map, entityList, max_rec)));
    dir_dist.insert(std::pair<Event::Enum, int>(Event::PLAYER_DOWN, dist_to_safety(Event::PLAYER_DOWN, go_to_dir(glm::round(p->getPosition()), Event::PLAYER_DOWN), p, map, entityList, max_rec)));
    
    std::pair<Event::Enum, int> a(Event::PLAYER_MOVE, 0);
    for (auto it : dir_dist){
        if (a.first == Event::PLAYER_MOVE || (a.second > it.second && it.second > 0) || (a.second < 0 && it.second > 0) ){
            a = it;
        }
    }
    // std::cout << "=========" << std::endl << p->getPosition().x << " " << p->getPosition().y << std::endl;
    // std::cout << Event::PLAYER_LEFT << " " << Event::PLAYER_RIGHT << " " << Event::PLAYER_UP << " " << Event::PLAYER_DOWN << std::endl;
    // std::cout << a.first << " " << a.second << std::endl;
    if (a.first != Event::PLAYER_MOVE){

        if (a.first != lol){
            switch (lol){
                case Event::PLAYER_LEFT:        SEventManager::getInstance().raise(Event::END_PLAYER_LEFT, p); break;
                case Event::PLAYER_RIGHT:       SEventManager::getInstance().raise(Event::END_PLAYER_RIGHT, p); break;
                case Event::PLAYER_UP:          SEventManager::getInstance().raise(Event::END_PLAYER_UP, p); break;
                case Event::PLAYER_DOWN:        SEventManager::getInstance().raise(Event::END_PLAYER_DOWN, p); break;
                default : break;
            }
        }
        lol = a.first;
        SEventManager::getInstance().raise(a.first, p);
    }
}

void        PlayerManager::ai(Player *p, Map const & map, std::vector<IGameEntity *> &entityList){
    if (can_place_bomb(p))
        SEventManager::getInstance().raise(Event::SPAWN_BOMB, p);
    // get objective position
    // run to it
    run_to_objective(p, map, entityList);
}

void    PlayerManager::compute(Map const & map, std::vector<IGameEntity *> &entityList){
    for (auto it = _players.begin(); it != _players.end(); it++){
        if (*it != _human_player){
            ai(*it, map, entityList);
        }
    }
}


void            PlayerManager::newDirLeft(void *p){
    Player *player = static_cast<Player *>(p);
    player->newDirLeft();
}
void            PlayerManager::newDirRight(void *p){
    Player *player = static_cast<Player *>(p);
    player->newDirRight();
}
void            PlayerManager::newDirUp(void *p){
    Player *player = static_cast<Player *>(p);
    player->newDirUp();
}
void            PlayerManager::newDirDown(void *p){
    Player *player = static_cast<Player *>(p);
    player->newDirDown();
}
void            PlayerManager::endNewDirLeft(void *p){
    Player *player = static_cast<Player *>(p);
    player->endNewDirLeft();
}
void            PlayerManager::endNewDirRight(void *p){
    Player *player = static_cast<Player *>(p);
    player->endNewDirRight();
}
void            PlayerManager::endNewDirUp(void *p){
    Player *player = static_cast<Player *>(p);
    player->endNewDirUp();
}
void            PlayerManager::endNewDirDown(void *p){
    Player *player = static_cast<Player *>(p);
    player->endNewDirDown();
}




void            PlayerManager::humanNewDirLeft(void *){
    if (_human_player)
        SEventManager::getInstance().raise(Event::PLAYER_LEFT, _human_player);
}
void            PlayerManager::humanNewDirRight(void *){
    if (_human_player)
        SEventManager::getInstance().raise(Event::PLAYER_RIGHT, _human_player);
}
void            PlayerManager::humanNewDirUp(void *){
    if (_human_player)
        SEventManager::getInstance().raise(Event::PLAYER_UP, _human_player);
}
void            PlayerManager::humanNewDirDown(void *){
    if (_human_player)
        SEventManager::getInstance().raise(Event::PLAYER_DOWN, _human_player);
}



void            PlayerManager::humanEndNewDirLeft(void *){
    if (_human_player)
        SEventManager::getInstance().raise(Event::END_PLAYER_LEFT, _human_player);
}
void            PlayerManager::humanEndNewDirRight(void *){
    if (_human_player)
        SEventManager::getInstance().raise(Event::END_PLAYER_RIGHT, _human_player);
}
void            PlayerManager::humanEndNewDirUp(void *){
    if (_human_player)
        SEventManager::getInstance().raise(Event::END_PLAYER_UP, _human_player);
}
void            PlayerManager::humanEndNewDirDown(void *){
    if (_human_player)
        SEventManager::getInstance().raise(Event::END_PLAYER_DOWN, _human_player);
}



void            PlayerManager::humanSpawnBomb(void *){
    if (_human_player)
        SEventManager::getInstance().raise(Event::SPAWN_BOMB, _human_player);
    _spawned_bomb = true;
}
void            PlayerManager::humanEndSpawnBomb(void *){
    _spawned_bomb = false;
}



void            PlayerManager::setHumanPlayer(Player *p){
    _human_player = p;
}
void            PlayerManager::addPlayer(Player *p){
    _players.push_back(p);
}