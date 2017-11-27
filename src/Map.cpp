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

std::vector<DestructibleBloc>		Map::getDestructibleBlocs() const
{
	return this->_destructibleBlocs;
}

std::vector<IndestructibleBloc>		Map::getIndestructibleBlocs() const
{
	return this->_indestructibleBlocs;
}

void						Map::addDestructibleBlocs(DestructibleBloc bloc)
{
	this->_destructibleBlocs.push_back(bloc);
}

void						Map::removeDestructibleBlocs(glm::vec2 pos)
{
	for (unsigned long i = 0 ; i < this->_destructibleBlocs.size() ; i++)
	{
		if (this->_destructibleBlocs[i].getPosition() == pos)
		{
			this->_destructibleBlocs.erase(this->_destructibleBlocs.begin()+i);
			return ;
		}
	}
}

void						Map::addIndestructibleBlocs(IndestructibleBloc bloc)
{
	this->_indestructibleBlocs.push_back(bloc);
}

int							Map::haveBloc(glm::vec2 pos) const
{
	for (unsigned long i = 0 ; i < this->_destructibleBlocs.size() ; i++)
		if (this->_destructibleBlocs[i].getPosition() == pos)
			return 1;
	for (unsigned long i = 0 ; i < this->_indestructibleBlocs.size() ; i++)
		if (this->_indestructibleBlocs[i].getPosition() == pos)
			return 1;
	return 0;
}

std::map<Type::Enum, std::string>	Map::getModels() const
{
	return this->_models;
}
