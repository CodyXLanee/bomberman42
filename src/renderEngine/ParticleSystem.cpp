/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParticleSystem.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/04 11:26:13 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/04 16:17:52 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParticleSystem.hpp"

ParticleSystem::ParticleSystem(glm::vec3 position, enum type t) : type(t), oriPos(position) {

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

    particleNbr = 1000;
    for(unsigned int i = 0; i < particleNbr; i++) {
        float r1 = static_cast<float>((rand() % 10)) - 10.f;
        float r2 = static_cast<float>((rand() % 10)) - 10.f;
        positions.push_back(position + glm::vec3(r1 / 10.f, r2 / 10.f, 0.f));
    }
}

ParticleSystem::~ParticleSystem() {
    glDeleteBuffers(1, &this->vbo);
	glDeleteBuffers(1, &this->ebo);
	glDeleteBuffers(1, &this->ibo);
	glDeleteVertexArrays(1, &this->vao);
}

void	ParticleSystem::setInstanceBuffer(std::vector<glm::mat4> const & data) {

	glBindVertexArray(this->vao);
	glGenBuffers(1, &this->ibo);
	glBindBuffer(GL_ARRAY_BUFFER, this->ibo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * data.size(), &data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


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
	
	glDeleteBuffers(1, &this->ibo);
}

void	ParticleSystem::draw(Shader *shader) {
    if (positions.size() == 0)
        return;
        
    shader->use();

    update(shader);

	glBindVertexArray(this->vao);
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, particleNbr);
	glBindVertexArray(0);
	return;
}

void	ParticleSystem::update(Shader *shader) {
    std::vector<glm::mat4> data;
	for (auto i = positions.begin(); i != positions.end(); i++){
        float r = static_cast<float>((rand() % 5)) - 2.5f;

        glm::vec3 dir = glm::vec3(r / 100.f, r / 100.f, 0.02f);
        *i += dir;

        float dist = glm::distance(oriPos, *i);
        if (dist > 2) {
            positions.erase(i);
            i--;
            break;
        } else
            shader->setFloat("dist", glm::distance(oriPos, *i));

        glm::mat4 transform = glm::mat4();
        transform = glm::translate(transform, *i);
        transform = glm::rotate(transform,glm::radians(-90.0f), glm::vec3(1.f, 0.f, 0.f));
        transform = glm::scale(transform, glm::vec3(0.02f, 0.02f, 0.02f));
        data.push_back(transform);
    }
    if (data.size() != 0)
        setInstanceBuffer(data);
}