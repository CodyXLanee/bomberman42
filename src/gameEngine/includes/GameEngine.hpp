/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameEngine.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 16:11:53 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/30 11:12:49 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAMEENGINE_HPP
# define GAMEENGINE_HPP

# include "bomberman.hpp"
# include "IGameEntity.hpp"
# include "CollisionsManager.hpp"
# include "BombManager.hpp"
# include "Map.hpp"
# include "Loader.hpp"
# include <glm/glm.hpp>
# include <vector>
# include <math.h>

class GameEngine {
	
	public:
		GameEngine();
		~GameEngine();
		
		void								compute(std::vector<Action::Enum> &actions);
		Map const &							getMap() const;
		std::vector<IGameEntity *> &		getEntityList();
		glm::vec2 const 					*getPlayerPos(void) const;

	private:
		Map * 						_map;
		Loader						_loader;
		std::vector<IGameEntity *>	_entityList;
		CollisionsManager			_collisionsManager;
		BombManager					_bombManager;

};

#endif