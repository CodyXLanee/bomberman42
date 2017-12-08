/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WeatherSystem.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/06 09:26:10 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/08 13:44:32 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WeatherSystem.hpp"

WeatherSystem::WeatherSystem() {
	dayTime = MIDDAY;
	cloudy = false;
	rainy = false;

	init();
	initClouds();
}

WeatherSystem::WeatherSystem(enum time t, bool cloudy, bool rainy)
	: dayTime(t), cloudy(cloudy), rainy(rainy) {
	init();
	if(cloudy)
		initClouds();
}

WeatherSystem::~WeatherSystem() {
	delete cloudModel;
	delete rain;
	delete sun;
}

void	WeatherSystem::init() {
	cloudModel = new Model("assets/models/obj/cloud.obj", false);
	rain = new ParticleSystem(rainy, glm::vec3(-2.f, -2.f, 10.f), ParticleSystem::type::RAIN);

	float ambiant, diffuse, specular;
	glm::vec3 sunPos, sunColor;

	switch (dayTime) {
		case MIDDAY:
			ambiant = 0.4f;
			diffuse = 0.6f;
			specular = 0.2f;
			sunPos = glm::vec3(16.f, 30.f, 40.f);
			sunColor = glm::vec3(1.f, 0.941f, 0.713f);
			break;
		case TWILIGHT:
			ambiant = 0.4f;
			diffuse = 0.6f;
			specular = 0.2f;
			sunPos = glm::vec3(16.f, 30.f, 20.f);
			sunColor = glm::vec3(1.f, 0.941f, 0.713f);
			break;
		case MIDNIGHT:
			ambiant = 0.2f;
			diffuse = 0.2f;
			specular = 0.2f;
			sunPos = glm::vec3(16.f, 30.f, 40.f);
			sunColor = glm::vec3(1.f, 1.f, 1.f);
			break;
	}

	if (cloudy) {
		ambiant /= 2;
		diffuse /= 2;
		specular /= 2;
	}
	if (rainy) {
		specular *= 2;
	}

	sun = new Light(sunPos, sunColor, Light::DIRECTIONAL);
	lightingValues = glm::vec3(ambiant, diffuse, specular);
}

void	WeatherSystem::setTime(enum time t) {
	dayTime = t;
}

void	WeatherSystem::startCloud(void*) {
	
}

void	WeatherSystem::stopCloud(void*) {

}

void	WeatherSystem::startRain(void*) {
	rainy = true;
	rain->start();
}

void	WeatherSystem::stopRain(void*) {
	rainy = false;
	rain->stop();
}

glm::vec3 const &WeatherSystem::getLightingValues() const {
	return lightingValues;
}

Light	&WeatherSystem::getSun() const {
	return *sun;
}

void	WeatherSystem::renderCloud(Shader &shader) {
	static float offset = 0.01f;
	std::vector<glm::mat4> data;
	if (!cloudy)
		return;
	for (auto it = cloudsPosition.begin(); it != cloudsPosition.end(); it++ ){
		(*it).x += offset;
		glm::mat4 transform = glm::mat4();
		transform = glm::translate(transform, *it);
		transform = glm::rotate(transform,glm::radians(90.0f), glm::vec3(1.f, 0.f, 0.f));
		data.push_back(transform);
		if ((*it).x > 20.f) {
			float rX = static_cast<float>((rand() % 200)) / 10.f;
			float rY = static_cast<float>((rand() % 150)) / 10.f;
			(*it) = glm::vec3( -5 - rX, rY + 5, 12.f);
		}
	}
	
    cloudModel->setInstanceBuffer(data);  
    cloudModel->draw(shader, data.size());
}

void	WeatherSystem::renderRain(Shader &shader) const {
	rain->draw(shader);
}

void	WeatherSystem::initClouds() {
	unsigned int cloudsNbr = 3;
	for(unsigned int i = 0; i < cloudsNbr; i++) {
		float rX = static_cast<float>((rand() % 200)) / 10.f;
		float rY = static_cast<float>((rand() % 150)) / 10.f;
		cloudsPosition.push_back(glm::vec3( -5 - rX, rY + 5, 12.f));
	}
}