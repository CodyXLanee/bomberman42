/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Map.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 17:05:57 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/23 17:17:56 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Map.hpp"

Map::Map() {}

Map::Map(glm::vec2 size) : _size(size)
{
}

Map::Map(Map const & src)
{
	*this = src;
}

Map::~Map() {}

Map &		Map::operator=(Map const & rhs)
{
	if (this != &rhs)
	{
		this->_size = rhs.getSize();
		this->_destroyableBlocs = rhs.getDestroyableBlocs();
		this->_undestroyableBlocs = rhs.getUndestroyableBlocs();
		this->_models = rhs.getModels();
	}

	return *this;
}

glm::vec2	Map::getSize() const
{
	return this->_size;
}

void		Map::setSize(glm::vec2 size)
{
	this->_size = size;
}

std::vector<glm::vec2>		Map::getDestroyableBlocs() const
{
	return this->_destroyableBlocs;
}

std::vector<glm::vec2>		Map::getUndestroyableBlocs() const
{
	return this->_undestroyableBlocs;
}

void						Map::addDestroyableBlocs(glm::vec2 pos)
{
	this->_destroyableBlocs.push_back(pos);
}

void						Map::removeDestroyableBlocs(glm::vec2 pos)
{
	for (unsigned long i = 0 ; i < this->_destroyableBlocs.size() ; i++)
	{
		if (this->_destroyableBlocs[i].x == pos.x && this->_destroyableBlocs[i].y == pos.y)
		{
			this->_destroyableBlocs.erase(this->_destroyableBlocs.begin()+i);
			return ;
		}
	}
}

void						Map::addUndestroyableBlocs(glm::vec2 pos)
{
	this->_undestroyableBlocs.push_back(pos);
}

std::map<Type::Enum, std::string>	Map::getModels() const
{
	return this->_models;
}
