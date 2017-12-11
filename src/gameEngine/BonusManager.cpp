/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BonusManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/11 10:59:46 by egaborea          #+#    #+#             */
/*   Updated: 2017/12/11 16:52:14 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BonusManager.hpp"

BonusManager::BonusManager(std::vector<IGameEntity *> *entityList) :
_entity_list(entityList),
_bonus_queue(new std::vector<Bonus *>()){
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
    for (auto i : *_bonus_queue){
        std::cout << "emptying queue" << std::endl;
        _entity_list->push_back(i);
    }
    _bonus_queue->clear();
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
    Bonus   *b = nullptr;
    if (r < 60){
        if (r < 20)
            b = new Bonus(*pos, BonusType::SPEED_UP);
        else if (r < 40)
            b = new Bonus(*pos, BonusType::FLAME_UP);
        else
            b = new Bonus(*pos, BonusType::BOMB_UP);
    }
    if (b){
        std::cout << "pushing bonus to queue" << std::endl;
        _bonus_queue->push_back(b);
    }
}
