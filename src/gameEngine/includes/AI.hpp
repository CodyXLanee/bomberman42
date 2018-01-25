/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AI.hpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/07 11:28:49 by tpierron          #+#    #+#             */
/*   Updated: 2018/01/25 12:25:33 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AI_HPP
# define AI_HPP

# include "Map.hpp"
# include "IGameEntity.hpp"
# include "SEventManager.hpp"
# include "Player.hpp"
# include <algorithm>
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
        Player  *getPlayer(void);

        AI(AI const &);
        AI   &operator=(AI const &rhs);
    private:

        AI();

        static Event::Enum   endEvents(Event::Enum e);
        static glm::vec2     dirToVec(Event::Enum e);
        static glm::ivec2     dirToIVec(Event::Enum e);
        static Event::Enum    vecToDir(glm::vec2 v);
        static std::vector<Event::Enum>  vecToDirs(glm::vec2 v);
        
        bool    would_be_blocked_by_bomb(void);
        bool    can_place_bomb(void);
        
        void    resetMapDist(void);
        void    updateMapDistRec(glm::ivec2 pos, int rec);
        void    SAFEupdateMapDistRec(glm::ivec2 pos, int rec);
        
        bool    lookForBonus(glm::ivec2 *obj);
        bool    aimClosestSafeSpace(glm::ivec2 *obj);
        bool    aimFarthestSafeSpace(glm::ivec2 *obj);
        
        void    markBombRangeOneDir(glm::ivec2 pos, glm::ivec2 dir, int range, Bomb *bomb);
        void    markBombRange(Bomb *bomb, Bomb *in_range_of);
        void    markDeadlySpots(void);
        
        void    updateMapWithEntity(IGameEntity *entity);
        
        void    updateMap(Map const & map, std::vector<IGameEntity *> & entities);
        bool    shouldAppearInDebug(glm::ivec2 pos);
        void    updateDebugCubes(Map const & map, std::vector<IGameEntity *> & entities) ;

        void    runToObjective();

        Player                              *_player;
        std::map<glm::ivec2, Spot>          _map;
        std::vector<glm::vec2>              *_debug_cubes;
        glm::ivec2                          _objective;
        std::vector<Event::Enum>            _last_dir;
        bool                                _went_far;
        bool                                _going_safely;
        
};

#endif