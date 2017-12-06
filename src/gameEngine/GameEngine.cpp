/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameEngine.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 16:14:09 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/06 16:22:04 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Player.hpp"
#include "GameEngine.hpp"
#include "glm/ext.hpp"
#include "IndestructibleBloc.hpp"
#include "DestructibleBloc.hpp"

GameEngine::GameEngine(GameMode::Enum gm) : _map(new Map()), _playerManager(new PlayerManager()), _gameMode(gm), _winManager(nullptr) {
	loadMap("maps/map.json");

}

GameEngine::~GameEngine() {}

void	GameEngine::compute(std::vector<Action::Enum> &actions) {
    if (this->_loader.getState() == -1 || !_winManager)
        return ;
	_playerManager->compute(*_map, _entityList);
	_collisionsManager.moves(*_map, _entityList);
	_bombManager.update(*_map, _entityList, actions);
	_winManager->update(*_map, _entityList);
}

Map const &		GameEngine::getMap() const {
	return *(this->_map);
}

std::vector<IGameEntity * > &	GameEngine::getEntityList() {
	return this->_entityList;
}


glm::vec2 const 				* GameEngine::getPlayerPos(void) const{
	for (auto i = _entityList.begin(); i != _entityList.end(); i++){
		if ((*i)->getType() == Type::PLAYER){
			return new glm::vec2((*i)->getPosition());
		}
	}
	return new glm::vec2(0., 0.);
}

void					GameEngine::loadMap(const char *path){
    rapidjson::Value * grid;
    rapidjson::Value * sun;

	this->_loader.setPath(path);
	if (this->_loader.load() != 1)
	{
		return ;
	}
	grid = this->_loader.getValue("grid");
	if (grid && !grid[0].IsArray())
    	return ;

	// MAP
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
    		if (entityType == -1) // destroyable bloc
    			this->_map->addDestructibleBlocs(DestructibleBloc(glm::vec2(j,i)));
    		// if (entityType == 0) // case vide
    		// 	break;
    		if (entityType >=1 && entityType <=4) {// players (1 is human, the rest is an AI)
    			glm::vec2		vec(static_cast<float>(j), static_cast<float>(i));
    			Player *	player = new Player(vec, entityType - 1);
				_playerManager->addPlayer(player);
    			this->_entityList.push_back(player);
				if (player->getPlayerNb() == 0)
					_playerManager->setHumanPlayer(player);
    		}
		}
	}
	// OTHER ATTRIBUTES
	rapidjson::Value *win;
	win = this->_loader.getValue("win");
	if (!win || !win->HasMember("condition") || !win->HasMember("spot") || !win[0]["spot"].IsArray() || win[0]["spot"].Size() != 2)
		return;
	_winManager = new WinManager(static_cast<WinCondition::Enum>(win[0]["condition"].GetInt()), glm::ivec2(win[0]["spot"][0].GetFloat(), win[0]["spot"][1].GetFloat()));

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


}