/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Light.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/30 09:38:15 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/30 10:35:34 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Light.hpp"

Light::Light(glm::vec3 position, glm::vec3 color, enum type t)
				: position(position), color(color), type(t) {
	model = new Model("assets/models/obj/light.obj",false);
}

Light::~Light() {}

glm::vec3	Light::getPosition() const {
	return this->position;
}

glm::vec3	Light::getColor() const {
	return this->color;
}

glm::mat4	Light::getLightSpaceMatrix() const {
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.f, 100.f);
	
	glm::mat4 lightView = glm::lookAt(glm::vec3(20.0f, 20.0f, 20.0f), 
							glm::vec3( 5.0f, 5.0f,  0.0f), 
							glm::vec3( 0.0f, 0.0f,  1.0f));
	
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;

	return	lightSpaceMatrix;
}

void	Light::render(Shader *shader, Camera const &camera) const {
    std::vector<glm::mat4> data;

	glm::mat4 transform = glm::mat4();
	transform = glm::mat4(glm::translate(transform, position));
	data.push_back(transform);

	glm::vec3 camPos = camera.getPosition();
	shader->setVec3("viewPos", camPos.x, camPos.y, camPos.z);
    shader->setView();
	model->setInstanceBuffer(data);  
    model->draw(shader, data.size());
}