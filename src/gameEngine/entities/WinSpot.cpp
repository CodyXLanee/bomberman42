/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WinSpot.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/05 18:22:55 by egaborea          #+#    #+#             */
/*   Updated: 2018/01/03 15:55:09 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "WinSpot.hpp"
# include "glm/ext.hpp"

WinSpot::WinSpot(const glm::vec2 & pos) : 
AGameEntity(pos, glm::vec2(0., -1.), State::STANDING, 0., Type::WIN_SPOT) {
    SEventManager::getInstance().registerEvent(Event::PLAYER_MOVE, MEMBER_CALLBACK(WinSpot::check_player_pos));
}

WinSpot::~WinSpot(){
    SEventManager::getInstance().unRegisterEvent(Event::PLAYER_MOVE, this);
}

void    WinSpot::check_player_pos(void *p){
    glm::vec2 player_pos = static_cast<IGameEntity *>(p)->getPosition();
    if (glm::round(player_pos) == getPosition()){
        SEventManager::getInstance().raise(Event::GAME_WIN, nullptr);
    }
}