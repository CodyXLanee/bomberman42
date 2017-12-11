/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 17:06:23 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/11 11:30:11 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_HPP
# define MAP_HPP

# include "rapidjson/document.h"
# include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
# include "DestructibleBloc.hpp"
# include "IndestructibleBloc.hpp"
# include "SEventManager.hpp"
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

		glm::vec2								getSize() const;
		void									setSize(glm::vec2 size);
		glm::vec3								getSunPos() const;
		void									setSunPos(glm::vec3 pos);
		glm::vec3								getSunColor() const;
		void									setSunColor(glm::vec3 color);
		const std::vector<DestructibleBloc>		getDestructibleBlocs() const;
		void									addDestructibleBlocs(DestructibleBloc bloc);
		bool									removeDestructibleBlocs(glm::vec2 pos);
		const std::vector<IndestructibleBloc>	getIndestructibleBlocs() const;
		void									addIndestructibleBlocs(IndestructibleBloc bloc);
		bool									hasBloc(glm::vec2 const &pos) const;
		bool									isInMap(glm::vec2 const &pos) const;
		std::map<Type::Enum, std::string>		getModels() const;


	private:
		glm::vec2							_size;
		glm::vec3							_sunPos;
		glm::vec3							_sunColor;
		std::vector<DestructibleBloc>		_destructibleBlocs;
		std::vector<IndestructibleBloc>		_indestructibleBlocs;
		std::map<Type::Enum, std::string>	_models;
};

#endif