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

Map::Map() : _mapFile(""), _map(NULL) {}

Map::Map(std::string mapFile) : _mapFile(mapFile), _map(NULL)
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
		this->_mapFile = rhs.getFile();
	}

	return *this;
}

void		Map::setFile(std::string mapFile)
{
	this->_mapFile = mapFile;
}

std::string	Map::getFile() const
{
	return this->_mapFile;
}

GameEntity ***	Map::getMap() const
{
	return this->_map;
}

std::vector<GameEntity*>	Map::getVEntity() const
{
	return this->_VEntity;
}

int			Map::load()
{
    std::string json;
    rapidjson::Document d;
	std::ifstream infile;
	char * writable;

	// leave if no mapFile
	if (!this->_mapFile.compare(""))
		return -1;

    // read the file
	infile.open(this->_mapFile);
	if (!infile.is_open())
		return -1;
	getline(infile, json);
	infile.close();

	// parse the json
	writable = new char[json.size() + 1];
	std::copy(json.begin(), json.end(), writable);
	writable[json.size()] = '\0';
    d.Parse(writable);
    if (!d.IsObject() || !d.HasMember("grid"))
    	return -1;
    const rapidjson::Value& grid = d["grid"];
    if (!grid.IsArray())
    	return -1;

    // build the map and the gameEntity vector
    this->_map = new GameEntity**[grid.Size()];
    for (unsigned int i = 0 ; i < grid.Size() ; i++)
    {
    	if (!grid[i].IsArray())
    		return -1;
    	this->_map[i] = new GameEntity*[grid[i].Size()];
    	for (unsigned int j = 0 ; j < grid[i].Size() ; j ++)
    	{
    		switch (grid[i][j].GetInt())
    		{
    		case 0: // case vide
    			this->_map[i][j] = NULL;
    			break;
    		case 1: // player set
    			glm::vec2	vec(static_cast<float>(j), static_cast<float>(i));
    			this->_map[i][j] = NULL;
    			this->_VEntity.push_back(new GameEntity(Type::PLAYER));
    			this->_VEntity.back()->setPosition(vec);
    			break;
    		}
    	}
    }

    return 1;
}