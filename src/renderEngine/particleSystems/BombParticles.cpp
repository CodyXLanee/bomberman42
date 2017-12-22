/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BombParticles.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/22 14:55:51 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/22 15:28:49 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BombParticles.hpp"

BombParticles::BombParticles() {
	running = false;
	type = BOMB;
	particleNbr = 0;
	createBuffers();
    setInstanceBuffer();
}


BombParticles::~BombParticles() {
    glDeleteBuffers(1, &this->vbo);
	glDeleteBuffers(1, &this->ebo);
	glDeleteBuffers(1, &this->ibo);
	glDeleteVertexArrays(1, &this->vao);
}

void    BombParticles::update() {
    std::vector<glm::mat4> data;
	for (auto i = bombPlaces.begin(); i != bombPlaces.end(); i++) {
		if (i->second.size() == 0) {
			bombPlaces.erase(i);
			i--;
			continue;
		}
		for(auto it = i->second.begin(); it != i->second.end(); it++) {
			float r = static_cast<float>((rand() % 90)) - 45.f;

			glm::mat4 transform = glm::mat4();
            transform = glm::translate(transform, *it);
            transform = glm::rotate(transform,glm::radians(-90.0f), glm::vec3(1.f, 0.f, 0.f));
            transform = glm::rotate(transform,glm::radians(r), glm::vec3(0.f, 0.f, 1.f));
            transform = glm::scale(transform, glm::vec3(0.01f, 0.1f, 1.f));

			data.push_back(transform);
		}
	}

	if (bombPlaces.size() == 0)
		running = false;

    if (data.size() != 0) {
		glBindBuffer(GL_ARRAY_BUFFER, this->ibo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * data.size(), &data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	std::cout << running << " : " << particleNbr << " : " << bombPlaces.size() << std::endl;
}

void    BombParticles::addBombPlace(glm::vec2 pos) {
    particleNbr += PARTICLES_BY_BOMB;
	
	std::vector<glm::vec3> particles;
	glm::vec3 ori = glm::vec3(pos.x + 0.5f, pos.y + 0.5f, 1.f);
	for(unsigned int i = 0; i < PARTICLES_BY_BOMB; i++) {
		particles.push_back(ori + glm::vec3(0.08f, 0.f, 0.59f));
	}
	bombPlaces.push_back(std::pair<glm::vec3, std::vector<glm::vec3>>(ori, particles));
	running = true;
}

void    BombParticles::removeBombPlace(glm::vec2 pos) {
    glm::vec3 ori = glm::vec3(pos.x + 0.5f, pos.y + 0.5f, 1.f);

    for (auto i = bombPlaces.begin(); i != bombPlaces.end(); i++) {
        if (i->first == ori) {
            bombPlaces.erase(i);
            break;
        }
    }
    particleNbr -= PARTICLES_BY_BOMB;
}