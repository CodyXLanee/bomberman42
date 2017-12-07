/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModelManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/06 11:22:44 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/07 13:21:20 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ModelManager.hpp"

ModelManager::ModelManager() {
	directory = "assets/models/obj/";
	groundModel = new Model(directory + "groundTile1.obj", false);
	wallModel = new Model(directory + "wall.obj", false);
	playerModel = new Model(directory + "player.obj", false);
	brickModel = new Model(directory + "brick.obj", false);
	bombModel = new Model(directory + "bomb.obj", false);
	flameModel = new Model(directory + "flame.obj", false);
	aiDebug = new Model(directory + "aiDebug.obj", false);
}

ModelManager::~ModelManager() {
	delete groundModel;
	delete wallModel;
	delete playerModel;
	delete brickModel;
	delete bombModel;
	delete flameModel;
	delete aiDebug;
}

Model	&ModelManager::getModel(enum model m) const {
	switch(m) {
		case PLAYER: return *playerModel; break;
		case GROUND: return *groundModel; break;
		case WALL: return *wallModel; break;
		case BRICK: return *brickModel; break;
		case BOMB: return *bombModel; break;
		case FLAME: return *flameModel; break;
		case AIDEBUG: return *aiDebug; break;
	}
}