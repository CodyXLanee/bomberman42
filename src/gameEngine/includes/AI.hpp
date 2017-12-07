/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AI.hpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/07 11:28:49 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/07 18:07:48 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AI_HPP
# define AI_HPP

# include "Map.hpp"
# include "IGameEntity.hpp"
# include "SEventManager.hpp"
# include "Player.hpp"
# include <vector>
# include <glm/vec2.hpp>
# include <iostream>
# include <map>

class Spot;

namespace std
{
    template<> struct less<glm::ivec2>
    {
       bool operator() (const glm::vec2& lhs, const glm::vec2& rhs) const
       {
           return lhs.x == rhs.x ? lhs.y < rhs.y : lhs.x < rhs.x;
       }
    };
}

class AI {
    public:
        AI(Player *player);
        ~AI();

        void    compute(Map const & map, std::vector<IGameEntity *> & entities);
    private:

        AI();
        bool    can_place_bomb(void);
        bool    is_safe(glm::ivec2 pos, std::vector<IGameEntity *> & entities);
        void    updateMapDistRec(glm::ivec2 pos, int rec);
        void    updateObjective(void);
        void    updateMap(Map const & map, std::vector<IGameEntity *> & entities);
        void    run_to_safety(Map const & map, std::vector<IGameEntity *> & entities);
        bool    shouldAppearInDebug(glm::ivec2 pos);
        void    updateDebugCubes(Map const & map, std::vector<IGameEntity *> & entities) ;

        Player                          *_player;
        std::map<glm::ivec2, Spot>           _map;
        std::vector<glm::vec2>              *_debug_cubes;
        glm::ivec2                          _objective;
};

#endif