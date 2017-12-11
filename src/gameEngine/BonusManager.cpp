/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BonusManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/11 10:59:46 by egaborea          #+#    #+#             */
/*   Updated: 2017/12/11 12:12:35 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BonusManager.hpp"

BonusManager::BonusManager(std::vector<IGameEntity *> *entityList) :
_entity_list(entityList){
    SEventManager::getInstance().registerEvent(Event::BRICK_BREAKS, MEMBER_CALLBACK(BonusManager::brickBreaksCallback));
    SEventManager::getInstance().registerEvent(Event::BONUS_ACTIVATE, MEMBER_CALLBACK(BonusManager::bonusActivateCallback));
}

BonusManager::~BonusManager(){
    SEventManager::getInstance().unRegisterEvent(Event::BRICK_BREAKS, this);
    SEventManager::getInstance().unRegisterEvent(Event::BONUS_ACTIVATE, this);
}

void			BonusManager::update(void){
    for (auto &&i : *_entity_list){
        if (i->getType() == Type::BONUS && i->getState() == State::DYING){
            delete i;
            i = nullptr;
        }
    }
    _entity_list->erase( std::remove(_entity_list->begin(), _entity_list->end(), nullptr), _entity_list->end());
}
void            BonusManager::bonusActivateCallback(void *p){
    std::pair<IGameEntity *, IGameEntity *> *pair = static_cast<std::pair<IGameEntity *, IGameEntity *> *>(p);
    pair->first->setState(State::DYING);
    switch(static_cast<Bonus *>(pair->first)->getBonusType()){
        case BonusType::BOMB_UP:   static_cast<Player *>(pair->second)->bombUp();     break;
        case BonusType::FLAME_UP:  static_cast<Player *>(pair->second)->flameUp();    break;
        case BonusType::SPEED_UP:  static_cast<Player *>(pair->second)->speedUp();    break;
    }
}
void            BonusManager::brickBreaksCallback(void *p){
    glm::vec2 *pos = static_cast<glm::vec2 *>(p);
    int r = (rand() % 100);
    std::cout << r << std::endl;
    Bonus   *b = nullptr;
    if (r < 60){
        if (r < 20)
            b = new Bonus(*pos, BonusType::SPEED_UP);
        else if (r < 40)
            b = new Bonus(*pos, BonusType::FLAME_UP);
        else
            b = new Bonus(*pos, BonusType::BOMB_UP);
    }
    if (b)
        _entity_list->push_back(b);
}