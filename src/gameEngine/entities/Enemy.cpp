/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Enemy.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/14 16:37:22 by egaborea          #+#    #+#             */
/*   Updated: 2018/01/25 10:50:56 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Enemy.hpp"

Enemy::Enemy(const glm::vec2 & pos, EnemyType::Enum type) :
    AGameEntity(pos, glm::vec2(0., -1.), State::MOVING, 0.01f, Type::ENEMY),
    _type(type), _graphicalDirection(glm::vec2(0,0)), _counter_dying(200) {

    SEventManager::getInstance().registerEvent(Event::PLAYER_MOVE, MEMBER_CALLBACK(Enemy::player_move_callback));
	SEventManager::getInstance().registerEvent(Event::SPAWN_FLAME, MEMBER_CALLBACK(Enemy::spawn_flame_callback));
}
Enemy::~Enemy(){
    SEventManager::getInstance().unRegisterEvent(Event::PLAYER_MOVE, this);
	SEventManager::getInstance().unRegisterEvent(Event::SPAWN_FLAME, this);
}


glm::vec2   Enemy::getGraphicalDirection() const
{
    return _graphicalDirection;
}

void        Enemy::setGraphicalDirection(glm::vec2 dir)
{
    _graphicalDirection = dir;
}

int        Enemy::getCounterDying() const
{
    return _counter_dying;
}

void        Enemy::setCounterDying(int counter)
{
    _counter_dying = counter;
}

EnemyType::Enum         Enemy::getEnemyType(void){
    return _type;
}

void    Enemy::player_move_callback(void *player){
    if (glm::round(static_cast<IGameEntity *>(player)->getPosition()) == getPosition())
        SEventManager::getInstance().raise(Event::PLAYER_DIES, player);
}

void	Enemy::spawn_flame_callback(void *flame){
	if (static_cast<IGameEntity *>(flame)->getPosition() == glm::round(getPosition()))
		this->setState(State::DYING);
}

Enemy::Enemy(void) :
    AGameEntity(glm::vec2(0., 0.), glm::vec2(0., -1.), State::MOVING, 0.01f, Type::ENEMY),
    _type(EnemyType::BALOON), _graphicalDirection(glm::vec2(0,0)), _counter_dying(200) {

    SEventManager::getInstance().registerEvent(Event::PLAYER_MOVE, MEMBER_CALLBACK(Enemy::player_move_callback));
	SEventManager::getInstance().registerEvent(Event::SPAWN_FLAME, MEMBER_CALLBACK(Enemy::spawn_flame_callback));
}


Enemy::Enemy(Enemy const &e) :
    AGameEntity(e._position, glm::vec2(0., -1.), State::MOVING, 0.01f, Type::ENEMY),
    _type(e._type), _graphicalDirection(glm::vec2(0,0)), _counter_dying(200) {

    SEventManager::getInstance().registerEvent(Event::PLAYER_MOVE, MEMBER_CALLBACK(Enemy::player_move_callback));
	SEventManager::getInstance().registerEvent(Event::SPAWN_FLAME, MEMBER_CALLBACK(Enemy::spawn_flame_callback));
}


Enemy   &Enemy::operator=(Enemy const &rhs){
    _type = rhs._type;
    _graphicalDirection = rhs._graphicalDirection;
    _counter_dying = rhs._counter_dying;
    return *this;
}
