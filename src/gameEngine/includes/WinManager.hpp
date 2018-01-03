/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WinManager.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/05 17:16:53 by egaborea          #+#    #+#             */
/*   Updated: 2018/01/03 14:28:45 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WINMANAGER_HPP
# define WINMANAGER_HPP

# include "IGameEntity.hpp"
# include "bomberman.hpp"
# include "Map.hpp"
# include "SEventManager.hpp"
# include "WinSpot.hpp"
# include <glm/glm.hpp>

class WinManager {
	
	public:
		WinManager(WinCondition::Enum condition, glm::vec2 spot);
		~WinManager();

		void			update(Map & map, std::vector<IGameEntity *> &entityList);
	private:
        void            win(void);
		WinManager();
        void             checkWinBlocks(Map &);
        void             checkWinEnemies(std::vector<IGameEntity *> &);
        WinCondition::Enum      _win_condition;
        glm::vec2               _win_spot;
        bool                    _won;
        
};

#endif