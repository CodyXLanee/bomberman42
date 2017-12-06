/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RenderEngine.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 16:35:00 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/06 11:14:08 by egaborea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RenderEngine.hpp"

RenderEngine::RenderEngine(SDL_Window *win, Camera & camera) : win(win), camera(camera) {

	int w, h;
	SDL_GetWindowSize(win, &w, &h);
	Shader::perspective = glm::perspective(glm::radians(FOV), static_cast<float>(w) / static_cast<float>(h), Z_NEAR, Z_FAR);

	createShadowBuffer();
	// createDepthCubemap();

	mainShader = new Shader("src/renderEngine/shaders/static_model_instanced.glvs",
								"src/renderEngine/shaders/directionalLighting.glfs");
	flamesShader = new Shader("src/renderEngine/shaders/flames.glvs",
								"src/renderEngine/shaders/flames.glfs");
	// mainShader = new Shader("src/renderEngine/shaders/static_model_instanced.glvs",
	// 							"src/renderEngine/shaders/omnidirectionalLighting.glfs");
	directionalShadowShader = new Shader("src/renderEngine/shaders/directionalShadowDepth.glvs",
								"src/renderEngine/shaders/empty.glfs");
	pointShadowShader = new Shader("src/renderEngine/shaders/pointShadowDepth.glvs",
								"src/renderEngine/shaders/pointShadowDepth.glgs",
								"src/renderEngine/shaders/pointShadowDepth.glfs");
	// debugDepthQuad = new Shader("src/renderEngine/shaders/debugShadow.glvs",
	// 							"src/renderEngine/shaders/debugShadow.glfs");

	particlesShader = new Shader("src/renderEngine/shaders/particles.glvs",
								"src/renderEngine/shaders/particles.glfs");
	
	groundModel = new Model("assets/models/obj/groundTile1.obj", false);
	wallModel = new Model("assets/models/obj/wall.obj", false);
	playerModel = new Model("assets/models/obj/player.obj", false);
	brickModel = new Model("assets/models/obj/brick.obj", false);
	bombModel = new Model("assets/models/obj/bomb.obj", false);
	flameModel = new Model("assets/models/obj/flame.obj", false);

	light = new Light(glm::vec3(8.f, 15.f, 20.f), glm::vec3(1.f, 0.941f, 0.713f), Light::DIRECTIONAL);
	// light = new Light(glm::vec3(20.f, 15.f, 11.f), glm::vec3(1.f, 0.941f, 0.713f), Light::DIRECTIONAL);
	// light = new Light(glm::vec3(3.5f, 6.5f, 1.f), glm::vec3(1.f, 1.f, 1.f), Light::DIRECTIONAL);

	meteo = new WeatherSystem();

	mainShader->use();
	mainShader->setInt("texture_diffuse", 0);
	mainShader->setInt("depthMap", 1);
	// debugDepthQuad->use();
	// debugDepthQuad->setInt("depthMap", 0);

	// particles.push_back(new ParticleSystem(glm::vec3(-2.f, -2.f, 10.f), ParticleSystem::RAIN));
}

RenderEngine::~RenderEngine() {}

void	RenderEngine::render(Map const & map, std::vector<IGameEntity *> & entities) {
	int	w, h;
	SDL_GetWindowSize(win, &w, &h);
	// recordNewEntities(entities);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mainShader->use();
	glDisable(GL_BLEND);
	getDirectionalShadowMap(map, entities);
	// getOmnidirectionalShadowMap(map, entities);

	glViewport(0, 0, w, h);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	// glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	renderScene(mainShader, map, entities);

	renderFlames(flamesShader, entities);

	// renderParticles();
	setCamera(camera.getMatrix(), particlesShader);
	meteo->renderRain(particlesShader);
	
	// light->render(mainShader, camera);
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
	// renderCloud(shader);
	meteo->renderCloud(shader);
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
	setCamera(camera.getMatrix(), shader);
	shader->use();
	shader->setInt("core", 1);
	glEnable(GL_BLEND);
	for (auto i = entities.begin(); i != entities.end(); i++ ){
		if ((*i)->getType() == Type::FLAME){
			glm::mat4 transform = glm::mat4();
			transform = glm::mat4(glm::translate(transform, glm::vec3((*i)->getPosition() + glm::vec2(0.5f, 0.5f) , 0.f))) * glm::scale(glm::vec3(flames_animation_scale(static_cast<Flame const *>(*i)) / 2));
			// transform = glm::rotate(transform,glm::radians(static_cast<float>(rand() % 360)), glm::vec3(0.f, 0.f, 1.f));
			data.insert(data.begin(), transform);
		}
	}
    flameModel->setInstanceBuffer(data);  
	flameModel->draw(shader, data.size());
	data.clear();
	shader->use();
	shader->setInt("core", 0);
	glEnable(GL_BLEND);
	for (auto i = entities.begin(); i != entities.end(); i++ ){
		if ((*i)->getType() == Type::FLAME){
			glm::mat4 transform = glm::mat4();
			transform = glm::mat4(glm::translate(transform, glm::vec3((*i)->getPosition() + glm::vec2(0.5f, 0.5f) , 0.f))) * glm::scale(glm::vec3(flames_animation_scale(static_cast<Flame const *>(*i))));
			// transform = glm::rotate(transform,glm::radians((rand() % 4) * 90.f), glm::vec3(0.f, 0.f, 1.f));
			data.insert(data.begin(), transform);
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

void	RenderEngine::createDepthCubemap() {
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	for (unsigned int i = 0; i < 6; ++i)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 
						SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void		RenderEngine::getDirectionalShadowMap(Map const & map, std::vector<IGameEntity *> &entities) {

	glm::mat4 lightSpaceMatrix = light->getDirectionalLightSpaceMatrix();
	
	directionalShadowShader->use();
	glUniformMatrix4fv(glGetUniformLocation(directionalShadowShader->getProgramID(), "lightSpaceMatrix"),
						1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
	mainShader->use();	
	glUniformMatrix4fv(glGetUniformLocation(mainShader->getProgramID(), "lightSpaceMatrix"),
						1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
	
	directionalShadowShader->use();
	glViewport(0, 0, 1024, 1024);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	renderScene(directionalShadowShader, map, entities);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void	RenderEngine::getOmnidirectionalShadowMap(Map const & map, std::vector<IGameEntity *> &entities) {

	std::vector<glm::mat4> shadowTransforms = light->getOmnidirectionalLightSpaceMatrix();

	glViewport(0, 0, 1024, 1024);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	pointShadowShader->use();
	for (unsigned int i = 0; i < 6; ++i)
		pointShadowShader->setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
	pointShadowShader->setFloat("far_plane", 25.f);
	glm::vec3 lightPos = light->getPosition();
	pointShadowShader->setVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);
	
	mainShader->use();
	mainShader->setFloat("far_plane", 25.f);
	mainShader->setVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);

    pointShadowShader->use();
			
	renderScene(pointShadowShader, map, entities);

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

void RenderEngine::renderParticles() const {
	// if (particles.size() == 0) {
	// 	particles.push_back(new ParticleSystem(glm::vec3(5.f, 5.f, 2.f), ParticleSystem::RAIN));
	// }

	// setCamera(camera.getMatrix(), particlesShader);
	// particles[0]->draw(particlesShader);
}

// void	RenderEngine::recordNewEntities(std::vector<IGameEntity *> & entities) {
// 	for(unsigned int it = 0; it < entities.size(); it++) {
// 		std::cout << &entities[it] << std::endl;
// 	}
// }