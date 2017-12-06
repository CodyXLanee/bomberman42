/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BombManager.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/28 13:20:33 by egaborea          #+#    #+#             */
/*   Updated: 2017/12/06 19:40:40 by egaborea         ###   ########.fr       */
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
		BombManager(Map *map, std::vector<IGameEntity *> *entityList);
		~BombManager();

		void			update(void);
	private:
		void                        spawn_bomb(void *p);
		void                        bomb_explodes(void *p);
		void                        spawn_flame(void *p);
		void                        flames_disapear(void *);
		std::vector<IGameEntity *>	*explode(Bomb const *bomb);
		std::vector<IGameEntity *>	*explodeOneDir(int flames, glm::vec2 pos, glm::vec2 &dir);
		Map 						*_map; 
		std::vector<IGameEntity *> 	*_entityList;
		std::vector<IGameEntity *>	*_flames_to_add;
};

#endif