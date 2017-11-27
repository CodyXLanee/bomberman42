/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameEngine.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 16:14:09 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/27 04:27:17 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GameEntity.hpp"
#include "GameEngine.hpp"
#include "glm/ext.hpp"

GameEngine::GameEngine() : map(new Map("maps/map.json")) {
	entityList.push_back(new GameEntity(Type::PLAYER));

	map->load();
}

GameEngine::~GameEngine() {}

void	GameEngine::compute(std::vector<Action::Enum> actions) {
	for (std::vector<IGameEntity *>::iterator i = entityList.begin(); i != entityList.end(); i++) {
		switch((*i)->getType()){
			case Type::PLAYER:	compute_player(*i, actions);
			default:			(void)actions;
		}
	}
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
	if (p->getState() == State::MOVING){
		p->setPosition(p->getPosition() + (p->getDirection() * p->getSpeed()));
	}
}

Map const &		GameEngine::getMap() const {
	return *map;
}

const std::vector<IGameEntity * >	GameEngine::getEntityList() const {
	return entityList;
}