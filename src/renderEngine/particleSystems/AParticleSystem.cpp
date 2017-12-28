/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AParticleSystem.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/04 11:26:13 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/22 15:30:51 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AParticleSystem.hpp"

AParticleSystem::AParticleSystem()
	: running(false), particleNbr(0) {
	createBuffers();
	setInstanceBuffer();
}


AParticleSystem::~AParticleSystem() {
    glDeleteBuffers(1, &this->vbo);
	glDeleteBuffers(1, &this->ebo);
	glDeleteBuffers(1, &this->ibo);
	glDeleteVertexArrays(1, &this->vao);
}

void	AParticleSystem::createBuffers() {
    GLfloat vertices[] = {-1, -1, 0,
                            -1,  1, 0,
                            1,  1, 0,
                            1, -1, 0};

    GLuint indices[] = {0,1,2,
                        0,2,3};
    
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);

	setInstanceBuffer();
}

void	AParticleSystem::setInstanceBuffer() {

	glBindVertexArray(this->vao);
	glGenBuffers(1, &this->ibo);
	glBindBuffer(GL_ARRAY_BUFFER, this->ibo);

	glBindBuffer(GL_ARRAY_BUFFER, this->ibo);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (GLvoid*)(0));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (GLvoid*)(sizeof(glm::vec4)));
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (GLvoid*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(8);
	glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (GLvoid*)(3 * sizeof(glm::vec4)));
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);	
	glVertexAttribDivisor(5, 1); 
	glVertexAttribDivisor(6, 1); 
	glVertexAttribDivisor(7, 1); 
	glVertexAttribDivisor(8, 1); 
	glBindVertexArray(0);
}

// void	ParticleSystem::init() {
// 	positions.clear();
// 	switch(type) {
// 		case FIRE:
// 			particleNbr = 10;
// 			for(unsigned int i = 0; i < particleNbr; i++) {
// 				float r1 = static_cast<float>((rand() % 10)) - 5.f;
// 				float r2 = static_cast<float>((rand() % 10)) - 5.f;
// 				positions.push_back(oriPos + glm::vec3(r1 / 10.f, r2 / 10.f, 0.5f));
// 			}
// 			break;
// 		case BOMB:
// 			particleNbr = 3;
// 			for(unsigned int i = 0; i < particleNbr; i++) {
// 				positions.push_back(oriPos + glm::vec3(0.08f, 0.f, 0.59f));
// 			}
// 			break;
// 		case RAIN:
// 			particleNbr = 1000;
// 			for(unsigned int i = 0; i < particleNbr; i++) {
// 				float r1 = static_cast<float>((rand() % 300)) / 10.f;
// 				float r2 = static_cast<float>((rand() % 300)) / 10.f;
// 				float r3 = static_cast<float>((rand() % 300)) / 10.f;
// 				positions.push_back(oriPos + glm::vec3(r1, r2, r3));
// 			}
// 			break;
// 	}
// }

void	AParticleSystem::draw(Shader &shader) {
    this->shader = &shader;
    shader.use();
	this->shader->setInt("type", type);
	update();

	glBindVertexArray(this->vao);
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, particleNbr);
	glBindVertexArray(0);
	return;
}




// void	ParticleSystem::updateRain(Shader &shader) {
//     std::vector<glm::mat4> data;
// 	shader.setInt("type", type);
	
// 	for (auto i = positions.begin(); i != positions.end(); i++){
//         // float r = static_cast<float>((rand() % 20)) / 10.f;

//         glm::vec3 dir = glm::vec3(0.f, 0.f, - 0.2f);
//         *i += dir;

//         // float dist = glm::distance(oriPos, *i);
//         if (i->z < -5) {
// 			if (running) {
// 				float r1 = static_cast<float>((rand() % 300)) / 10.f;
// 				float r2 = static_cast<float>((rand() % 300)) / 10.f;
// 				float r3 = static_cast<float>((rand() % 300)) / 10.f;
// 				*i = oriPos + glm::vec3(r1, r2, r3);
// 				i--;
// 			} else {
// 				positions.erase(i);
// 				i--;
// 			}
//             // break;
// 		}

//         glm::mat4 transform = glm::mat4();
//         transform = glm::translate(transform, *i);
//         transform = glm::rotate(transform,glm::radians(-90.0f), glm::vec3(1.f, 0.f, 0.f));
//         transform = glm::scale(transform, glm::vec3(0.01f, 0.5f, 1.f));
//         data.push_back(transform);
//     }
//     if (data.size() != 0) {
// 		glBindBuffer(GL_ARRAY_BUFFER, this->ibo);
// 		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * data.size(), &data[0], GL_STATIC_DRAW);
// 		glBindBuffer(GL_ARRAY_BUFFER, 0);
// 	}
// }

// void	AParticleSystem::start() {
// 	init();
// 	running = true;
// }

// void	AParticleSystem::stop() {
// 	running = false;
// }

bool	AParticleSystem::isRunning() const {
	return running;
}

AParticleSystem::Type	AParticleSystem::getType() const {
	return type;
}