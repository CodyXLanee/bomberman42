/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 10:27:21 by tpierron          #+#    #+#             */
/*   Updated: 2018/01/04 15:27:44 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Camera.hpp"
#include <cstdlib>

Camera::Camera(glm::vec3 position, glm::vec3 lookAt) :
    position(position), front(glm::normalize(position - lookAt)), up(0.f, 0.f, 1.f),
    mode(Camera::Mode::FOLLOW_PLAYER),
    initialPosition(position),
    speed(0.5f), sensitivity(0.5f), 
    wiggle_duration(500), wiggle_start(std::chrono::steady_clock::now()) {

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

        SEventManager & event = SEventManager::getInstance();
        event.registerEvent(Event::BOMB_EXPLODES, MEMBER_CALLBACK(Camera::initWiggle));
        event.registerEvent(Event::CAMERA_MODE_UPDATE, MEMBER_CALLBACK(Camera::setMode));
        event.registerEvent(Event::CAMERA_LEFT, MEMBER_CALLBACK(Camera::moveLeft));
        event.registerEvent(Event::CAMERA_RIGHT, MEMBER_CALLBACK(Camera::moveRight));
        event.registerEvent(Event::CAMERA_UP, MEMBER_CALLBACK(Camera::moveUp));
        event.registerEvent(Event::CAMERA_DOWN, MEMBER_CALLBACK(Camera::moveDown));
}

void    Camera::moveLeft(void *) {
    switch (mode) {
        case FREE:          position -= glm::normalize(glm::cross(front, up)) * speed; break;
        case FOLLOW_PLAYER: yaw -= speed; break;
        default: break;
    }
}

void    Camera::moveRight(void *) {
    switch (mode) {
        case FREE:          position += glm::normalize(glm::cross(front, up)) * speed; break;
        case FOLLOW_PLAYER: yaw += speed; break;
        default: break;
    }
}

void    Camera::moveUp(void *) {
    switch (mode) {
        case FREE:          position += speed * front; break;
        case FOLLOW_PLAYER: pitch += speed; break;
        default: break;
    }
}

void    Camera::moveDown(void *) {
    switch (mode) {
        case FREE:          position -= speed * front; break;
        case FOLLOW_PLAYER: pitch -= speed; break;
        default: break;
    }
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
        wiggle_yaw = static_cast <float> (rand()) / static_cast <float>(RAND_MAX) * speed;
        wiggle_pitch = static_cast <float> (rand()) / static_cast <float>(RAND_MAX) * speed;
    } else {
        wiggle_yaw = 0.f;
        wiggle_pitch = 0.f;
    }
}

void    Camera::update(int const mouseOffsetX, int const mouseOffsetY, glm::vec2 const *playerPos) {
    switch (mode) {
        case FIXED:         break;
        case FREE:          updateRotation(mouseOffsetX, mouseOffsetY); break;
        case FOLLOW_PLAYER:
            wiggle();
            updateFront();
            glm::vec3   rel_cam_pos(front * -10.f);
            position += (glm::vec3(*playerPos, 0.) + rel_cam_pos - position) / 10.f;
            break;
    }
    setup();
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

void    Camera::setMode(void *p) {
    mode = *static_cast<Camera::Mode*>(p);
    reset();
}

Camera::Mode   Camera::getMode() const {
    return mode;
}