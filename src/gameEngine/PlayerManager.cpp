# include "PlayerManager.hpp"

PlayerManager::PlayerManager() : _human_player(nullptr), _players(){	
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
}
PlayerManager::~PlayerManager(){
}

void    PlayerManager::compute(Map const & map, std::vector<IGameEntity *> &entityList){
    (void)map;
    (void)entityList;
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


void            PlayerManager::setHumanPlayer(Player *p){
    _human_player = p;
}
void            PlayerManager::addPlayer(Player *p){
    _players.push_back(p);
}