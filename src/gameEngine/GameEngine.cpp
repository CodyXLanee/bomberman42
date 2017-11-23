/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameEngine.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 16:14:09 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/23 17:16:57 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GameEngine.hpp"

GameEngine::GameEngine() {}

GameEngine::~GameEngine() {}

void	GameEngine::compute(std::vector<Action::Enum> actions) {
	(void)actions; //////////////////
}

Map const &		GameEngine::getMap() const {
	return map;
}

std::vector<IGameEntity const * >	GameEngine::getEntityList() const {
	return entityList;
}