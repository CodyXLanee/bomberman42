/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FireParticles.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/22 11:52:27 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/22 14:45:19 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FireParticles.hpp"

FireParticles::FireParticles() {
	running = false;
	type = FIRE;
	particleNbr = 0;
	createBuffers();
    setInstanceBuffer();
}


FireParticles::~FireParticles() {
    glDeleteBuffers(1, &this->vbo);
	glDeleteBuffers(1, &this->ebo);
	glDeleteBuffers(1, &this->ibo);
	glDeleteVertexArrays(1, &this->vao);
}

void    FireParticles::update(Shader &shader) {


    std::vector<glm::mat4> data;
	for (auto i = firePlaces.begin(); i != firePlaces.end(); i++) {
		if (i->second.size() == 0) {
			firePlaces.erase(i);
			i--;
			continue;
		}
		for(auto it = i->second.begin(); it != i->second.end(); it++) {
			float r = static_cast<float>((rand() % 5)) - 2.5f;

			glm::vec3 dir = glm::vec3(r / 100.f, r / 100.f, 0.02f);
			*it += dir;

			float dist = glm::distance(i->first, *it);
			if (dist > 2) {
				particleNbr--;
				i->second.erase(it);
				it--;
				continue;
			}
			shader.setFloat("dist", dist);
			shader.setInt("type", type);

			glm::mat4 transform = glm::mat4();
			transform = glm::translate(transform, *it);
			transform = glm::rotate(transform,glm::radians(-90.0f), glm::vec3(1.f, 0.f, 0.f));
			transform = glm::scale(transform, glm::vec3(0.02f, 0.02f, 0.02f));
			data.push_back(transform);
		}
	}

	if (firePlaces.size() == 0)
		running = false;

    if (data.size() != 0) {
		glBindBuffer(GL_ARRAY_BUFFER, this->ibo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * data.size(), &data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	// std::cout << running << " : " << particleNbr << " : " << firePlaces.size() << std::endl;
}

void    FireParticles::addFirePlace(glm::vec2 pos) {
    particleNbr += 10;
	
	std::vector<glm::vec3> particles;
	glm::vec3 ori = glm::vec3(pos.x + 0.5f, pos.y + 0.5f, 0.f);
	for(unsigned int i = 0; i < 10; i++) {
		float r1 = static_cast<float>((rand() % 10)) - 5.f;
		float r2 = static_cast<float>((rand() % 10)) - 5.f;
		particles.push_back(ori + glm::vec3(r1 / 10.f, r2 / 10.f, 0.5f));
	}
	firePlaces.push_back(std::pair<glm::vec3, std::vector<glm::vec3>>(ori, particles));
	running = true;
}