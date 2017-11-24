/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 10:27:21 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/24 10:29:15 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Camera.hpp"

Camera::Camera() {
	this->lookingDistance = 5.f;
	this->positionDistance = 5.f;
	this->height = 2.f;
}

void    Camera::setCamera(float targetX, float gameClock) {
    float posX;
    float posY;
    float lookX;
    float lookY;

    this->matrix = glm::lookAt(
        glm::vec3(posX, posY, this->height),
        glm::vec3(lookX, lookY, 0.f),
        glm::vec3(0.f, 0.f, 1.f)
}

glm::mat4	Camera::getMatrix() const {
	return this->matrix;
}