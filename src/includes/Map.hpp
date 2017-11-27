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
# include <map>
# include <glm/gtc/matrix_transform.hpp>

class Map {
	public:
		Map();
		Map(glm::vec2 size);
		Map(Map const & src);

		~Map();

		Map &						operator=(Map const & rhs);

		glm::vec2					getSize() const;
		void						setSize(glm::vec2 size);
		std::vector<glm::vec2>		getDestroyableBlocs() const;
		void						addDestroyableBlocs(glm::vec2 pos);
		void						removeDestroyableBlocs(glm::vec2 pos);
		std::vector<glm::vec2>		getUndestroyableBlocs() const;
		void						addUndestroyableBlocs(glm::vec2 pos);
		std::map<Type::Enum, std::string>	getModels() const;


	private:
		glm::vec2					_size;
		std::vector<glm::vec2>		_destroyableBlocs;
		std::vector<glm::vec2>		_undestroyableBlocs;
		std::map<Type::Enum, std::string>	_models;
};

#endif