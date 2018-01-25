/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WinManager.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/05 17:16:53 by egaborea          #+#    #+#             */
/*   Updated: 2018/01/25 12:25:51 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WINMANAGER_HPP
# define WINMANAGER_HPP

# include "IGameEntity.hpp"
# include "bomberman.hpp"
# include "Map.hpp"
# include "SEventManager.hpp"
# include "WinSpot.hpp"
# include "Player.hpp"
# include <glm/glm.hpp>

class WinManager {
	
	public:
		WinManager(WinCondition::Enum condition, GameMode::Enum game_mode, glm::vec2 spot);
		~WinManager();

		void			update(Map & map, std::vector<IGameEntity *> &entityList);

        WinManager(WinManager const &);
        WinManager   &operator=(WinManager const &rhs);
	private:
        void            win(void);
		WinManager();
        void             checkWinBlocks(Map &);
        void             checkWinEnemies(std::vector<IGameEntity *> &);
        void             checkWinPlayers(std::vector<IGameEntity *> &entityList);
        GameMode::Enum          _game_mode;
        WinCondition::Enum      _win_condition;
        glm::vec2               _win_spot;
        bool                    _won;
        
};

#endif