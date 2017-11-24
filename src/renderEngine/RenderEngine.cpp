/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RenderEngine.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 16:35:00 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/24 16:41:06 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RenderEngine.hpp"

RenderEngine::RenderEngine(SDL_Window *win) : win(win) {
	shader = new Shader("src/renderEngine/shaders/static_model_instanced.glvs",
						"src/renderEngine/shaders/simple_diffuse.glfs");
	groundModel = new Model("assets/models/obj/groundTile1.obj", false);
	wallModel = new Model("assets/models/obj/wall.obj", false);
	playerModel = new Model("assets/models/obj/player.obj", false);
}

RenderEngine::~RenderEngine() {}

void	RenderEngine::render(Map const & map, std::vector<IGameEntity const *> const & entities, glm::mat4 const & cameraMatrix) {
	(void)map; //////////
	(void)entities; ////////////
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	setCamera(cameraMatrix);
	renderMap();
	for (std::vector<IGameEntity const *>::const_iterator i = entities.begin(); i != entities.end(); i++ ){
		if ((*i)->getType() == Type::PLAYER)
			renderPlayer(*i);
	}
	SDL_GL_SwapWindow(win);
}

void	RenderEngine::renderMap() const {
	renderGround();
	renderWall();
}

void	RenderEngine::renderPlayer(IGameEntity const *player) const {
    std::vector<glm::mat4> data;
	
	glm::mat4 transform = glm::mat4();
	transform = glm::translate(transform, glm::vec3(player->getPositon(), 0.f));
	// transform = glm::rotate(transform, glm::radians(findHeadOrientation()), glm::vec3(0.f, 1.f, 0.f));
	data.push_back(transform);

    shader->use();
    playerModel->setInstanceBuffer(data);  
    playerModel->draw(shader, 2);
}

void	RenderEngine::renderGround() const {
    std::vector<glm::mat4> data;
	
	for(float j = 0; j < 10 ; j++) {
		for(float i = 0; i < 10 ; i++) {
			glm::mat4 transform = glm::mat4();
			transform = glm::translate(transform, glm::vec3(i, j, 0.f));
			// transform = glm::rotate(transform, glm::radians(findHeadOrientation()), glm::vec3(0.f, 1.f, 0.f));
			data.push_back(transform);
		}
	}

    shader->use();
    groundModel->setInstanceBuffer(data);  
    groundModel->draw(shader, data.size());
}

void	RenderEngine::renderWall() const {
    std::vector<glm::mat4> data;
	
	for(float i = -2; i < 11 ; i++) {
			glm::mat4 transform = glm::mat4();
			transform = glm::translate(transform, glm::vec3(-1, i, 0.f));
			data.push_back(transform);
			transform = glm::translate(transform, glm::vec3(11, 0.f, 0.f));
			data.push_back(transform);
	}
	for(float i = 0; i < 10 ; i++) {
			glm::mat4 transform = glm::mat4();
			transform = glm::translate(transform, glm::vec3(i, -2, 0.f));
			data.push_back(transform);
			transform = glm::translate(transform, glm::vec3(0.f, 12.f, 0.f));
			data.push_back(transform);
	}

    shader->use();
    wallModel->setInstanceBuffer(data);  
    wallModel->draw(shader, data.size());
}

void	RenderEngine::setCamera(glm::mat4 const & cameraMatrix) {
	shader->use();
    shader->setCamera(cameraMatrix);
    shader->setView();
}