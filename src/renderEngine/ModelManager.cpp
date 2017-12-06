/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModelManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/06 11:22:44 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/06 11:36:07 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ModelManager.hpp"

ModelManager::ModelManager() {
	groundModel = new Model("assets/models/obj/groundTile1.obj", false);
	wallModel = new Model("assets/models/obj/wall.obj", false);
	playerModel = new Model("assets/models/obj/player.obj", false);
	brickModel = new Model("assets/models/obj/brick.obj", false);
	bombModel = new Model("assets/models/obj/bomb.obj", false);
	flameModel = new Model("assets/models/obj/flame.obj", false);
}

ModelManager::~ModelManager() {
	delete groundModel;
	delete wallModel;
	delete playerModel;
	delete brickModel;
	delete bombModel;
	delete flameModel;
}

Model	&ModelManager::getModel(enum model m) const {
	switch(m) {
		case PLAYER: return *playerModel; break;
		case GROUND: return *groundModel; break;
		case WALL: return *wallModel; break;
		case BRICK: return *brickModel; break;
		case BOMB: return *bombModel; break;
		case FLAME: return *flameModel; break;
	}
}