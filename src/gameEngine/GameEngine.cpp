/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameEngine.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 16:14:09 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/30 11:05:52 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Player.hpp"
#include "GameEngine.hpp"
#include "glm/ext.hpp"
#include "IndestructibleBloc.hpp"
#include "DestructibleBloc.hpp"

GameEngine::GameEngine() : _map(new Map()) {
    rapidjson::Value * grid;
    rapidjson::Value * sun;

	this->_loader.setPath("maps/map.json");
	if (this->_loader.load() != 1)
	{
		return ;
	}

	grid = this->_loader.getValue("grid");
	if (grid && !grid[0].IsArray())
    	return ;

    for (unsigned int i = 0 ; i < grid[0].Size() ; i++)
    {
    	if (!grid[0][i].IsArray())
    		return ;
    	for (unsigned int j = 0 ; j < grid[0][i].Size() ; j ++)
    	{
    		if (i == 0)
    			this->_map->setSize(glm::vec2(grid[0][i].Size(), grid[0].Size()));
    		if (!grid[0][i][j].IsInt())
    			return ;
    		switch (grid[0][i][j].GetInt())
    		{
    		case -2: // undestroyable bloc
    			this->_map->addIndestructibleBlocs(IndestructibleBloc(glm::vec2(j,i)));
    			break;
    		case -1: // destroyable bloc
    			this->_map->addDestructibleBlocs(DestructibleBloc(glm::vec2(j,i)));
    			break;
    		case 0: // case vide
    			break;
    		case 1: // player set
    			glm::vec2		vec(static_cast<float>(j), static_cast<float>(i));
    			Player *	player = new Player(vec);
    			this->_entityList.push_back(player);
    			break;
    		}
    	}
    }

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

GameEngine::~GameEngine() {}

void	GameEngine::compute(std::vector<Action::Enum> &actions) {
    if (this->_loader.getState() == -1)
        return ;
	_collisionsManager.moves(*_map, _entityList, actions);
	_bombManager.update(*_map, _entityList, actions);
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