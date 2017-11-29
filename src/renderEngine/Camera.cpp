/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 10:27:21 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/29 22:42:37 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Camera.hpp"

#define RELATIVE_CAM_POS glm::vec3(-3.f, -10.f, 12.f))
#define FRONT_FOLLOW_PLAYER glm::vec3(0.30f, 0.50f, -0.80f)

Camera::Camera(glm::vec3 position, glm::vec3 lookAt) :
    position(position), front(glm::normalize(position - lookAt)), up(0.f, 0.f, 1.f),
    initialPosition(position),
    speed(0.5f), sensitivity(0.5f) {

        float rotx = atan2( front.y, front.z );
        float roty = atan2( front.x * cos(rotx), front.z );
        float rotz = atan2( cos(rotx), sin(rotx) * sin(roty) );

        yaw = glm::degrees(rotz);
        pitch = glm::degrees(rotx);
        initialYaw = yaw;
        initialPitch = pitch;
        
        updateRotation(0, 0);
        setup();
}

void    Camera::reset() {
    position = initialPosition;
    yaw = initialYaw;
    pitch = initialPitch;
    updateRotation(0, 0);
}

void    Camera::followPlayer(glm::vec2 const * playerPos){
    position += ((glm::vec3(*playerPos, 0.) + RELATIVE_CAM_POS - position) / 10.f;
    front = glm::normalize(FRONT_FOLLOW_PLAYER);
}

void    Camera::update(std::vector<Action::Enum> const & actions, int const mouseOffsetX, int const mouseOffsetY, glm::vec2 const *playerPos) {
    if (find(actions.begin(), actions.end(), Action::DEBUG_MODE) != actions.end()) {
        updatePosition(actions);
        updateRotation(mouseOffsetX, mouseOffsetY);
        setup();
    }
    else if (find(actions.begin(), actions.end(), Action::FOLLOW_PLAYER) != actions.end()) {
        followPlayer(playerPos);
        setup();
    }
}

void    Camera::updatePosition(std::vector<Action::Enum> const & actions) {
    std::for_each(actions.begin(), actions.end(),
        [this](Action::Enum action) {
            switch (action) {
                case Action::CAMERA_UP:     position += speed * front; break;
                case Action::CAMERA_DOWN:   position -= speed * front; break;
                case Action::CAMERA_LEFT:   position -= glm::normalize(glm::cross(front, up)) * speed; break;
                case Action::CAMERA_RIGHT:  position += glm::normalize(glm::cross(front, up)) * speed; break;
                case Action::RESET_CAMERA:  reset(); break;
                default: break;
            }
        }
    );
}

void    Camera::updateRotation(int const mouseOffsetX, int const mouseOffsetY) {
        float   xOffset = -mouseOffsetX;
        float   yOffset = -mouseOffsetY;
        xOffset *= sensitivity;
        yOffset *= sensitivity;
        yaw += xOffset;
        pitch += yOffset;

        glm::vec3   f;
        f.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
        f.z = sin(glm::radians(pitch));
        f.y = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
        front = glm::normalize(f);
}

void    Camera::setup() {
    matrix = glm::lookAt(position, position + front, up);
}

glm::mat4   Camera::getMatrix() const {
	return matrix;
}

glm::vec3   Camera::getPosition() const {
	return position;
}

glm::vec3   Camera::getFront() const {
	return front;
}