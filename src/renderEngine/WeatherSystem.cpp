/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WeatherSystem.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/06 09:26:10 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/06 15:10:54 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WeatherSystem.hpp"

WeatherSystem::WeatherSystem() {
	dayTime = MIDDAY;
	cloudy = false;
	rainy = false;

	init();
}

WeatherSystem::WeatherSystem(enum time t, bool cloudy, bool rainy)
	: dayTime(t), cloudy(cloudy), rainy(rainy) {
	init();
}

WeatherSystem::~WeatherSystem() {
	delete cloudModel;
	delete rain;
	delete sun;
}

void	WeatherSystem::init() {
	cloudModel = new Model("assets/models/obj/cloud.obj", false);
	rain = new ParticleSystem(glm::vec3(-2.f, -2.f, 10.f), ParticleSystem::type::RAIN);
	sun = new Light(glm::vec3(8.f, 15.f, 20.f), glm::vec3(1.f, 0.941f, 0.713f), Light::DIRECTIONAL);
	lightingValues = glm::vec3(1.f);
	// lightingColor = glm::vec3(1.f);
	// sunPosition = glm::vec3(8.f, 15.f, 20.f);
}

void	WeatherSystem::setTime(enum time t) {
	dayTime = t;
}

void	WeatherSystem::startCloud() {
	
}

void	WeatherSystem::stopCloud() {

}

void	WeatherSystem::startRain() {

}

void	WeatherSystem::stopRain() {

}

glm::vec3 WeatherSystem::getLightingValues() const {
	return lightingValues;
}

Light	&WeatherSystem::getSun() const {
	return *sun;
}

void	WeatherSystem::renderCloud(Shader &shader) const {
	static float offset = 0.f;
	std::vector<glm::mat4> data;

	// for (auto i = entities.begin(); i != entities.end(); i++ ){
	glm::mat4 transform = glm::mat4();
	transform = glm::translate(transform, glm::vec3(-5.f + offset, 12.f, 12.f));
	transform = glm::rotate(transform,glm::radians(90.0f), glm::vec3(1.f, 0.f, 0.f));
	data.push_back(transform);
	// }
	
    cloudModel->setInstanceBuffer(data);  
    cloudModel->draw(shader, data.size());
	if(offset > 20.f)
		offset = 0.f;
	offset += 0.005f;
}

void	WeatherSystem::renderRain(Shader &shader) const {
	rain->draw(shader);
}