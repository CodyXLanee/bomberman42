/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 10:27:21 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/27 12:30:18 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 lookAt) :
    position(position), front(glm::normalize(lookAt - position)), up(0.f, 0.f, 1.f),
    resetPosition(position), resetFront(glm::normalize(lookAt - position)),
    speed(0.5f),
    sensitivity(0.5f), yaw(0.f), pitch(0.f) {       
        setup();
}

void    Camera::reset() {
    position = resetPosition;
    front = resetFront;
}

void    Camera::update(std::vector<Action::Enum> const & actions, int const mouseOffsetX, int const mouseOffsetY) {
    if (find(actions.begin(), actions.end(), Action::DEBUG_MODE) != actions.end()) {
        updatePosition(actions);
        updateRotation(mouseOffsetX, mouseOffsetY);
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
//    std::cout << position.x << " " << position.y << " " << position.z << std::endl;
//   std::cout << front.x << " " << front.y << " " << front.z << std::endl;
    matrix = glm::lookAt(position, position + front, up);
}

glm::mat4   Camera::getMatrix() const {
	return matrix;
}

glm::vec3   Camera::getPosition() const {
	return position;
}