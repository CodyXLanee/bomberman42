# include "PlayerManager.hpp"

PlayerManager::PlayerManager() : _human_player(nullptr), _spawned_bomb(false){	
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

	em.registerEvent(Event::PLAYER_DIES, MEMBER_CALLBACK(PlayerManager::playerDies));
}

PlayerManager::~PlayerManager(){    SEventManager & em = SEventManager::getInstance();
	em.unRegisterEvent(Event::PLAYER_LEFT, this);
	em.unRegisterEvent(Event::PLAYER_RIGHT, this);
	em.unRegisterEvent(Event::PLAYER_UP, this);
	em.unRegisterEvent(Event::PLAYER_DOWN, this);
	em.unRegisterEvent(Event::END_PLAYER_LEFT, this);
	em.unRegisterEvent(Event::END_PLAYER_RIGHT, this);
	em.unRegisterEvent(Event::END_PLAYER_UP, this);
	em.unRegisterEvent(Event::END_PLAYER_DOWN, this);

    em.unRegisterEvent(Event::HUMAN_PLAYER_LEFT, this);
	em.unRegisterEvent(Event::HUMAN_PLAYER_RIGHT, this);
	em.unRegisterEvent(Event::HUMAN_PLAYER_UP, this);
	em.unRegisterEvent(Event::HUMAN_PLAYER_DOWN, this);
	em.unRegisterEvent(Event::END_HUMAN_PLAYER_LEFT, this);
	em.unRegisterEvent(Event::END_HUMAN_PLAYER_RIGHT, this);
	em.unRegisterEvent(Event::END_HUMAN_PLAYER_UP, this);
	em.unRegisterEvent(Event::END_HUMAN_PLAYER_DOWN, this);

	em.unRegisterEvent(Event::HUMAN_SPAWN_BOMB, this);
	em.unRegisterEvent(Event::END_HUMAN_SPAWN_BOMB, this);

	em.unRegisterEvent(Event::PLAYER_DIES, this);
}


void    PlayerManager::compute(Map const & map, std::vector<IGameEntity *> &entityList){
    for (auto it = _AIs.begin(); it != _AIs.end(); it++){
       (*it)->compute(map, entityList);
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
    if (_human_player && _human_player->getBombCount() < _human_player->getMaxBombNb())
        SEventManager::getInstance().raise(Event::PLAYER_SPAWN_BOMB, _human_player);
    _spawned_bomb = true;
}
void            PlayerManager::humanEndSpawnBomb(void *){
    _spawned_bomb = false;
}

void            PlayerManager::playerDies(void *p){
    if (static_cast<Player *>(p)->getState() != State::DYING){
        std::cout << "Player n = " << static_cast<Player *>(p)->getPlayerNb() << " dies after " << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - static_cast<Player *>(p)->getCreationTime()).count() << " seconds alive." << std::endl;
        static_cast<Player *>(p)->setState(State::DYING);
        if (static_cast<Player *>(p)->getPlayerNb() == 0) {
            SEventManager::getInstance().raise(Event::GAME_OVER, nullptr);
        }
    }
    for (auto &&i : _AIs){
        if (i->getPlayer() == static_cast<Player *>(p)){
            delete i;
            i = nullptr;
        }
    }
    _AIs.erase(std::remove(_AIs.begin(), _AIs.end(), nullptr), _AIs.end());
}




void            PlayerManager::setHumanPlayer(Player *p){
    _human_player = p;
}
void            PlayerManager::addPlayer(Player *p){
    _AIs.push_back(new AI(p));
}