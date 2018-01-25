/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bonus.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/11 10:47:21 by egaborea          #+#    #+#             */
/*   Updated: 2018/01/25 10:39:29 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bonus.hpp"

Bonus::Bonus(const glm::vec2 & pos, BonusType::Enum type) :
AGameEntity(pos, glm::vec2(0., -1.), State::STANDING, 0., Type::BONUS), _bonus_type(type) {
    SEventManager::getInstance().registerEvent(Event::PLAYER_MOVE, MEMBER_CALLBACK(Bonus::playerMoveCallback));
}

Bonus::~Bonus(){
    SEventManager::getInstance().unRegisterEvent(Event::PLAYER_MOVE, this);
}
    
void    Bonus::playerMoveCallback(void *p){
    if (glm::round(static_cast<IGameEntity *>(p)->getPosition()) == getPosition()){
        std::pair<IGameEntity *, IGameEntity *> *pair = new std::pair<IGameEntity *, IGameEntity *>(this, static_cast<IGameEntity *>(p));
        SEventManager::getInstance().raise(Event::BONUS_ACTIVATE, pair);
        delete pair;
    }
}


BonusType::Enum     Bonus::getBonusType(void){
    return _bonus_type;
}

Bonus::Bonus(void):
AGameEntity(glm::vec2(0., 0.), glm::vec2(0., -1.), State::STANDING, 0., Type::BONUS), _bonus_type(BonusType::BOMB_UP) {
    SEventManager::getInstance().registerEvent(Event::PLAYER_MOVE, MEMBER_CALLBACK(Bonus::playerMoveCallback));
}


Bonus::Bonus(Bonus const &b):
AGameEntity(b._position, glm::vec2(0., -1.), State::STANDING, 0., Type::BONUS), _bonus_type(b._bonus_type) {
    SEventManager::getInstance().registerEvent(Event::PLAYER_MOVE, MEMBER_CALLBACK(Bonus::playerMoveCallback));
}

Bonus   &Bonus::operator=(Bonus const &rhs){
    _bonus_type = rhs._bonus_type;
    return *this;
}