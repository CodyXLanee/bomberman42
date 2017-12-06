/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 10:27:21 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/06 23:05:25 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Camera.hpp"
#include <cstdlib>

#define CAM_SPEED 1.f

Camera::Camera(glm::vec3 position, glm::vec3 lookAt) :
    position(position), front(glm::normalize(position - lookAt)), up(0.f, 0.f, 1.f),
    initialPosition(position),
    speed(0.5f), sensitivity(0.5f), 
    wiggle_duration(200), wiggle_start(std::chrono::steady_clock::now()) {

        float rotx = atan2( front.y, front.z );
        float roty = atan2( front.x * cos(rotx), front.z );
        float rotz = atan2( cos(rotx), sin(rotx) * sin(roty) );

        yaw = glm::degrees(rotz);
        pitch = glm::degrees(rotx);
        initialYaw = yaw;
        initialPitch = pitch;


        wiggle_yaw = 0.f;
        wiggle_pitch = 0.f;
        is_wiggling = false;
        
        updateRotation(0, 0);
        setup();
        SEventManager::getInstance().registerEvent(Event::BOMB_EXPLODES, MEMBER_CALLBACK(Camera::initWiggle));
}

void    Camera::reset() {
    position = initialPosition;
    yaw = initialYaw;
    pitch = initialPitch;
    updateRotation(0, 0);
}

void    Camera::initWiggle(void *){
    is_wiggling = true;
    wiggle_start = std::chrono::steady_clock::now();
}

void    Camera::wiggle(void){
    std::chrono::milliseconds   time_since_wiggle_start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - wiggle_start);
    if (time_since_wiggle_start > wiggle_duration)
        is_wiggling = false;
    if (is_wiggling) {
        wiggle_yaw = static_cast <float> (rand()) / static_cast <float>(RAND_MAX) * CAM_SPEED;
        wiggle_pitch = static_cast <float> (rand()) / static_cast <float>(RAND_MAX) * CAM_SPEED;
    } else {
        wiggle_yaw = 0.f;
        wiggle_pitch = 0.f;
    }
}

void    Camera::followPlayer(glm::vec2 const * playerPos, std::vector<Action::Enum> & actions){
    std::for_each(actions.begin(), actions.end(),
        [this](Action::Enum action) {
            switch (action) {
                case Action::CAMERA_UP:     pitch += CAM_SPEED; break;
                case Action::CAMERA_DOWN:   pitch -= CAM_SPEED; break;
                case Action::CAMERA_LEFT:   yaw -= CAM_SPEED; break;
                case Action::CAMERA_RIGHT:  yaw += CAM_SPEED; break;
                case Action::RESET_CAMERA:  yaw = initialYaw; pitch = initialPitch ; break;
                default: break;
            }
        }
    );
    actions.erase(std::remove(actions.begin(), actions.end(), Action::BOMB_EXPLODES), actions.end());
    wiggle();
    updateFront();
    glm::vec3   rel_cam_pos(front * -10.f);
    position += (glm::vec3(*playerPos, 0.) + rel_cam_pos - position) / 10.f;
}

void    Camera::update(std::vector<Action::Enum> & actions, int const mouseOffsetX, int const mouseOffsetY, glm::vec2 const *playerPos) {
    if (find(actions.begin(), actions.end(), Action::DEBUG_MODE) != actions.end()) {
        updatePosition(actions);
        updateRotation(mouseOffsetX, mouseOffsetY);
        setup();
    }
    else if (find(actions.begin(), actions.end(), Action::FOLLOW_PLAYER) != actions.end()) {
        followPlayer(playerPos, actions);
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

void    Camera::updateFront(void){
    glm::vec3   f;
    f.x = cos(glm::radians(pitch + wiggle_pitch)) * cos(glm::radians(yaw + wiggle_yaw));
    f.z = sin(glm::radians(pitch + wiggle_pitch));
    f.y = cos(glm::radians(pitch + wiggle_pitch)) * sin(glm::radians(yaw + wiggle_yaw));
    front = glm::normalize(f);
}

void    Camera::updateRotation(int const mouseOffsetX, int const mouseOffsetY) {
    float   xOffset = -mouseOffsetX;
    float   yOffset = -mouseOffsetY;
    xOffset *= sensitivity;
    yOffset *= sensitivity;
    yaw += xOffset;
    pitch += yOffset;

    updateFront();
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