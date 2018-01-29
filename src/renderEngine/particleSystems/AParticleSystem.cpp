/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AParticleSystem.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/04 11:26:13 by tpierron          #+#    #+#             */
/*   Updated: 2018/01/29 09:35:48 by tpierron         ###   ########.fr       */
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

bool	AParticleSystem::isRunning() const {
	return running;
}

AParticleSystem::Type	AParticleSystem::getType() const {
	return type;
}