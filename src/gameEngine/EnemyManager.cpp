/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EnemyManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/14 19:20:30 by egaborea          #+#    #+#             */
/*   Updated: 2017/12/14 19:26:48 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "EnemyManager.hpp"

EnemyManager::EnemyManager(std::vector<IGameEntity *> *entityList) : _entity_list(entityList){
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