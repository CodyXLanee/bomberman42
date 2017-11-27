/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameEntity.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 15:01:13 by egaborea          #+#    #+#             */
/*   Updated: 2017/11/27 03:38:22 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GameEntity.hpp"

GameEntity::GameEntity(const Type::Enum & type) : 
        _position(glm::vec2(0., 0.)), _direction(glm::vec2(0., -1.)), _state(State::STANDING), _speed(0.), _type(type){
}

GameEntity::GameEntity(const Type::Enum & type, const glm::vec2 & pos) : 
_position(glm::vec2(pos)), _direction(glm::vec2(0., -1.)), _state(State::STANDING), _speed(0.), _type(type){
}

GameEntity::~GameEntity(){
}

glm::vec2	    GameEntity::getPosition() const {
    return _position;
}

glm::vec2	GameEntity::getDirection() const {
    return _direction;
}

State::Enum	    GameEntity::getState() const {
    return _state;
}

float		    GameEntity::getSpeed() const {
    return _speed;
}

Type::Enum	    GameEntity::getType() const {
    return _type;
}


void	GameEntity::setPosition(const glm::vec2 &position){
    _position = position;
}

void	GameEntity::setDirection(const glm::vec2 &direction){
    _direction = direction;
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