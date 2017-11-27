/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameEngine.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 16:11:53 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/27 03:47:40 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAMEENGINE_HPP
# define GAMEENGINE_HPP

# include "bomberman.hpp"
# include "IGameEntity.hpp"
# include "Map.hpp"
# include "Loader.hpp"
# include <glm/glm.hpp>
# include <vector>

class GameEngine {
	
	public:
		GameEngine();
		~GameEngine();
		
		void								compute(std::vector<Action::Enum> actions);
		Map const &							getMap() const;
		const std::vector<IGameEntity *>		getEntityList() const; // No need to mark IGameEntities as const

		glm::vec2	compute_direction(std::vector<Action::Enum> actions);
		void		compute_player(IGameEntity*p, std::vector<Action::Enum> actions);

	private:
		Map * 						_map;
		Loader						_loader;
		std::vector<IGameEntity *>	_entityList;
};

#endif