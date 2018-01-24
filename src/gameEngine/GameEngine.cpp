/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameEngine.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 16:14:09 by tpierron          #+#    #+#             */
/*   Updated: 2018/01/24 11:00:40 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Player.hpp"
#include "Enemy.hpp"
#include "GameEngine.hpp"
#include "glm/ext.hpp"
#include "IndestructibleBloc.hpp"
#include "DestructibleBloc.hpp"

GameEngine::GameEngine(GameParams &gp) : _map(new Map()), 
_entityList(new std::vector<IGameEntity *>()), 
_bonusManager(new BonusManager(_entityList, gp)),
_enemyManager(new EnemyManager(_entityList, _map)),
_bombManager(new BombManager(_map, _entityList)), 
_playerManager(new PlayerManager()),
_gameParams(gp),
_winManager(nullptr),
_win(false), _active(true) {
    SEventManager &em = SEventManager::getInstance();
    em.registerEvent(Event::GAME_WIN, MEMBER_CALLBACK(GameEngine::gameWin));
    em.registerEvent(Event::GAME_OVER, MEMBER_CALLBACK(GameEngine::gameOver));
    em.registerEvent(Event::GAME_PAUSE, MEMBER_CALLBACK(GameEngine::pause));
    em.registerEvent(Event::GAME_UNPAUSE, MEMBER_CALLBACK(GameEngine::unPause));
	
	if (_gameParams.get_game_mode() == GameMode::BRAWL){
		loadMap("maps/brawl_0.json");
		placeBrawlPlayers(_gameParams.get_color());
	}
	else if (_gameParams.get_game_mode() == GameMode::CAMPAIGN)
	{
		loadMap(std::string("maps/campaign/" + std::to_string(_gameParams.get_level() + 1) + ".json").c_str());
	}

}

GameEngine::~GameEngine() {
    SEventManager::getInstance().unRegisterEvent(Event::GAME_WIN, this);
    SEventManager::getInstance().unRegisterEvent(Event::GAME_OVER, this);
    SEventManager::getInstance().unRegisterEvent(Event::GAME_PAUSE, this);
    SEventManager::getInstance().unRegisterEvent(Event::GAME_UNPAUSE, this);
	delete _playerManager;
	delete _bombManager;
	delete _bonusManager;
	delete _enemyManager;
	delete _winManager;
    for (auto &&i : *_entityList){
		delete i;
	}
	delete _entityList;
}

void	GameEngine::compute() {
	// auto start = std::chrono::system_clock::now();

    if (this->_loader.getState() == -1 || !_winManager || !_active)
        return ;

    _playerManager->compute(*_map, *_entityList);
	_collisionsManager.moves(*_map, *_entityList);
	_bombManager->update();
	_bonusManager->update();
	_enemyManager->update();
	_winManager->update(*_map, *_entityList);

	// auto end = std::chrono::system_clock::now();
	// auto elapsed = end - start;
	// std::cout << "Game Engine compute time : " << elapsed.count() << std::endl;

}

Map const &		GameEngine::getMap() const {
	return *(this->_map);
}

std::vector<IGameEntity * > &	GameEngine::getEntityList() {
	return *(this->_entityList);
}


glm::vec2 const 				* GameEngine::getPlayerPos(void) const{
	for (auto i = _entityList->begin(); i != _entityList->end(); i++){
		if ((*i)->getType() == Type::PLAYER){
			return new glm::vec2((*i)->getPosition());
		}
	}
	return new glm::vec2(0., 0.);
}

glm::vec2				GameEngine::placeBrawlAI(Player *human, int i){
	glm::vec2	v(human->getPosition());
	if (i == 0 || i == 2)
		v.y = human->getPosition().y == 0.f ? _map->getSize().y - 1.f : 0.f;
	if (i == 0 || i == 1 )
		v.x = human->getPosition().x == 0.f ? _map->getSize().x - 1.f : 0.f;
	return v;
}

void					GameEngine::placeBrawlPlayers(PlayerColor::Enum _color){
	// Set the Human Player at a random corner
	Player *human = new Player(glm::vec2((rand() % 2) * (_map->getSize().x - 1), (rand() % 2) * (_map->getSize().y - 1)), 0, _color);
	this->_entityList->push_back(human);
	_playerManager->setHumanPlayer(human);
	for (int i = 0; i < _gameParams.get_brawl_enemy_nb(); i++){
		_color = _color == PlayerColor::YELLOW ? PlayerColor::WHITE : static_cast<PlayerColor::Enum>(_color + 1);
		Player *p = new Player(placeBrawlAI(human, i), i + 1, _color);
		_playerManager->addPlayer(p);
		this->_entityList->push_back(p);
	}
}

void					GameEngine::loadMap(const char *path){
    rapidjson::Value * grid;
    rapidjson::Value * sun;

	Player *	player;
	Enemy *		enemy;
	Bonus *		bonus;
	glm::vec2	vec;

	this->_loader.setPath(path);
	if (this->_loader.load() != 1)
	{
		return ;
	}
	grid = this->_loader.getValue("grid");
	if (grid && !grid[0].IsArray())
    	return ;

	// MAP
	if (_gameParams.get_game_mode() == GameMode::BRAWL)
	{
	    for (unsigned int i = 0 ; i < grid[0].Size() ; i++)
	    {
	    	if (!grid[0][i].IsArray())
	    		return ;
	    	for (unsigned int j = 0 ; j < grid[0][i].Size() ; j ++) {
	    		if (i == 0)
	    			this->_map->setSize(glm::vec2(grid[0][i].Size(), grid[0].Size()));
	    		if (!grid[0][i][j].IsInt())
	    			return ;
				
	    		int entityType = grid[0][i][j].GetInt();

	    		if (entityType == -2) // undestroyable bloc
	    			this->_map->addIndestructibleBlocs(IndestructibleBloc(glm::vec2(j,i)));
	    		if (entityType == 0 && ((rand() % 10) < 6)) // destroyable bloc
	    			this->_map->addDestructibleBlocs(DestructibleBloc(glm::vec2(j,i)));
	    		// if (entityType == 0) // case vide
	    		// 	break;
				
	    		// if (entityType >=1 && entityType <=5) {// players (1 is human, the rest is an AI)
	    		// 	glm::vec2		vec(static_cast<float>(j), static_cast<float>(i));
	    		// 	Player *	player = new Player(vec, entityType - 1);
				// 	if (player->getPlayerNb() == 0)
				// 		_playerManager->setHumanPlayer(player);
				// 	else
				// 		_playerManager->addPlayer(player);
	    		// 	this->_entityList->push_back(player);
	    		// }
			}
		}
	}
	else if (_gameParams.get_game_mode() == GameMode::CAMPAIGN)
	{

	    for (unsigned int i = 0 ; i < grid[0].Size() ; i++)
	    {
	    	if (!grid[0][i].IsArray())
	    		return ;
	    	for (unsigned int j = 0 ; j < grid[0][i].Size() ; j ++) {
	    		if (i == 0)
	    			this->_map->setSize(glm::vec2(grid[0][i].Size(), grid[0].Size()));
	    		if (!grid[0][i][j].IsInt())
	    			return ;
				
	    		int entityType = grid[0][i][j].GetInt();

	    		switch (entityType)
	    		{
	    		case -2: // undestroyable bloc
	    			this->_map->addIndestructibleBlocs(IndestructibleBloc(glm::vec2(j,i)));
	    			break;
	    		case -1: // destroyable bloc
	    			this->_map->addDestructibleBlocs(DestructibleBloc(glm::vec2(j,i)));
	    			break;
	    		case 0: // case vide
	    		 	break;

	    		case 1: // players (1 is human, the rest is an AI)
	    			vec = glm::vec2(static_cast<float>(j), static_cast<float>(i));
	    			player = new Player(vec, 0, _gameParams.get_color());
					// _playerManager->addPlayer(player);
	    			this->_entityList->push_back(player);
					_playerManager->setHumanPlayer(player);
	    			break;
	    		case 2:
	    			vec = glm::vec2(static_cast<float>(j), static_cast<float>(i));
	    			enemy = new Enemy(vec, EnemyType::BALOON);
	    			this->_entityList->push_back(enemy);
	    			break;
	    		case 3:
	    		case 4:
	    		case 5:
	    		case 6:
	    		case 7:
	    		case 8:
	    		case 9:
	    			break;
	    		case 10: // power-bomb +1
	    			vec = glm::vec2(static_cast<float>(j), static_cast<float>(i));
	    			bonus = new Bonus(vec, BonusType::FLAME_UP);
	    			this->_entityList->push_back(bonus);
	    			break;
	    		case 11: // power-bomb -1
	    			// vec = glm::vec2(static_cast<float>(j), static_cast<float>(i));
	    			// bonus = new Bonus(vec, BonusType::FLAME_DOWN);
	    			// this->_entityList->push_back(bonus);
	    			break;
	    		case 12: // nb-bomb +1
	    			vec = glm::vec2(static_cast<float>(j), static_cast<float>(i));
	    			bonus = new Bonus(vec, BonusType::BOMB_UP);
	    			this->_entityList->push_back(bonus);
	    			break;
	    		case 13: // nb-bomb -1
	    			// vec = glm::vec2(static_cast<float>(j), static_cast<float>(i));
	    			// bonus = new Bonus(vec, BonusType::BOMB_DOWN);
	    			// this->_entityList->push_back(bonus);
	    			break;
	    		case 14: // speed +1
	    			vec = glm::vec2(static_cast<float>(j), static_cast<float>(i));
	    			bonus = new Bonus(vec, BonusType::SPEED_UP);
	    			this->_entityList->push_back(bonus);
	    			break;
	    		case 15: // speed -1
	    			// vec = glm::vec2(static_cast<float>(j), static_cast<float>(i));
	    			// bonus = new Bonus(vec, BonusType::SPEED_DOWN);
	    			// this->_entityList->push_back(bonus);
	    			break;
	    		case 16:
	    		case 17:
	    		case 18:
	    		case 19:
	    			break;

	    		}

			}
		}
	}
	// OTHER ATTRIBUTES
	rapidjson::Value *win;
	win = this->_loader.getValue("win");
	if (!win || !win->HasMember("condition") || !win->HasMember("spot") || !win[0]["spot"].IsArray() || win[0]["spot"].Size() != 2)
		return;
	_winManager = new WinManager(static_cast<WinCondition::Enum>(win[0]["condition"].GetInt()), _gameParams.get_game_mode(), glm::ivec2(win[0]["spot"][0].GetFloat(), win[0]["spot"][1].GetFloat()));

    sun = this->_loader.getValue("sun");
    if (!sun || !sun->HasMember("pos") || !sun[0]["pos"].IsArray() || sun[0]["pos"].Size() != 3)
        return ;
    if (!sun[0]["pos"][0].IsFloat() || !sun[0]["pos"][1].IsFloat() || !sun[0]["pos"][2].IsFloat())
        return ;
    this->_map->setSunPos(glm::vec3(sun[0]["pos"][0].GetFloat(), sun[0]["pos"][1].GetFloat(), sun[0]["pos"][2].GetFloat()));

    if (!sun || !sun->HasMember("color") || !sun[0]["color"].IsArray() || sun[0]["color"].Size() != 3)
        return ;
    if (!sun[0]["color"][0].IsFloat() || !sun[0]["color"][1].IsFloat() || !sun[0]["color"][2].IsFloat())
        return ;
    this->_map->setSunColor(glm::vec3(sun[0]["color"][0].GetFloat(), sun[0]["color"][1].GetFloat(), sun[0]["color"][2].GetFloat()));

	rapidjson::Value *enemies = this->_loader.getValue("enemies");
	if (!enemies || !enemies->IsArray())
		return;
	for (unsigned int i = 0 ; i < enemies[0].Size() ; i++){
		if (!enemies[0][i].HasMember("pos") || !enemies[0][i].HasMember("type") || !enemies[0][i]["pos"].IsArray() || enemies[0][i]["pos"].Size() != 2 || !enemies[0][i]["pos"][0].IsInt() || !enemies[0][i]["pos"][1].IsInt() || !enemies[0][i]["type"].IsInt())
			return;
		_entityList->push_back(new Enemy(glm::vec2(enemies[0][i]["pos"][0].GetInt(), enemies[0][i]["pos"][1].GetInt()), static_cast<EnemyType::Enum>(enemies[0][i]["type"].GetInt())));
	}

}

void			GameEngine::gameWin(void *)
{
	rapidjson::Value *	stars;
	int					star_result = 0;

	_win = true;
	if (_gameParams.get_game_mode() == GameMode::CAMPAIGN)
	{
		for (auto i = _entityList->begin(); i != _entityList->end(); i++){
			if ((*i)->getType() == Type::PLAYER){
				stars = this->_loader.getValue("stars");
				if (stars && stars[0].IsArray() && stars[0].Size() == 3)
				{
					if (stars[0][0].IsInt() && static_cast<Player*>(*i)->getTotalBombCount() <= stars[0][0].GetInt())
						star_result = 3;
					else if (stars[0][1].IsInt() && static_cast<Player*>(*i)->getTotalBombCount() <= stars[0][1].GetInt())
						star_result = 2;
					else if (stars[0][2].IsInt() && static_cast<Player*>(*i)->getTotalBombCount() <= stars[0][2].GetInt())
						star_result = 1;
				}
			}
		}
		Menu::Enum menu = Menu::START;
		SEventManager::getInstance().raise(Event::GUI_TOGGLE, &menu);
		menu = Menu::LEVEL_SELECTION;
		SEventManager::getInstance().raise(Event::GUI_TOGGLE, &menu);
    	SEventManager::getInstance().raise(Event::GAME_FINISH, nullptr);
	}
	else {
		Menu::Enum menu = Menu::BRAWL_WIN;
		SEventManager::getInstance().raise(Event::GUI_TOGGLE, &menu);
	}
	SEventManager::getInstance().raise(Event::GAME_FINISH, nullptr);
}


void			GameEngine::gameOver(void *)
{
	SEventManager::getInstance().raise(Event::GAME_FINISH, nullptr);
    SEventManager::getInstance().raise(Event::GUI_TOGGLE, new Menu::Enum(Menu::GAME_OVER));
}

GameParams		GameEngine::getGameParams(void) const
{
	return _gameParams;
}

int 			GameEngine::getStarsCampaign(void)
{
	rapidjson::Value *	stars;
	int					star_result = 0;

	for (auto i = _entityList->begin(); i != _entityList->end(); i++){
		if ((*i)->getType() == Type::PLAYER){
			stars = this->_loader.getValue("stars");
			if (stars && stars[0].IsArray() && stars[0].Size() == 3)
			{
				if (stars[0][0].IsInt() && static_cast<Player*>(*i)->getTotalBombCount() <= stars[0][0].GetInt())
					star_result = 3;
				else if (stars[0][1].IsInt() && static_cast<Player*>(*i)->getTotalBombCount() <= stars[0][1].GetInt())
					star_result = 2;
				else if (stars[0][2].IsInt() && static_cast<Player*>(*i)->getTotalBombCount() <= stars[0][2].GetInt())
					star_result = 1;
			}

			return star_result;
		}
	}
	return -1;
}

bool		GameEngine::getWin(void) const
{
	return _win;
}


void		GameEngine::pause(void *) {
	_playerManager->unRegister();
}


void		GameEngine::unPause(void *) {
	_playerManager->Register();
}