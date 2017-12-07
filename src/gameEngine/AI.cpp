/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AI.cpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/07 11:27:49 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/07 14:40:50 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AI.hpp"

AI::AI(IGameEntity &player) : player(player) {
    SEventManager & em = SEventManager::getInstance();
    em.raise(Event::AIPTR, &freePlaces);
}  

AI::~AI() {

}

void    AI::compute(Map const & map, std::vector<IGameEntity *> & entities) {
    (void)entities; /////////////////
    getFreePlaces(map);
}

void    AI::getFreePlaces(Map const & map) {
    glm::vec2 mapSize = map.getSize();
    freePlaces.clear();

    for(unsigned int i = 0; i < mapSize.x; i++) {
        for(unsigned int j = 0; j < mapSize.y; j++) {
            glm::vec2 place = glm::vec2(i, j);
            if (!map.hasBloc(place))
                freePlaces.push_back(place);
        }
    }
}