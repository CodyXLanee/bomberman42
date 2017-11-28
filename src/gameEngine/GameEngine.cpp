/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameEngine.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 16:14:09 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/27 16:18:09 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Player.hpp"
#include "GameEngine.hpp"
#include "glm/ext.hpp"
#include "IndestructibleBloc.hpp"
#include "DestructibleBloc.hpp"

GameEngine::GameEngine() : _map(new Map()) {
	rapidjson::Value * grid;

	this->_loader.setPath("maps/map.json");
	if (this->_loader.load() != 1)
	{
		return ;
	}

	grid = this->_loader.getValue("grid");
	if (!grid[0].IsArray())
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
}

GameEngine::~GameEngine() {}

void	GameEngine::compute(std::vector<Action::Enum> actions) {
	collisionsManageV2(actions);
}

glm::vec2	GameEngine::compute_direction(std::vector<Action::Enum> actions) {
	glm::vec2 v(0., 0.);
	for (std::vector<Action::Enum>::iterator i = actions.begin(); i != actions.end(); i++) {
		switch (*i) {
			case Action::LEFT: 		v.x += -1.; break;
			case Action::RIGHT: 	v.x += 1.; break;
			case Action::UP: 		v.y += 1.; break;
			case Action::DOWN: 		v.y += -1.; break;
			default:				(void)v;
		}
	}
	return (glm::normalize(v));
}

void	GameEngine::compute_player(IGameEntity *p, std::vector<Action::Enum> actions){
	glm::vec2	v = compute_direction(actions);
	(void)p;
	if (!std::isnan(v.x) && !std::isnan(v.y)){ // glm::isnan wouldn't compile for some reason...
		p->setState(State::MOVING);
		p->setDirection(v);
		p->setSpeed(0.1);
	}
	else
	{
		p->setState(State::STANDING);
		p->setSpeed(0.);
	}
}

Map const &		GameEngine::getMap() const {
	return *(this->_map);
}

const std::vector<IGameEntity * >	GameEngine::getEntityList() const {
	return this->_entityList;
}

void			GameEngine::collisionsManageV2(std::vector<Action::Enum> actions)
{
	for (std::vector<IGameEntity *>::iterator i = _entityList.begin(); i != _entityList.end(); i++) {
		switch((*i)->getType()){
			case Type::PLAYER:
				compute_player(*i, actions);
				if ((*i)->getState() == State::MOVING){
					glm::vec2	posToCheck = (*i)->getPosition() + ((*i)->getDirection() * (*i)->getSpeed());
					glm::vec2	newPos = (*i)->getPosition() + ((*i)->getDirection() * (*i)->getSpeed());
					int			isBloced = false;
					
					// border map
					if (posToCheck.x < 0)
						newPos.x = 0;
					if (posToCheck.x > (_map->getSize().x - 1))
						newPos.x = _map->getSize().x - 1;
					if (posToCheck.y < 0)
						newPos.y = 0;
					if (posToCheck.y > (_map->getSize().y - 1))
						newPos.y = _map->getSize().y - 1;
					
					// bloc realy front of
					if ((*i)->getDirection().x > 0)
					{
						if (_map->haveBloc(glm::vec2(round(posToCheck.x + RPLAYER), round(posToCheck.y))))
						{
							isBloced = true;
							newPos.x = round(posToCheck.x + RPLAYER) - RPLAYER - 0.5;
						}
					}
					if ((*i)->getDirection().x < 0)
					{
						if (_map->haveBloc(glm::vec2(round(posToCheck.x - RPLAYER), round(posToCheck.y))))
						{
							isBloced = true;
							newPos.x = round(posToCheck.x - RPLAYER) + RPLAYER + 0.5;
						}
					}
					if ((*i)->getDirection().y > 0)
					{
						if (_map->haveBloc(glm::vec2(round(posToCheck.x), round(posToCheck.y + RPLAYER))))
						{
							isBloced = true;
							newPos.y = round(posToCheck.y + RPLAYER) - RPLAYER - 0.5;
						}
					}
					if ((*i)->getDirection().y < 0)
					{
						if (_map->haveBloc(glm::vec2(round(posToCheck.x), round(posToCheck.y - RPLAYER))))
						{
							isBloced = true;
							newPos.y = round(posToCheck.y - RPLAYER) + RPLAYER + 0.5;
						}
					}

					// gestion border
					if (!isBloced && (*i)->getDirection().x > 0)
					{
						if (_map->haveBloc(glm::vec2(round(posToCheck.x + RPLAYER), ceil(posToCheck.y))))
						{
							glm::vec2	angle = glm::vec2(round(posToCheck.x + RPLAYER), ceil(posToCheck.y));
							angle.x -= 0.5;
							angle.y -= 0.5;
							while (distance(newPos, angle) < RPLAYER)
							{
								newPos.x -= 0.01;
								newPos.y -= 0.01;
							}
						}
						if (_map->haveBloc(glm::vec2(round(posToCheck.x + RPLAYER), floor(posToCheck.y))))
						{
							glm::vec2	angle = glm::vec2(round(posToCheck.x + RPLAYER), floor(posToCheck.y));
							angle.x -= 0.5;
							angle.y += 0.5;
							while (distance(newPos, angle) < RPLAYER)
							{
								newPos.x -= 0.01;
								newPos.y += 0.01;
							}
						}
					}
					if (!isBloced && (*i)->getDirection().x < 0)
					{
						if (_map->haveBloc(glm::vec2(round(posToCheck.x - RPLAYER), ceil(posToCheck.y))))
						{
							glm::vec2	angle = glm::vec2(round(posToCheck.x - RPLAYER), ceil(posToCheck.y));
							angle.x += 0.5;
							angle.y -= 0.5;
							while (distance(newPos, angle) < RPLAYER)
							{
								newPos.x += 0.01;
								newPos.y -= 0.01;
							}
						}
						if (_map->haveBloc(glm::vec2(round(posToCheck.x - RPLAYER), floor(posToCheck.y))))
						{
							glm::vec2	angle = glm::vec2(round(posToCheck.x - RPLAYER), floor(posToCheck.y));
							angle.x += 0.5;
							angle.y += 0.5;
							while (distance(newPos, angle) < RPLAYER)
							{
								newPos.x += 0.01;
								newPos.y += 0.01;
							}
						}
					}
					if (!isBloced && (*i)->getDirection().y > 0)
					{
						if (_map->haveBloc(glm::vec2(ceil(posToCheck.x), round(posToCheck.y + RPLAYER))))
						{
							glm::vec2	angle = glm::vec2(ceil(posToCheck.x), round(posToCheck.y + RPLAYER));
							angle.x -= 0.5;
							angle.y -= 0.5;
							while (distance(newPos, angle) < RPLAYER)
							{
								newPos.x -= 0.01;
								newPos.y -= 0.01;
							}
						}
						if (_map->haveBloc(glm::vec2(floor(posToCheck.x), round(posToCheck.y + RPLAYER))))
						{
							glm::vec2	angle = glm::vec2(floor(posToCheck.x), round(posToCheck.y + RPLAYER));
							angle.x += 0.5;
							angle.y -= 0.5;
							while (distance(newPos, angle) < RPLAYER)
							{
								newPos.x += 0.01;
								newPos.y -= 0.01;
							}
						}
					}
					if (!isBloced && (*i)->getDirection().y < 0)
					{
						if (_map->haveBloc(glm::vec2(ceil(posToCheck.x), round(posToCheck.y - RPLAYER))))
						{
							glm::vec2	angle = glm::vec2(ceil(posToCheck.x), round(posToCheck.y - RPLAYER));
							angle.x -= 0.5;
							angle.y += 0.5;
							while (distance(newPos, angle) < RPLAYER)
							{
								newPos.x -= 0.01;
								newPos.y += 0.01;
							}
						}
						if (_map->haveBloc(glm::vec2(floor(posToCheck.x), round(posToCheck.y - RPLAYER))))
						{
							glm::vec2	angle = glm::vec2(floor(posToCheck.x), round(posToCheck.y - RPLAYER));
							angle.x += 0.5;
							angle.y += 0.5;
							while (distance(newPos, angle) < RPLAYER)
							{
								newPos.x += 0.01;
								newPos.y += 0.01;
							}
						}
					}

					// set the new position
					(*i)->setPosition(newPos);
				}
				break;
			default:
				(void)actions;
				break;
		}
	}
}

void			GameEngine::collisionsManageV1(std::vector<Action::Enum> actions)
{
	for (std::vector<IGameEntity *>::iterator i = _entityList.begin(); i != _entityList.end(); i++) {
		switch((*i)->getType()){
			case Type::PLAYER:
				compute_player(*i, actions);
				if ((*i)->getState() == State::MOVING){
					glm::vec2 posToCheck = (*i)->getPosition() + ((*i)->getDirection() * (*i)->getSpeed());
					glm::vec2 newPos = (*i)->getPosition() + ((*i)->getDirection() * (*i)->getSpeed());
					if (posToCheck.x < 0)
						newPos.x = 0;
					if (posToCheck.x > (_map->getSize().x - 1))
						newPos.x = _map->getSize().x - 1;
					if (posToCheck.y < 0)
						newPos.y = 0;
					if (posToCheck.y > (_map->getSize().y - 1))
						newPos.y = _map->getSize().y - 1;
					if ((*i)->getDirection().x > 0)
						if (_map->haveBloc(glm::vec2(ceil(posToCheck.x - (0.5 - RPLAYER)), ceil(posToCheck.y - (0.5 - RPLAYER) - 0.1))) || _map->haveBloc(glm::vec2(ceil(posToCheck.x - (0.5 - RPLAYER)), floor(posToCheck.y + (0.5 - RPLAYER) + 0.1))))
							newPos.x = ceil(posToCheck.x - 0.1) - 1.0 + (0.5 - RPLAYER);
					if ((*i)->getDirection().x < 0)
						if (_map->haveBloc(glm::vec2(floor(posToCheck.x + (0.5 - RPLAYER)), ceil(posToCheck.y - (0.5 - RPLAYER) - 0.1))) || _map->haveBloc(glm::vec2(floor(posToCheck.x + (0.5 - RPLAYER)), floor(posToCheck.y + (0.5 - RPLAYER) + 0.1))))
							newPos.x = floor(posToCheck.x + 0.1) + 1.0 - (0.5 - RPLAYER);
					if ((*i)->getDirection().y > 0)
						if (_map->haveBloc(glm::vec2(ceil(posToCheck.x - (0.5 - RPLAYER) - 0.1), ceil(posToCheck.y - (0.5 - RPLAYER)))) || _map->haveBloc(glm::vec2(floor(posToCheck.x + (0.5 - RPLAYER) + 0.1), ceil(posToCheck.y - (0.5 - RPLAYER)))))
							newPos.y = ceil(posToCheck.y - 0.1) - 1.0 + (0.5 - RPLAYER);
					if ((*i)->getDirection().y < 0)
						if (_map->haveBloc(glm::vec2(floor(posToCheck.x + (0.5 - RPLAYER) + 0.1), floor(posToCheck.y + (0.5 - RPLAYER)))) || _map->haveBloc(glm::vec2(ceil(posToCheck.x - (0.5 - RPLAYER) - 0.1), floor(posToCheck.y + (0.5 - RPLAYER)))))
							newPos.y = floor(posToCheck.y + 0.1) + 1.0 - (0.5 - RPLAYER);
					(*i)->setPosition(newPos);
				}
				break;
			default:
				(void)actions;
				break;
		}
	}
}