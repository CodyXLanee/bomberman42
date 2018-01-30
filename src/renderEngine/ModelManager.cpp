/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModelManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/06 11:22:44 by tpierron          #+#    #+#             */
/*   Updated: 2018/01/30 15:25:47 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ModelManager.hpp"

ModelManager::ModelManager() {
	directory = "assets/models/obj/";

	playerModel[0] = new Model(directory + "playerWhite.fbx");
	playerModel[1] = new Model(directory + "playerBlack.fbx");
	playerModel[2] = new Model(directory + "playerRed.fbx");
	playerModel[3] = new Model(directory + "playerYellow.fbx");
	
	setThemeLava();
}

ModelManager::~ModelManager() {
	delete groundModel;
	delete wallModel;
	delete brickModel;
	delete bombModel;
	delete flameModel;
	delete aiDebug;
	delete sceneryModel;
	delete winSpot;
	delete flameUpModel;
	delete bombUpModel;
	delete speedUpModel;
	delete baloonModel;
	for (unsigned int i = 0; i < 4; i++) {
		delete playerModel[i];
	}
}

ModelManager::ModelManager(ModelManager const & src) {
	*this = src;
}

ModelManager &	ModelManager::operator=(ModelManager const &) {
	return *this;
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
		case WIN_SPOT: return *winSpot; break;
	}
}

void	ModelManager::setMapTheme(void *p) {
	delete groundModel;
	delete wallModel;
	delete brickModel;
	delete bombModel;
	delete flameModel;
	delete aiDebug;
	delete sceneryModel;
	delete winSpot;
	delete flameUpModel;
	delete bombUpModel;
	delete speedUpModel;
	delete baloonModel;
	Theme::Enum t = *static_cast<Theme::Enum * >(p);
	switch (t) {
		case Theme::LAVA: setThemeLava(); break;
		case Theme::FOREST: setThemeForest(); break;
	}
}

void	ModelManager::setThemeLava() {
	wallModel = new Model(directory + "wall.obj");
	groundModel = new Model(directory + "groundTile1.obj");
	brickModel = new Model(directory + "brick.obj");
	sceneryModel = new Model(directory + "sceneryGround.obj");

	flameModel = new Model(directory + "flame.obj");
	bombModel = new Model(directory + "bomb.obj");

	flameUpModel = new Model(directory + "flameUp.obj");
	bombUpModel = new Model(directory + "bombUp.obj");
	speedUpModel = new Model(directory + "speedUp.obj");

	baloonModel = new Model(directory + "mob.fbx");
	
	aiDebug = new Model(directory + "aiDebug.obj");
	winSpot = new Model(directory + "winSpot.obj");
}

void	ModelManager::setThemeForest() {
	wallModel = new Model(directory + "wall.obj");
	groundModel = new Model(directory + "groundTile1.obj");
	brickModel = new Model(directory + "brick.obj");
	sceneryModel = new Model(directory + "sceneryGround.obj");

	flameModel = new Model(directory + "flame.obj");
	bombModel = new Model(directory + "bomb.obj");

	flameUpModel = new Model(directory + "flameUp.obj");
	bombUpModel = new Model(directory + "bombUp.obj");
	speedUpModel = new Model(directory + "speedUp.obj");

	baloonModel = new Model(directory + "mob.fbx");
	
	aiDebug = new Model(directory + "aiDebug.obj");
	winSpot = new Model(directory + "winSpot.obj");
}