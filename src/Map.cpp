/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Map.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 17:05:57 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/29 21:42:50 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Map.hpp"
#include "glm/ext.hpp"

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
		this->_destructibleBlocs = rhs.getDestructibleBlocs();
		this->_indestructibleBlocs = rhs.getIndestructibleBlocs();
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

glm::vec3	Map::getSunPos() const
{
	return this->_sunPos;
}

void		Map::setSunPos(glm::vec3 pos)
{
	this->_sunPos = pos;
}

glm::vec3	Map::getSunColor() const
{
	return this->_sunColor;
}

void		Map::setSunColor(glm::vec3 color)
{
	this->_sunColor = color;
}

const std::vector<DestructibleBloc>		Map::getDestructibleBlocs() const
{
	return this->_destructibleBlocs;
}

const std::vector<IndestructibleBloc>		Map::getIndestructibleBlocs() const
{
	return this->_indestructibleBlocs;
}

void						Map::addDestructibleBlocs(DestructibleBloc bloc)
{
	this->_destructibleBlocs.push_back(bloc);
}

bool						Map::removeDestructibleBlocs(glm::vec2 pos)
{
	for (unsigned long i = 0 ; i < this->_destructibleBlocs.size() ; i++)
	{
		if (this->_destructibleBlocs[i].getPosition() == pos)
		{
			this->_destructibleBlocs.erase(this->_destructibleBlocs.begin()+i);
			return true;
		}
	}
	return false;
}

void						Map::addIndestructibleBlocs(IndestructibleBloc bloc)
{
	this->_indestructibleBlocs.push_back(bloc);
}

bool						Map::isInMap(glm::vec2 const &pos) const {
	return (pos.x >= 0. && pos.y >= 0. && pos.x < _size.x && pos.y < _size.y);
}

bool						Map::hasBloc(glm::vec2 const &pos) const
{
	for (unsigned long i = 0 ; i < this->_destructibleBlocs.size() ; i++)
		if (this->_destructibleBlocs[i].getPosition() == pos)
			return true;
	for (unsigned long i = 0 ; i < this->_indestructibleBlocs.size() ; i++)
		if (this->_indestructibleBlocs[i].getPosition() == pos)
			return true;
	return !isInMap(pos);
}

std::map<Type::Enum, std::string>	Map::getModels() const
{
	return this->_models;
}
