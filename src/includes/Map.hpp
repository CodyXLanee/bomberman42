/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 17:06:23 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/23 17:07:33 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_HPP
# define MAP_HPP

# include "rapidjson/document.h"
# include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
# include "gameEngine/includes/GameEntity.hpp"
# include "includes/IGameEntity.hpp"
# include <iostream>
# include <fstream>
# include <vector>
# include <glm/gtc/matrix_transform.hpp>

class Map {
	public:
		Map();
		Map(std::string mapFile);
		Map(Map const & src);

		~Map();

		Map &		operator=(Map const & rhs);

		void						setFile(std::string mapFile);
		std::string					getFile() const;
		GameEntity ***				getMap() const;
		std::vector<GameEntity*>	getVEntity() const;

		int			load();

	private:
		std::string					_mapFile;
		GameEntity					***_map;
		std::vector<GameEntity*>	_VEntity;
};

#endif