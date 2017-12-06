/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/27 15:54:09 by egaborea          #+#    #+#             */
/*   Updated: 2017/12/06 12:03:22 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Player.hpp"

Player::Player(const glm::vec2 & pos) : 
AGameEntity(pos, glm::vec2(0., -1.), State::STANDING, 0., Type::PLAYER),
left(false), right(false), up(false), down(false),
_graphicalDirection(glm::vec2(0,0)){
	SEventManager & em = SEventManager::getInstance();
	em.registerEvent(Event::PLAYER_LEFT, MEMBER_CALLBACK(Player::newDirLeft));
	em.registerEvent(Event::PLAYER_RIGHT, MEMBER_CALLBACK(Player::newDirRight));
	em.registerEvent(Event::PLAYER_UP, MEMBER_CALLBACK(Player::newDirUp));
	em.registerEvent(Event::PLAYER_DOWN, MEMBER_CALLBACK(Player::newDirDown));
	em.registerEvent(Event::END_PLAYER_LEFT, MEMBER_CALLBACK(Player::endNewDirLeft));
	em.registerEvent(Event::END_PLAYER_RIGHT, MEMBER_CALLBACK(Player::endNewDirRight));
	em.registerEvent(Event::END_PLAYER_UP, MEMBER_CALLBACK(Player::endNewDirUp));
	em.registerEvent(Event::END_PLAYER_DOWN, MEMBER_CALLBACK(Player::endNewDirDown));
}

glm::vec2	Player::getGraphicalDirection() const
{
	return _graphicalDirection;
}

void		Player::setGraphicalDirection(glm::vec2 dir)
{
	_graphicalDirection = dir;
}

glm::vec2	Player::getNewDirection(void){
	glm::vec2	ret(0,0);
	if (left)
		ret.x -= 1.;
	if (right)
		ret.x += 1.;
	if (up)
		ret.y += 1.;
	if (down)
		ret.y -= 1.;
	return glm::normalize(ret);
}

void		Player::newDirLeft(void *){
	left = true;
}
void		Player::newDirRight(void *){
	right = true;
}
void		Player::newDirUp(void *){
	up = true;
}
void		Player::newDirDown(void *){
	down = true;
}

void		Player::endNewDirLeft(void *){
	left = false;
}
void		Player::endNewDirRight(void *){
	right = false;
}
void		Player::endNewDirUp(void *){
	up = false;
}
void		Player::endNewDirDown(void *){
	down = false;
}