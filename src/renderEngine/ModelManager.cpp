/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModelManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/06 11:22:44 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/22 09:57:28 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ModelManager.hpp"

ModelManager::ModelManager() {
	directory = "assets/models/obj/";

	int i = 0;
	while(i < 4) {
		playerModel[i] = new Model(directory + "player.fbx");
		i++;
	}

	wallModel = new Model(directory + "wall.obj");
	groundModel = new Model(directory + "groundTile1.obj");
	brickModel = new Model(directory + "brick.obj");
	sceneryModel = new Model(directory + "sceneryGround.obj");

	flameModel = new Model(directory + "flame.obj");
	bombModel = new Model(directory + "bomb.obj");

	flameUpModel = new Model(directory + "flameUp.obj");
	bombUpModel = new Model(directory + "bombUp.obj");
	speedUpModel = new Model(directory + "speedUp.obj");

	// baloonModel = new Model(directory + "baloon.obj");
	baloonModel = new Model(directory + "mob.fbx");
	
	aiDebug = new Model(directory + "aiDebug.obj");
}

ModelManager::~ModelManager() {
	delete groundModel;
	delete wallModel;
	delete brickModel;
	delete bombModel;
	delete flameModel;
	delete aiDebug;
	delete sceneryModel;
	for (unsigned int i = 0; i < 4; i++) {
		delete playerModel[i];
	}
}

Model	&ModelManager::getPlayerModel(unsigned int n) const {
	if (n > 3)
		n = 3;
	return *playerModel[n];
}

Model	&ModelManager::getModel(enum model m) const {
	switch(m) {
		case PLAYER: return *playerModel[0]; break;
		case GROUND: return *groundModel; break;
		case WALL: return *wallModel; break;
		case BRICK: return *brickModel; break;
		case BOMB: return *bombModel; break;
		case FLAME: return *flameModel; break;
		case AIDEBUG: return *aiDebug; break;
		case FLAME_UP:	return *flameUpModel; break;
		case BOMB_UP:	return *bombUpModel; break;
		case SPEED_UP:	return *speedUpModel; break;
		case BALOON:	return *baloonModel; break;
		case SCENERY: return *sceneryModel; break;
	}
}