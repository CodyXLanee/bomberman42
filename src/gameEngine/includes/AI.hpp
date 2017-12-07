/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AI.hpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/07 11:28:49 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/07 13:47:34 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AI_HPP
# define AI_HPP

# include "Map.hpp"
# include "IGameEntity.hpp"
# include "SEventManager.hpp"
# include <vector>
# include <glm/vec2.hpp>
# include <iostream>

class AI {
    public:
        AI(IGameEntity &player);
        ~AI();

        void    compute(Map const & map, std::vector<IGameEntity *> & entities);
    private:

        AI();
        void    updateReachableMap();
        void    getFreePlaces(Map const & map);
        void    getReachablePlaces();

        IGameEntity &player;
        std::vector<glm::vec2>  freePlaces;
        std::vector<glm::vec2>  reachablePlaces;
};

#endif