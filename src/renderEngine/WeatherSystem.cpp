/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WeatherSystem.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/06 09:26:10 by tpierron          #+#    #+#             */
/*   Updated: 2018/01/31 10:38:37 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WeatherSystem.hpp"

WeatherSystem::WeatherSystem() {
	dayTime = MIDDAY;
	cloudy = false;
	rainy = false;

	init();
	initClouds();
	SEventManager::getInstance().registerEvent(Event::SET_THEME, MEMBER_CALLBACK(WeatherSystem::setSunTheme));
}

WeatherSystem::WeatherSystem(enum time t, bool cloudy, bool rainy)
	: dayTime(t), cloudy(cloudy), rainy(rainy) {
	init();
	if(cloudy)
		initClouds();
}

WeatherSystem::WeatherSystem(WeatherSystem const & src) {
	*this = src;
}

WeatherSystem&	WeatherSystem::operator=(WeatherSystem const & rhs) {
	if (this != &rhs)
		sun = &rhs.getSun();

	return *this;
}

WeatherSystem::~WeatherSystem() {
	delete cloudModel;
	// delete rain;
	delete sun;
}

void	WeatherSystem::init() {
	cloudModel = new Model("assets/models/obj/cloud.obj");

	float ambiant, diffuse, specular;
	glm::vec3 sunPos, sunColor;

	switch (dayTime) {
		case MIDDAY:
			ambiant = 0.4f;
			diffuse = 0.6f;
			specular = 0.6f;
			sunPos = glm::vec3(16.f, 30.f, 40.f);
			sunColor = glm::vec3(1.f, 0.941f, 0.713f);
			break;
		case TWILIGHT:
			ambiant = 0.4f;
			diffuse = 0.6f;
			specular = 0.8f;
			sunPos = glm::vec3(16.f, 30.f, 10.f);
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
		ambiant *= 0.7f;
		diffuse *= 0.7f;
		specular *= 0.7f;
	}
	if (rainy) {
		specular *= 2;
	}

	/////////
		ambiant = 0.3f;
		diffuse = 0.9f;
		specular = 1.f;
	/////////

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
		transform = glm::scale(transform, glm::vec3(4.f, 2.f, 0.f));
		transform = glm::rotate(transform,glm::radians(90.0f), glm::vec3(1.f, 0.f, 0.f));
		data.push_back(transform);
	if ((*it).x > 40.f) {
			float rX = static_cast<float>((rand() % 400)) / 10.f;
			float rY = static_cast<float>((rand() % 300)) / 10.f;
			(*it) = glm::vec3( -5 - rX, rY + 5, 12.f);
		}
	}

    cloudModel->draw(shader, data);
}

void	WeatherSystem::renderRain(Shader &shader) {
	if (rainy)
		rain.draw(shader);
}

void	WeatherSystem::initClouds() {
	unsigned int cloudsNbr = 3;
	for(unsigned int i = 0; i < cloudsNbr; i++) {
		float rX = static_cast<float>((rand() % 200)) / 10.f;
		float rY = static_cast<float>((rand() % 150)) / 10.f;
		cloudsPosition.push_back(glm::vec3( -5 - rX, rY + 5, 12.f));
	}
}

void	WeatherSystem::setSunTheme(void *p) {
	Theme::Enum t = *static_cast<Theme::Enum * >(p);
	switch (t) {
		case Theme::LAVA:
			sun->setColor(glm::vec3(1.f, 0.9f, 0.9f));
			cloudy = false;
			rainy = false;
			rain.stopRain();
			break;
		case Theme::FOREST:
			sun->setColor(glm::vec3(0.9f, 1.f, 0.9f));
			cloudy = true;
			rainy = true;
			rain.startRain();
			break;
		case Theme::ICE:
			sun->setColor(glm::vec3(0.9f, 0.9f, 1.f));
			cloudy = false;
			rainy = false;
			rain.stopRain();
			break;
	}
}