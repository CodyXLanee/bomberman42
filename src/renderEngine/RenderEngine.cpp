/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RenderEngine.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 16:35:00 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/24 11:55:54 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RenderEngine.hpp"

RenderEngine::RenderEngine(SDL_Window *win) : win(win) {
	shader = new Shader("src/renderEngine/shaders/static_model_instanced.glvs",
						"src/renderEngine/shaders/simple_diffuse.glfs");
	groundModel = new Model("assets/models/obj/groundTile1.obj", false);
	wallModel = new Model("assets/models/obj/wall.obj", false);
}

RenderEngine::~RenderEngine() {}

void	RenderEngine::render(Map const & map, std::vector<IGameEntity const *> const & entities) {
	(void)map; //////////
	(void)entities; ////////////
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	setCamera();
	renderMap();

	SDL_GL_SwapWindow(win);
}

void	RenderEngine::renderMap() const {
	renderGround();
	renderWall();
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
    groundModel->draw(shader, 100);
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
    wallModel->draw(shader, 100);
}

void	RenderEngine::setCamera() {
    glm::mat4 camera = glm::lookAt(
        glm::vec3(5.f, 15.f, 10.f),
        glm::vec3(5.f, 5.f, 0.f),
        glm::vec3(0.f, 0.f, 1.f)
    );
    shader->setCamera(camera);
    shader->setView();
}