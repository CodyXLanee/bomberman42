/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EnemyManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/14 19:20:30 by egaborea          #+#    #+#             */
/*   Updated: 2017/12/14 22:45:17 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "EnemyManager.hpp"
# include <glm/ext.hpp>

EnemyManager::EnemyManager(std::vector<IGameEntity *> *entityList) : _entity_list(entityList){
	SEventManager::getInstance().registerEvent(Event::ENEMY_COLLIDES, MEMBER_CALLBACK(EnemyManager::enemyCollidesCallback));
	SEventManager::getInstance().registerEvent(Event::ENEMY_DIES, MEMBER_CALLBACK(EnemyManager::enemyDiesCallback));
}

EnemyManager::~EnemyManager(){
}

void			EnemyManager::update(void){
    for (auto &&i : *_entity_list){
        if (i->getType() == Type::ENEMY && i->getState() == State::DYING){
            delete i;
            i = nullptr;
        }
    }
    _entity_list->erase( std::remove(_entity_list->begin(), _entity_list->end(), nullptr), _entity_list->end());
}

void            EnemyManager::enemyCollidesCallback(void *entity){
    Enemy   *enemy = static_cast<Enemy *>(entity);
    if (enemy->getEnemyType() == EnemyType::BALOON){
        enemy->setDirection(updateDirectionBaloon(enemy->getDirection()));
    }
}

void            EnemyManager::enemyDiesCallback(void *enemy){
    static_cast<Enemy *>(enemy)->setState(State::DYING);
}

glm::vec2       EnemyManager::updateDirectionBaloon(glm::vec2 direction){
    if (direction == glm::vec2(-1, 0))
        return glm::vec2(0, 1);
    if (direction == glm::vec2(0, -1))
        return glm::vec2(-1, 0);
    if (direction == glm::vec2(1, 0))
        return glm::vec2(0, -1);
    if (direction == glm::vec2(0, 1))
        return glm::vec2(1, 0);
    return glm::vec2(0, -1);
}