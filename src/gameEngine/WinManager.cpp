/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WinManager.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/05 17:35:04 by egaborea          #+#    #+#             */
/*   Updated: 2017/12/05 18:31:10 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WinManager.hpp"

WinManager::WinManager(WinCondition::Enum condition, glm::vec2 spot) : _win_condition(condition), _win_spot(spot), _won(false){
    SEventManager::getInstance().registerEvent(Event::GAME_WIN, MEMBER_CALLBACK(WinManager::win));
}

WinManager::~WinManager(){
}



void	WinManager::update(Map & map, std::vector<IGameEntity *> &entityList){
    if (_won)
        return;

    switch(_win_condition) {
        case WinCondition::NO_MORE_BLOCKS:  checkWinBlocks(map); break;
        case WinCondition::NO_MORE_ENEMIES:  checkWinEnemies(entityList); break;
    }
    if (_won)
        entityList.push_back(new WinSpot(_win_spot));
}

void        WinManager::checkWinBlocks(Map &map){
    if (map.getDestructibleBlocs().size() == 0)
        SEventManager::getInstance().raise(Event::GAME_WIN, nullptr);
}

void        WinManager::checkWinEnemies(std::vector<IGameEntity *> &entityList){
    for (auto it = entityList.begin(); it != entityList.end(); it++){
        if ((*it)->getType() == Type::ENEMY){
            return ;
        }
    }
    SEventManager::getInstance().raise(Event::GAME_WIN, nullptr);
}



void    WinManager::win(void *p){
    (void)p;
    _won = true;
}