/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BombManager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/28 13:24:20 by egaborea          #+#    #+#             */
/*   Updated: 2017/11/28 17:08:16 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BombManager.hpp"
#include "glm/ext.hpp"

BombManager::BombManager() : _spawned_bomb(false) {}

BombManager::~BombManager() {}

void    BombManager::update(std::vector<IGameEntity *> & entityList, std::vector<Action::Enum> const actions){
    Bomb    *new_bomb = NULL;
    if (_spawned_bomb && find(actions.begin(), actions.end(), Action::SPAWN_BOMB) == actions.end()){
        _spawned_bomb = false;
    }
    for (auto i = entityList.begin(); i != entityList.end(); i++){
        switch ((*i)->getType()) {
            case Type::PLAYER:
                if (find(actions.begin(), actions.end(), Action::SPAWN_BOMB) != actions.end() && _spawned_bomb == false){
                    //std::cout << glm::to_string((*i)->getPosition()) << " => " << glm::to_string(glm::floor((*i)->getPosition())) << std::endl;
                    new_bomb = new Bomb(glm::floor((*i)->getPosition()));
                    _spawned_bomb = true;
                }
                break;
            case Type::BOMB:
                (*i)->update();
                break;
            default:
                break;
        }
    }
    for (auto i = entityList.begin(); i != entityList.end(); i++){
        if (new_bomb && (*i)->getPosition() == new_bomb->getPosition()){
            delete new_bomb;
            new_bomb = NULL;
        }
    }
    if (new_bomb){
        entityList.push_back(new_bomb);
    }
    entityList.erase(
        std::remove_if(entityList.begin(), entityList.end(), 
            [](const IGameEntity * e) {
                return e->getState() == State::DYING;
            }),
        entityList.end());
}