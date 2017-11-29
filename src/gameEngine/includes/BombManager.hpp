/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BombManager.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/28 13:20:33 by egaborea          #+#    #+#             */
/*   Updated: 2017/11/29 14:30:11 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOMBMANAGER_HPP
# define BOMBMANAGER_HPP

# include "IGameEntity.hpp"
# include "bomberman.hpp"
# include "Bomb.hpp"
# include "Flame.hpp"
# include "Map.hpp"
# include <vector>
# include <array>
# include <iostream>

class BombManager {
	
	public:
		BombManager();
		~BombManager();

		void			update(Map & map, std::vector<IGameEntity *> &entityList, std::vector<Action::Enum> const actions);
	private:
		std::vector<IGameEntity *>	*explode(Map &map, Bomb const *bomb);
		std::vector<IGameEntity *>	*explodeOneDir(Map &map, int flames, glm::vec2 pos, glm::vec2 &dir);
		bool	_spawned_bomb;
};

#endif