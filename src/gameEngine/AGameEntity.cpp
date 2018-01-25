/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AGameEntity.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/27 15:28:54 by egaborea          #+#    #+#             */
/*   Updated: 2018/01/25 12:31:40 by egaborea         ###   ########.fr       */
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
    return glm::normalize(_direction);
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

AGameEntity::AGameEntity(void) :
_position(glm::vec2(0., 0.)), _direction(glm::vec2(0., -1.)), _state(State::STANDING), _speed(0.f), _type(Type::PLAYER){
}

AGameEntity::AGameEntity(AGameEntity const &a) :
_position(a._position), _direction(a._direction), _state(a._state), _speed(a._speed), _type(a._type){
}
AGameEntity   &AGameEntity::operator=(AGameEntity const &rhs){
    _position = rhs._position;
    _direction = rhs._direction;
    _state = rhs._state;
    _speed = rhs._speed;
    _type = rhs._type;
    return *this;
}