/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WinSpot.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/05 18:22:55 by egaborea          #+#    #+#             */
/*   Updated: 2017/12/09 15:13:18 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "WinSpot.hpp"
# include "glm/ext.hpp"

WinSpot::WinSpot(const glm::vec2 & pos) : 
AGameEntity(pos, glm::vec2(0., -1.), State::STANDING, 0., Type::WIN_SPOT) {
    SEventManager::getInstance().registerEvent(Event::PLAYER_MOVE, MEMBER_CALLBACK(WinSpot::check_player_pos));
}

void    WinSpot::check_player_pos(void *p){
    glm::vec2 player_pos = static_cast<IGameEntity *>(p)->getPosition();
    if (glm::round(player_pos) == getPosition()){
        SEventManager::getInstance().raise(Event::GAME_FINISH, nullptr);
    }
}