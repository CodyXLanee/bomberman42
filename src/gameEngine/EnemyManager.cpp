/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EnemyManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/14 19:20:30 by egaborea          #+#    #+#             */
/*   Updated: 2018/01/04 14:31:40 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "EnemyManager.hpp"
# include <glm/ext.hpp>

EnemyManager::EnemyManager(std::vector<IGameEntity *> *entityList, Map *map) : _entity_list(entityList), _map(map){
	SEventManager::getInstance().registerEvent(Event::ENEMY_COLLIDES, MEMBER_CALLBACK(EnemyManager::enemyCollidesCallback));
    SEventManager::getInstance().registerEvent(Event::ENEMY_MOVE, MEMBER_CALLBACK(EnemyManager::enemyMoveCallback));
	SEventManager::getInstance().registerEvent(Event::ENEMY_DIES, MEMBER_CALLBACK(EnemyManager::enemyDiesCallback));
}

EnemyManager::~EnemyManager(){
	SEventManager::getInstance().unRegisterEvent(Event::ENEMY_COLLIDES, this);
    SEventManager::getInstance().unRegisterEvent(Event::ENEMY_MOVE, this);
	SEventManager::getInstance().unRegisterEvent(Event::ENEMY_DIES, this);
}

void			EnemyManager::update(void){
    for (auto &&i : *_entity_list){
        if (i->getType() == Type::ENEMY)
        {
            if (i->getState() == State::DYING)
                dynamic_cast<Enemy*>(i)->setCounterDying(dynamic_cast<Enemy*>(i)->getCounterDying() - 1);
            if (dynamic_cast<Enemy*>(i)->getCounterDying() <= 0)
            {
                glm::vec2   pos(round(i->getPosition().x), round(i->getPosition().y));
                Bomb    *b = new Bomb(pos);
                delete b;
                delete i;
                i = nullptr;
            }
        }
    }
    _entity_list->erase( std::remove(_entity_list->begin(), _entity_list->end(), nullptr), _entity_list->end());
}

void            EnemyManager::enemyCollidesCallback(void *entity){
    Enemy   *enemy = static_cast<Enemy *>(entity);
    if (enemy->getEnemyType() == EnemyType::BALOON){
        enemy->setDirection(updateDirectionBaloon(enemy->getDirection(), enemy->getPosition()));
    }
}

void            EnemyManager::enemyMoveCallback(void *entity){
    static int count = 0;
    Enemy   *enemy = static_cast<Enemy *>(entity);
    glm::vec2   direction = enemy->getDirection();
    glm::vec2   pos = glm::vec2(round(enemy->getPosition().x), round(enemy->getPosition().y));
    count--;

    if (enemy->getEnemyType() == EnemyType::BALOON){
        if (direction == glm::vec2(-1, 0) && round(enemy->getPosition().x + 0.49) == floor(enemy->getPosition().x) && count < 0)
        {
            pos.y++;
            if (!_map->hasBloc(pos))
            {
                count = 5;
                enemy->setDirection(glm::vec2(0, 1));
            }
            return ;
        }
        if (direction == glm::vec2(0, -1) && round(enemy->getPosition().y + 0.49) == floor(enemy->getPosition().y) && count < 0)
        {
            pos.x--;
            if (!_map->hasBloc(pos))
            {
                count = 5;
                enemy->setDirection(glm::vec2(-1, 0));
            }
            return ;
        }
        if (direction == glm::vec2(1, 0) && round(enemy->getPosition().x - 0.49) == ceil(enemy->getPosition().x) && count < 0)
        {
            pos.y--;
            if (!_map->hasBloc(pos))
            {
                count = 5;
                enemy->setDirection(glm::vec2(0, -1));
            }
            return ;
        }
        if (direction == glm::vec2(0, 1) && round(enemy->getPosition().y - 0.49) == ceil(enemy->getPosition().y) && count < 0)
        {
            pos.x++;
            if (!_map->hasBloc(pos))
            {
                count = 5;
                enemy->setDirection(glm::vec2(1, 0));
            }
            return ;
        }
    }
}

void            EnemyManager::enemyDiesCallback(void *enemy){
    static_cast<Enemy *>(enemy)->setState(State::DYING);
}

void            EnemyManager::setMap(Map *map)
{
    _map = map;
}

Map             *EnemyManager::getMap(void) const
{
    return _map;
}

glm::vec2       EnemyManager::updateDirectionBaloon(glm::vec2 direction, glm::vec2 position){
    glm::vec2 pos(floor(position.x), floor(position.y));

    if (direction == glm::vec2(-1, 0))
    {
        pos.y++;
        if (!_map->hasBloc(pos))
            return glm::vec2(0, 1);
        return glm::vec2(1,0);
    }
    if (direction == glm::vec2(0, -1))
    {
        pos.x--;
        if (!_map->hasBloc(pos))
            return glm::vec2(-1, 0);
        return glm::vec2(0, 1);
    }
    if (direction == glm::vec2(1, 0))
    {
        pos.y--;
        if (!_map->hasBloc(pos))
            return glm::vec2(0, -1);
        return glm::vec2(-1, 0);
    }
    if (direction == glm::vec2(0, 1))
    {
        pos.x++;
        if (!_map->hasBloc(pos))
            return glm::vec2(1, 0);
        return glm::vec2(0, -1);
    }
    return glm::vec2(0, -1);
}