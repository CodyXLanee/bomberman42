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
    _lookAt(lookAt),
    mode(Camera::Mode::FOLLOW_PLAYER),
    initialPosition(position),
    speed(0.5f), sensitivity(0.5f), 
    wiggle_duration(500), wiggle_start(std::chrono::steady_clock::now()),
    is_standing_animation(false) {

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

        event.registerEvent(Event::START_ANIMATION, MEMBER_CALLBACK(Camera::animations));

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

void    Camera::initFirstAnimation(){
    is_first_animation = true;
    first_animation_start = std::chrono::steady_clock::now();
    first_animation_init_pos = position;
    position = position + glm::vec3(0.f, 30.f, 10.f);
    first_animation_init_yaw = yaw;
    yaw += 180.f;
    first_animation_init_pitch = pitch;
    pitch -= 45.f;
}

void    Camera::firstAnimation(glm::vec2 playerPos){
    if (mode == FOLLOW_PLAYER)
        first_animation_init_pos = glm::vec3(playerPos.x + 0.5f, playerPos.y + 0.5f, 0.f) + front * -4.f;
    if (position.z - first_animation_init_pos.z < 0.002f)
    {
        position = first_animation_init_pos;
        yaw = first_animation_init_yaw;
        pitch = first_animation_init_pitch;
    }
    if (position == first_animation_init_pos)
        is_first_animation = false;
    if (is_first_animation) {
        position = position - (position - first_animation_init_pos) * 0.06f;
        yaw = yaw - (yaw - first_animation_init_yaw) * 0.06f;
        pitch = pitch - (pitch - first_animation_init_pitch) * 0.06f;
        updateFront();
    }
}

void    Camera::initStandingAnimation(){
    is_standing_animation = true;
    counter_standing_animation = 250;
    standing_animation_init_yaw = yaw;
    standing_animation_init_front = front;
}

void    Camera::standingAnimation(glm::vec2 playerPos){
    if (!is_standing_animation)
        return;
    if (counter_standing_animation == 250)
    {
        front = standing_animation_init_front;
        switch (rand() % 4)
        {
        case 0:
            position = glm::vec3(playerPos.x + 0.5f, playerPos.y + 0.5f, 0.f) + front * -5.f;
            yaw = standing_animation_init_yaw;
            break;
        case 1:
            position = glm::vec3(playerPos.x + 0.5f + front.x * -5.f, playerPos.y + 0.5f - front.y * -5.f, 0.f + front.z * -5.f);
            yaw = standing_animation_init_yaw - 180.f;
            break;
        case 2:
            position = glm::vec3(playerPos.x + 0.5f + front.y * -5.f, playerPos.y + 0.5f + front.x * -5.f, 0.f + front.z * -5.f);
            yaw = standing_animation_init_yaw - 90.f;
            break;
        case 3:
            position = glm::vec3(playerPos.x + 0.5f - front.y * -5.f, playerPos.y + 0.5f + front.x * -5.f, 0.f + front.z * -5.f);
            yaw = standing_animation_init_yaw + 90.f;
            break;
        }
        switch (rand() % 4)
        {
        case 0:
            standing_animation_add_pos = glm::vec3(0.f, 0.02f, 0.f);
            break;
        case 1:
            standing_animation_add_pos = glm::vec3(0.f, -0.02f, 0.f);
            break;
        case 2:
            standing_animation_add_pos = glm::vec3(0.02f, 0.f, 0.f);
            break;
        case 3:
            standing_animation_add_pos = glm::vec3(-0.02f, 0.f, 0.f);
            break;
        }
        updateFront();
    }
    counter_standing_animation--;
    if (counter_standing_animation <= 0)
    {
        counter_standing_animation = 250;
    }
    if (is_standing_animation) {
        position += standing_animation_add_pos;
    }
}

void    Camera::animations(void *anim) {
    Animation::Enum *animation = static_cast<Animation::Enum *>(anim);

    switch (*animation) {
        case Animation::Enum::START:          initFirstAnimation(); break;
        case Animation::Enum::GAME_OVER:
        case Animation::Enum::WIN:          initStandingAnimation(); break;
        default: break;
    }
}

void    Camera::update(int const mouseOffsetX, int const mouseOffsetY, glm::vec2 const *playerPos) {
    firstAnimation(*playerPos);
    standingAnimation(*playerPos);

    if (!is_first_animation && !is_standing_animation)
    {
        switch (mode) {
            case FIXED:         break;
            case FREE:          updateRotation(mouseOffsetX, mouseOffsetY); break;
            case FOLLOW_PLAYER:
                wiggle();
                updateFront();
                glm::vec3   rel_cam_pos(front * -10.f);
                position += (glm::vec3(*playerPos + 0.5f, 0.) + rel_cam_pos - position) / 10.f;
                break;
        }
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