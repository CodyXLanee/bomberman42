/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BombManager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/28 13:24:20 by egaborea          #+#    #+#             */
/*   Updated: 2017/12/06 22:40:47 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BombManager.hpp"
#include "glm/ext.hpp"

BombManager::BombManager(Map *map, std::vector<IGameEntity *> *entityList) : 
_map(map), _entityList(entityList), _flames_to_add(new std::vector<IGameEntity *>()) {
    SEventManager &em = SEventManager::getInstance();
    em.registerEvent(Event::SPAWN_BOMB, MEMBER_CALLBACK(BombManager::spawn_bomb));
    em.registerEvent(Event::SPAWN_FLAME, MEMBER_CALLBACK(BombManager::spawn_flame));
    em.registerEvent(Event::BOMB_EXPLODES, MEMBER_CALLBACK(BombManager::bomb_explodes));
    em.registerEvent(Event::FLAME_DISAPEAR, MEMBER_CALLBACK(BombManager::flames_disapear));
}

BombManager::~BombManager() {}

void              BombManager::spawn_bomb(void *p){
    Player *player = static_cast<Player *>(p);
    for (auto it = _entityList->begin(); it != _entityList->end(); it++){
        if (glm::round((*it)->getPosition()) == glm::round(player->getPosition()) && (*it)->getType() != Type::PLAYER)
            return ;

    }
    if (player->getBombCount() >= player->getMaxBombNb())
        return;
    player->addBombToCount();
    _entityList->push_back(new Bomb(
        glm::round(player->getPosition()), static_cast<Player *>(p)
        ));
}

// Creates flames and stuffs
void              BombManager::bomb_explodes(void *p){
    std::vector<IGameEntity *>	*new_flames = explode(static_cast<Bomb *>(p));
    for (auto it = new_flames->begin(); it != new_flames->end(); it++){
        SEventManager::getInstance().raise(Event::SPAWN_FLAME, *it);
    }
    delete new_flames;
}

void              BombManager::spawn_flame(void *p){
    _flames_to_add->push_back(static_cast<Flame *>(p));
}

// Make flames disapear
void              BombManager::flames_disapear(void *){
}


std::vector<IGameEntity *>	*BombManager::explodeOneDir(int flames, glm::vec2 pos, glm::vec2 &dir){
    std::vector<IGameEntity *>	*ret = new std::vector<IGameEntity *>();

    if (_map->hasBloc(pos)){
        if (_map->removeDestructibleBlocs(pos)){
            ret->push_back(new Flame(pos));
        }
    }
    else {
        ret->push_back(new Flame(pos));
        if (flames > 0){
            std::vector<IGameEntity *>	*tmp = explodeOneDir(--flames, pos + dir, dir);
            if (tmp->size() > 0)
                ret->insert(ret->end(), tmp->begin(), tmp->end());
            delete tmp;
        }
    }
    return ret;
}

std::vector<IGameEntity *>	*BombManager::explode(Bomb const *bomb){
    std::vector<IGameEntity *>	*ret = new std::vector<IGameEntity *>();
    std::vector<IGameEntity *>	*tmp = NULL;

    ret->push_back(new Flame(bomb->getPosition()));
    // Four directions : Right, Up, Left, Down
    std::array<glm::vec2, 4> dir = {{ glm::vec2(1, 0), glm::vec2(0, 1), glm::vec2(-1, 0), glm::vec2(0, -1) }};

    // Looping over directions
    for (auto i = dir.begin(); i != dir.end(); i++){
        tmp = explodeOneDir(bomb->getFlameNb() - 1, bomb->getPosition() + *i, *i);
        if (tmp->size() > 0)
            ret->insert(ret->end(), tmp->begin(), tmp->end());
        delete tmp;
    }
    return ret;
}


void    BombManager::update(void){
    for (auto i = _entityList->begin(); i != _entityList->end(); i++){
        if ((*i)->getType() == Type::BOMB || (*i)->getType() == Type::FLAME)
            (*i)->update();
    }
    // Erase Bomb/Flames that needs to disapear
    _entityList->erase(
        std::remove_if(_entityList->begin(), _entityList->end(),
            [](const IGameEntity * e) {
                return e->getState() == State::DYING;
            }),
        _entityList->end());
    if (_flames_to_add->size() > 0){
        _entityList->insert(_entityList->end(), _flames_to_add->begin(), _flames_to_add->end());
        _flames_to_add->clear();
    }
}