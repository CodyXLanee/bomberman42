/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameEngine.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 16:11:53 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/23 17:16:37 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAMEENGINE_HPP
# define GAMEENGINE_HPP

# include "bomberman.hpp"
# include "IGameEntity.hpp"
# include "Map.hpp"
# include <vector>

class GameEngine {
	
	public:
		GameEngine();
		~GameEngine();
		
		void								compute(std::vector<Action::Enum> actions);
		Map const &							getMap() const;
		std::vector<IGameEntity const *>		getEntityList() const;

	private:
		Map * map;
		std::vector<IGameEntity const *> entityList;
};

#endif