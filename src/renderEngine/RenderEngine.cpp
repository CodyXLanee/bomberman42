/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RenderEngine.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 16:35:00 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/24 10:39:49 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RenderEngine.hpp"

RenderEngine::RenderEngine() {
	shader = new Shader("src/renderEngine/shaders/static_model.glvs",
						"src/renderEngine/shaders/simple_diffuse.glfs");
	model = new Model("assets/models/obj/groundTile1.obj", false);
}

RenderEngine::~RenderEngine() {}

void	RenderEngine::render(Map const & map, std::vector<IGameEntity const *> const & entities) {
	(void)map; //////////
	(void)entities; ////////////
	setCamera();
	renderMap();
}

void	RenderEngine::renderMap() const {
	renderGround();
}

void	RenderEngine::renderGround() const {
	// std::vector<glm::mat4> data;
    glm::mat4 mat;
    // model = glm::scale(model, glm::vec3(1.f / scalingRate, 1.f / scalingRate, 1.f / scalingRate));
    // model = glm::translate(model, glm::vec3(xScaled, yScaled, this->z));
    // model = glm::rotate(model, glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
    // model = glm::rotate(model, glm::radians(angle), glm::vec3(0.f, 1.f, 0.f));
    // data.push_back(model);
    
    shader->use();
	shader->setModel(mat);
    // model->setInstanceBuffer(data);

    shader->setView();
    model->draw(shader, 1);
}

void	RenderEngine::setCamera() {
    glm::mat4 camera = glm::lookAt(
        glm::vec3(5.f, 5.f, 5.f),
        glm::vec3(0.f, 0.f, 0.f),
        glm::vec3(0.f, 0.f, 1.f)
    );
    shader->setCamera(camera);
    shader->setView();
}