/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 10:27:21 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/24 16:18:59 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 lookAt) :
    position(position),
    lookAt(lookAt),
    up(0.f, 0.f, 1.f) {
    setup();
}

void    Camera::update(std::vector<Action::Enum> const & actions) {
    std::for_each(actions.begin(), actions.end(),
        [this](Action::Enum action) {
            switch (action) {
                case Action::CAMERA_LEFT:  position.x += 0.1f; break;
                case Action::CAMERA_RIGHT: position.x -= 0.1f; break;
                case Action::CAMERA_UP:    position.z += 0.1f; break;
                case Action::CAMERA_DOWN:  position.z -= 0.1f; break;
                default: break;
            }
        }
    );
    setup();
}

void    Camera::setup() {
    matrix = glm::lookAt(position, lookAt, up);
}

glm::mat4	Camera::getMatrix() const {
	return matrix;
}