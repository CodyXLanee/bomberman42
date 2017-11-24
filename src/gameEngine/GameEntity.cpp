/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameEntity.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 15:01:13 by egaborea          #+#    #+#             */
/*   Updated: 2017/11/24 15:33:05 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GameEntity.hpp"

GameEntity::GameEntity(const Type::Enum & type) : 
        _position(glm::vec2(0., 0.)), _state(State::STAND), _speed(0.), _type(type){
}

GameEntity::GameEntity(const Type::Enum & type, const glm::vec2 & pos) : 
_position(glm::vec2(pos)), _state(State::STAND), _speed(0.), _type(type){
}

GameEntity::~GameEntity(){
}

glm::vec2	    GameEntity::getPositon(){
    return _position;
}

State::Enum	    GameEntity::getState(){
    return _state;
}

float		    GameEntity::getSpeed(){
    return _speed;
}

Type::Enum	    GameEntity::getType(){
    return _type;
}


void	GameEntity::setPositon(const glm::vec2 &position){
    _position = position;
}

void	GameEntity::setState(const State::Enum & state){
    _state = state;
}

void	GameEntity::setSpeed(const float & speed){
    _speed = speed;
}

void	GameEntity::setType(const Type::Enum & type){
    _type = type;
}