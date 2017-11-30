/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BombManager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/28 13:24:20 by egaborea          #+#    #+#             */
/*   Updated: 2017/11/30 11:14:15 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BombManager.hpp"
#include "glm/ext.hpp"

BombManager::BombManager() : _spawned_bomb(false) {}

BombManager::~BombManager() {}

std::vector<IGameEntity *>	*BombManager::explodeOneDir(Map &map, int flames, glm::vec2 pos, glm::vec2 &dir){
    std::vector<IGameEntity *>	*ret = new std::vector<IGameEntity *>();

    if (map.hasBloc(pos)){
        if (map.removeDestructibleBlocs(pos)){
            ret->push_back(new Flame(pos));
        }
    }
    else {
        ret->push_back(new Flame(pos));
        if (flames > 0){
            std::vector<IGameEntity *>	*tmp = explodeOneDir(map, --flames, pos + dir, dir);
            if (tmp->size() > 0)
                ret->insert(ret->end(), tmp->begin(), tmp->end());
            delete tmp;
        }
    }
    return ret;
}

std::vector<IGameEntity *>	*BombManager::explode(Map &map, Bomb const *bomb, std::vector<Action::Enum> &actions){
    std::vector<IGameEntity *>	*ret = new std::vector<IGameEntity *>();
    std::vector<IGameEntity *>	*tmp = NULL;

    if (find(actions.begin(), actions.end(), Action::BOMB_EXPLODES) == actions.end())
        actions.push_back(Action::BOMB_EXPLODES);

    ret->push_back(new Flame(bomb->getPosition()));
    // Four directions : Right, Up, Left, Down
    std::array<glm::vec2, 4> dir = {{ glm::vec2(1, 0), glm::vec2(0, 1), glm::vec2(-1, 0), glm::vec2(0, -1) }};

    // Looping over directions
    for (auto i = dir.begin(); i != dir.end(); i++){
        tmp = explodeOneDir(map, bomb->getFlameNb() - 1, bomb->getPosition() + *i, *i);
        if (tmp->size() > 0)
            ret->insert(ret->end(), tmp->begin(), tmp->end());
        delete tmp;
    }
    return ret;
}


void    BombManager::update(Map &map, std::vector<IGameEntity *> & entityList, std::vector<Action::Enum> &actions){
    Bomb                        *new_bomb = NULL;
    std::vector<IGameEntity *>  *new_flames = new std::vector<IGameEntity *>();
    if (_spawned_bomb && find(actions.begin(), actions.end(), Action::SPAWN_BOMB) == actions.end()){
        _spawned_bomb = false;
    }
    for (auto i = entityList.begin(); i != entityList.end(); i++){
        switch ((*i)->getType()) {            // Spawn New Bomb
            case Type::PLAYER:
                if (find(actions.begin(), actions.end(), Action::SPAWN_BOMB) != actions.end() && _spawned_bomb == false){
                    new_bomb = new Bomb(glm::round((*i)->getPosition()));
                    _spawned_bomb = true;
                }
                break;
            // Explode Bombs
            case Type::BOMB:
                (*i)->update();
                if ((*i)->getState() == State::DYING){
                    std::vector<IGameEntity *>  *tmp = explode(map, static_cast<Bomb *>(*i), actions);
                    if (tmp->size() > 0)
                        new_flames->insert(new_flames->end(), tmp->begin(), tmp->end());
                    delete tmp;
                }
                break;
            case Type::FLAME:
                (*i)->update();
                break;
            default:
                break;
        }
    }



    // Bomb explosion make bomb explode
    
    size_t  flame_count = 0;
    std::vector<IGameEntity *>  *sec_new_flames;
    
    while (flame_count != new_flames->size()) {
        flame_count = new_flames->size();
        sec_new_flames = new std::vector<IGameEntity *>();

        for (auto i = new_flames->begin(); i != new_flames->end(); i++) {
            for (auto j = entityList.begin(); j != entityList.end(); j++){
                switch((*j)->getType()){
                    case Type::PLAYER:
                        // WE CHECK IF HE DIES HERE
                            // HE DIES HERE
                        break;
                    case Type::BOMB:
                        if ((*j)->getPosition() == (*i)->getPosition() && (*j)->getState() != State::DYING){
                            (*j)->setState(State::DYING);
                            std::vector<IGameEntity *>  *tmp = explode(map, static_cast<Bomb *>(*j), actions);
                            if (tmp->size() > 0)
                                sec_new_flames->insert(sec_new_flames->end(), tmp->begin(), tmp->end());
                            delete tmp;
                        }
                        break;
                    default:
                        break;
                }
            }
        }
        if (sec_new_flames->size() != 0)
            new_flames->insert(new_flames->end(), sec_new_flames->begin(), sec_new_flames->end());
        delete sec_new_flames;
    }

    if (new_flames->size() > 0)
        entityList.insert(entityList.end(), new_flames->begin(), new_flames->end());
    delete new_flames;
    // Cancels Bomb spawning if a bomb is already here
    for (auto i = entityList.begin(); i != entityList.end(); i++){
        if (new_bomb && (*i)->getType() && (*i)->getPosition() == new_bomb->getPosition()){
            delete new_bomb;
            new_bomb = NULL;
        }
    }
    if (new_bomb){
        entityList.push_back(new_bomb);
    }
    // Erase Bomb/Flames that needs to disapear
    entityList.erase(
        std::remove_if(entityList.begin(), entityList.end(),
            [](const IGameEntity * e) {
                return e->getState() == State::DYING;
            }),
        entityList.end());
}