/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RenderEngine.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 16:35:00 by tpierron          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2017/12/01 12:31:52 by egaborea         ###   ########.fr       */
=======
/*   Updated: 2017/12/01 09:24:57 by tpierron         ###   ########.fr       */
>>>>>>> 528117126af7ce4f75c7d5cf66cf82c37b5d7818
/*                                                                            */
/* ************************************************************************** */

#include "RenderEngine.hpp"
#include "Player.hpp"
#include "Flame.hpp"
#include "Bomb.hpp"
#include <cmath>
#include <glm/ext.hpp>
#include <glm/gtx/vector_angle.hpp>

RenderEngine::RenderEngine(SDL_Window *win, Camera & camera) : win(win), camera(camera), gui(win) {

	createShadowBuffer();

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
	bombModel = new Model("assets/models/obj/bomb.obj", false);
	flameModel = new Model("assets/models/obj/flame.obj", false);

	light = new Light(glm::vec3(20.f, 20.f, 20.f), glm::vec3(0.682f, 0.356f, 0.803f), Light::DIRECTIONAL);

	textureShader->use();
	textureShader->setInt("texture_diffuse", 0);
	textureShader->setInt("depthMap", 1);
	debugDepthQuad->use();
	debugDepthQuad->setInt("depthMap", 0);
}

RenderEngine::~RenderEngine() {}

void	RenderEngine::render(Map const & map, std::vector<IGameEntity *> & entities) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	getShadowMap(map, entities);

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	renderScene(textureShader, map, entities);

	renderFlames(textureShader, entities);
	
	// light->render(textureShader, camera);
	// renderShadowMap();

	// SDL_GL_SwapWindow(win);
}

void	RenderEngine::renderScene(Shader *shader, Map const & map, std::vector<IGameEntity *> &entities) const {
	shader->use();

	setCamera(camera.getMatrix(), shader);
	glm::vec3 camPos = camera.getPosition();
	shader->setVec3("viewPos", camPos.x, camPos.y, camPos.z);
	light->setShaderVariables(shader);
	
	renderGround(shader, map);
	renderWall(shader, map.getIndestructibleBlocs(), map);
	renderBrick(shader, map.getDestructibleBlocs(), map);
	renderPlayer(shader, entities);
	renderBombs(shader, entities);
}

void	RenderEngine::renderGUI(std::vector<Action::Enum> & actions) {
	gui.render(actions, camera);
}

void	RenderEngine::renderPlayer(Shader *shader, std::vector<IGameEntity *> const & entities) const {
    std::vector<glm::mat4> data;
	
	for (auto i = entities.begin(); i != entities.end(); i++ ){
		if ((*i)->getType() != Type::PLAYER)
			continue;
		glm::mat4 transform = glm::mat4();
		transform = glm::translate(transform, glm::vec3((*i)->getPosition() + glm::vec2(0.5f, 0.5f), 0.f));
		// transform = glm::scale(transform, glm::vec3(.4f, .4f, .4f));
		// transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(1.f, 0.f, 0.f));

		glm::vec2	graphicalDir = dynamic_cast<Player*>(*i)->getGraphicalDirection();
		if (graphicalDir.x < (*i)->getDirection().x)
			graphicalDir.x += 0.1;
		if (graphicalDir.x > (*i)->getDirection().x)
			graphicalDir.x -= 0.1;
		if (graphicalDir.y < (*i)->getDirection().y)
			graphicalDir.y += 0.1;
		if (graphicalDir.y > (*i)->getDirection().y)
			graphicalDir.y -= 0.1;
		dynamic_cast<Player*>(*i)->setGraphicalDirection(graphicalDir);

		int sign = (graphicalDir.x < 0) ? -1 : 1;
		transform = glm::rotate(transform, sign * angle(glm::vec2(0.f, -1.f), graphicalDir), glm::vec3(0.f, 0.f, 1.f));

		data.push_back(transform);
	}
    playerModel->setInstanceBuffer(data);  
    playerModel->draw(shader, 2);
}

void	RenderEngine::renderGround(Shader *shader, Map const & map) const {
    std::vector<glm::mat4> data;
	
	for(float j = 0; j < map.getSize().y ; j++) {
		for(float i = 0; i < map.getSize().x ; i++) {
			glm::mat4 transform = glm::mat4();
			transform = glm::translate(transform, glm::vec3(i, j, 0.f));
			// transform = glm::rotate(transform, glm::radians(180.f), glm::vec3(0.f, 1.f, 0.f));
			data.push_back(transform);
		}
	}
	groundModel->setInstanceBuffer(data);
    groundModel->draw(shader, data.size());
}

void	RenderEngine::renderWall(Shader *shader, const std::vector<IndestructibleBloc> &b, Map const & map) const {
    std::vector<glm::mat4> data;
	
	for (auto i = b.begin(); i != b.end(); i++){
		glm::mat4 transform = glm::mat4();
		transform = glm::mat4(glm::translate(transform, glm::vec3(i->getPosition(), 0.f)));
		data.push_back(transform);
	}

	for(float i = -1; i < map.getSize().y + 1 ; i++) {
			glm::mat4 transform = glm::mat4();
			transform = glm::translate(transform, glm::vec3(-1, i, 0.f));
			data.push_back(transform);
			transform = glm::translate(transform, glm::vec3(map.getSize().y + 1, 0.f, 0.f));
			data.push_back(transform);
	}
	
	for(float i = 0; i < map.getSize().y ; i++) {
			glm::mat4 transform = glm::mat4();
			transform = glm::translate(transform, glm::vec3(i, -1.f, 0.f));
			data.push_back(transform);
			transform = glm::translate(transform, glm::vec3(0.f, map.getSize().x + 1, 0.f));
			data.push_back(transform);
	}
	wallModel->setInstanceBuffer(data);
    wallModel->draw(shader, data.size());
}

void	RenderEngine::renderBrick(Shader *shader, const std::vector<DestructibleBloc> &blocs, Map const & map) const {
	(void)map;
    std::vector<glm::mat4> data;

	glm::mat4 transform;
	for (auto i = blocs.begin(); i != blocs.end(); i++){
		glm::mat4 transform = glm::mat4();
		transform = glm::mat4(glm::translate(transform, glm::vec3(i->getPosition(), 0.f)));
		data.push_back(transform);
	}
	brickModel->setInstanceBuffer(data);  
    brickModel->draw(shader, data.size());
}

static	float		bombs_animation_scale(Bomb const *b){
	std::chrono::milliseconds time_since_creation(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - b->get_creation_time()));
	float ratio = static_cast<float>(time_since_creation.count()) / static_cast<float>(b->get_ms_before_explode().count());
	return 1.f + cos(ratio * 17.f) * 0.05f;
}

void	RenderEngine::renderBombs(Shader *shader, std::vector<IGameEntity *> const & entities) const {
	std::vector<glm::mat4> data;
	for (auto i = entities.begin(); i != entities.end(); i++ ){
		if ((*i)->getType() == Type::BOMB){
			glm::mat4 transform = glm::mat4();
			transform = glm::mat4(glm::translate(transform, glm::vec3((*i)->getPosition() + glm::vec2(0.5f, 0.5f) , 0.f))) * glm::scale(glm::vec3(bombs_animation_scale(static_cast<Bomb const *>(*i))));
			data.push_back(transform);
		}
	}
    bombModel->setInstanceBuffer(data);  
    bombModel->draw(shader, data.size());
}

static	float		flames_animation_scale(Flame const *f){
	std::chrono::milliseconds time_since_creation(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - f->get_creation_time()));
	float ratio = static_cast<float>(time_since_creation.count()) / static_cast<float>(f->get_ms_before_explode().count());
	return ((std::pow(ratio, 0.10f) - std::pow(ratio, 10.f)) + cos(ratio * 6.f) * 0.15f) * 1.10f ;
}

void	RenderEngine::renderFlames(Shader *shader, std::vector<IGameEntity *> const & entities) const {
	std::vector<glm::mat4> data;
	for (auto i = entities.begin(); i != entities.end(); i++ ){
		if ((*i)->getType() == Type::FLAME){
			glm::mat4 transform = glm::mat4();
			transform = glm::mat4(glm::translate(transform, glm::vec3((*i)->getPosition() + glm::vec2(0.5f, 0.5f) , 0.f))) * glm::scale(glm::vec3(flames_animation_scale(static_cast<Flame const *>(*i))));
			data.push_back(transform);
		}
	}
    flameModel->setInstanceBuffer(data);  
    flameModel->draw(shader, data.size());
}

void	RenderEngine::setCamera(glm::mat4 const & cameraMatrix, Shader *shader) const {
	shader->use();
    shader->setCamera(cameraMatrix);
    shader->setView();
}

struct nk_context *	RenderEngine::getGUIContext() const { return gui.getContext(); }

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

void		RenderEngine::getShadowMap(Map const & map, std::vector<IGameEntity *> &entities) {

	glm::mat4 lightSpaceMatrix = light->getLightSpaceMatrix();
	
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
	renderScene(shadowShader, map, entities);
	
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
