/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AGameEntity.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/27 15:28:54 by egaborea          #+#    #+#             */
/*   Updated: 2017/11/28 14:04:20 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AGameEntity.hpp"

AGameEntity::~AGameEntity(){
}


AGameEntity::AGameEntity(const glm::vec2 &pos, const glm::vec2 &dir, const State::Enum &state, const float &speed, const Type::Enum &type) :
_position(pos), _direction(dir), _state(state), _speed(speed), _type(type){

}

glm::vec2	    AGameEntity::getPosition() const {
    return _position;
}

glm::vec2	AGameEntity::getDirection() const {
    return _direction;
}

State::Enum	    AGameEntity::getState() const {
    return _state;
}

float		    AGameEntity::getSpeed() const {
    return _speed;
}

Type::Enum	    AGameEntity::getType() const {
    return _type;
}


void	AGameEntity::setPosition(const glm::vec2 &position){
    _position = position;
}

void	AGameEntity::setDirection(const glm::vec2 &direction){
    _direction = direction;
}

void	AGameEntity::setState(const State::Enum & state){
    _state = state;
}

void	AGameEntity::setSpeed(const float & speed){
    _speed = speed;
}

void        AGameEntity::update(void){
}