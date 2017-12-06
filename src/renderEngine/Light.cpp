/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Light.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/30 09:38:15 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/06 13:30:03 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Light.hpp"

Light::Light(glm::vec3 position, glm::vec3 color, enum type t)
				: position(position), color(color), type(t) {
	model = new Model("assets/models/obj/light.obj",false);
	lookAt = glm::vec3(5.0f, 5.0f, 0.0f);
}

Light::~Light() {}

glm::vec3	Light::getPosition() const {
	return this->position;
}

glm::vec3	Light::getColor() const {
	return this->color;
}

glm::mat4	Light::getDirectionalLightSpaceMatrix() const {
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.f, 100.f);
	
	glm::mat4 lightView = glm::lookAt(position, 
							lookAt, 
							glm::vec3(0.0f, 0.0f, 1.0f));
	
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;

	return	lightSpaceMatrix;
}

std::vector<glm::mat4>	Light::getOmnidirectionalLightSpaceMatrix() const {
	float aspect = 1024.f/1024.f;
	float near = 0.1f;
	float far = 25.0f;
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);

	std::vector<glm::mat4> shadowTransforms;
	shadowTransforms.push_back(shadowProj * 
				glm::lookAt(position, position + glm::vec3( 1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * 
				glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * 
				glm::lookAt(position, position + glm::vec3( 0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	shadowTransforms.push_back(shadowProj * 
				glm::lookAt(position, position + glm::vec3( 0.0,-1.0, 0.0), glm::vec3(0.0, 0.0,-1.0)));
	shadowTransforms.push_back(shadowProj * 
				glm::lookAt(position, position + glm::vec3( 0.0, 0.0, 1.0), glm::vec3(0.0,-1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * 
				glm::lookAt(position, position + glm::vec3( 0.0, 0.0,-1.0), glm::vec3(0.0,-1.0, 0.0)));

	return shadowTransforms;
}


void	Light::render(Shader &shader, Camera const &camera) const {
    std::vector<glm::mat4> data;

	glm::mat4 transform = glm::mat4();
	transform = glm::mat4(glm::translate(transform, position));
	data.push_back(transform);

	glm::vec3 camPos = camera.getPosition();
	shader.setVec3("viewPos", camPos.x, camPos.y, camPos.z);
    shader.setView();
	model->setInstanceBuffer(data);  
    model->draw(shader, data.size());
}

void	Light::setShaderVariables(Shader &shader) const {
	shader.setVec3("lightPos", position.x, position.y, position.z);
	shader.setVec3("lightColor", color.x, color.y, color.z);
}

void		Light::setPosition(glm::vec3 pos) {
	position = pos;
}