/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RenderEngine.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 16:35:00 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/29 11:48:23 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RenderEngine.hpp"

RenderEngine::RenderEngine(SDL_Window *win, Camera & camera) : win(win), camera(camera) {
	createShadowBuffer();
	shader = new Shader("src/renderEngine/shaders/static_model_instanced.glvs",
						"src/renderEngine/shaders/simple_diffuse.glfs");
	textureShader = new Shader("src/renderEngine/shaders/static_model_instanced.glvs",
								"src/renderEngine/shaders/diffuse_texture.glfs");
	shadowShader = new Shader("src/renderEngine/shaders/directionalShadowDepth.glvs",
								"src/renderEngine/shaders/empty.glfs");
	debugDepthQuad = new Shader("src/renderEngine/shaders/debugShadow.glvs",
								"src/renderEngine/shaders/debugShadow.glfs");
	
	groundModel = new Model("assets/models/obj/groundTile1.obj", false);
	wallModel = new Model("assets/models/obj/wall.obj", false);
	playerModel = new Model("assets/models/obj/player.obj", false);
	brickModel = new Model("assets/models/obj/brick.obj", false);

	textureShader->use();
	textureShader->setInt("texture_diffuse", 0);
	textureShader->setInt("depthMap", 1);
	debugDepthQuad->use();
	debugDepthQuad->setInt("depthMap", 0);
}

RenderEngine::~RenderEngine() {}

void	RenderEngine::render(Map const & map, std::vector<IGameEntity *> const & entities) {
	(void)map; //////////
	(void)entities; ////////////
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	setCamera(camera.getMatrix());

	getShadowMap();

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	renderScene(textureShader);
	// renderShadowMap();
	SDL_GL_SwapWindow(win);
}

void	RenderEngine::renderScene(Shader *shader) const {
	shader->use();
	renderGround(shader);
	renderWall(shader);
	renderBrick(shader);
}

void	RenderEngine::renderPlayer(IGameEntity const *player) const {
    std::vector<glm::mat4> data;
	
	glm::mat4 transform = glm::mat4();
	transform = glm::translate(transform, glm::vec3(player->getPosition(), 0.f));
	// transform = glm::rotate(transform, glm::radians(findHeadOrientation()), glm::vec3(0.f, 1.f, 0.f));
	data.push_back(transform);

    shader->use();
    playerModel->setInstanceBuffer(data);  
    playerModel->draw(shader, 2);
}

void	RenderEngine::renderGround(Shader *shader) const {
    std::vector<glm::mat4> data;
	
	for(float j = 0; j < 10 ; j++) {
		for(float i = 0; i < 10 ; i++) {
			glm::mat4 transform = glm::mat4();
			transform = glm::translate(transform, glm::vec3(i, j, 0.f));
			// transform = glm::rotate(transform, glm::radians(findHeadOrientation()), glm::vec3(0.f, 1.f, 0.f));
			data.push_back(transform);
		}
	}

	glm::vec3 camPos = camera.getPosition();
	shader->setVec3("viewPos", camPos.x, camPos.y, camPos.z);
    shader->setView();
	groundModel->setInstanceBuffer(data);
    groundModel->draw(shader, data.size());
}

void	RenderEngine::renderWall(Shader *shader) const {
    std::vector<glm::mat4> data;
	
	for(float i = -2; i < 11 ; i++) {
			glm::mat4 transform = glm::mat4();
			transform = glm::translate(transform, glm::vec3(-1, i, 0.f));
			data.push_back(transform);
			transform = glm::translate(transform, glm::vec3(11, 0.f, 0.f));
			data.push_back(transform);
	}
	for(float i = 0; i < 10 ; i++) {
			glm::mat4 transform = glm::mat4();
			transform = glm::translate(transform, glm::vec3(i, -2, 0.f));
			data.push_back(transform);
			transform = glm::translate(transform, glm::vec3(0.f, 12.f, 0.f));
			data.push_back(transform);
	}

	glm::vec3 camPos = camera.getPosition();
	shader->setVec3("viewPos", camPos.x, camPos.y, camPos.z);
    shader->setView();
	wallModel->setInstanceBuffer(data);
    wallModel->draw(shader, data.size());
}

void	RenderEngine::renderBrick(Shader *shader) const {
    std::vector<glm::mat4> data;
	
	glm::mat4 transform = glm::mat4();
	transform = glm::translate(transform, glm::vec3(5.f, 5.f, 2.f));
	data.push_back(transform);

	glm::vec3 camPos = camera.getPosition();
	shader->setVec3("viewPos", camPos.x, camPos.y, camPos.z);
    shader->setView();
	brickModel->setInstanceBuffer(data);  
    brickModel->draw(shader, data.size());
}

void	RenderEngine::setCamera(glm::mat4 const & cameraMatrix) {
	shader->use();
    shader->setCamera(cameraMatrix);
    shader->setView();
}

void	RenderEngine::createShadowBuffer() {
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
				 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}

void		RenderEngine::getShadowMap() {

	glm::mat4 lightProjection = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, 0.f, 100.f);
	
	glm::mat4 lightView = glm::lookAt(glm::vec3(20.0f, 20.0f, 20.0f), 
							glm::vec3( 5.0f, 5.0f,  0.0f), 
							glm::vec3( 0.0f, 0.0f,  1.0f));
	
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;
	
	shadowShader->use();
	glUniformMatrix4fv(glGetUniformLocation(shadowShader->getProgramID(), "lightSpaceMatrix"),
						1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
	textureShader->use();	
	glUniformMatrix4fv(glGetUniformLocation(textureShader->getProgramID(), "lightSpaceMatrix"),
						1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
	
	shadowShader->use();
	glViewport(0, 0, 1024, 1024);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	renderScene(shadowShader);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderEngine::renderShadowMap()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	
	debugDepthQuad->use();
	debugDepthQuad->setFloat("near_plane", 1.f);
	debugDepthQuad->setFloat("far_plane", 7.5f);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMap);

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}