/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameEngine.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 16:14:09 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/24 15:51:29 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GameEntity.hpp"
#include "GameEngine.hpp"

GameEngine::GameEngine() : map(new Map("maps/map.json")) {
	entityList.push_back(new GameEntity(Type::PLAYER));

	map->load();
}

GameEngine::~GameEngine() {}

void	GameEngine::compute(std::vector<Action::Enum> actions) {
	(void)actions; //////////////////
}

Map const &		GameEngine::getMap() const {
	return *map;
}

std::vector<IGameEntity const * >	GameEngine::getEntityList() const {
	return entityList;
}