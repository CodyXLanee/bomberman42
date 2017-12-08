/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RenderEngine.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 16:35:00 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/08 10:33:22 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RenderEngine.hpp"

RenderEngine::RenderEngine(SDL_Window *win, Camera & camera) : win(win), camera(camera) {
	std::cout << "RENDFR" << std::endl;
	SDL_GetWindowSize(win, &w, &h);
	Shader::perspective = glm::perspective(glm::radians(FOV), static_cast<float>(w) / static_cast<float>(h), Z_NEAR, Z_FAR);

	createShadowBuffer();
	// createDepthCubemap();

	meteo = new WeatherSystem();
	SEventManager::getInstance().registerEvent(Event::AIPTR, MEMBER_CALLBACK(RenderEngine::setAiDebugPointer));
}

RenderEngine::~RenderEngine() {
	delete meteo;
}

void	RenderEngine::render(Map const & map, std::vector<IGameEntity *> & entities) {
	int	w, h;

	SDL_GetWindowSize(win, &w, &h);
	setFireLights(entities);
	// recordNewEntities(entities);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shaderManager.setCamera(camera.getMatrix());
	shaderManager.setLightingValues(meteo->getLightingValues());

	shadowPass(map, entities);
	normalPass(map, entities);
	blendedPass(entities);

	// SDL_GL_SwapWindow(win);
}

void	RenderEngine::shadowPass(Map const & map, std::vector<IGameEntity *> &entities) const {
	glDisable(GL_BLEND);

	glViewport(0, 0, 1024, 1024);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	getDirectionalShadowMap(map, entities);
	// getOmnidirectionalShadowMap(map, entities);
}

void	RenderEngine::normalPass(Map const & map, std::vector<IGameEntity *> &entities) const {
	glViewport(0, 0, w, h);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	// glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	renderScene(shaderManager.getMainShader(), map, entities);
}

void	RenderEngine::blendedPass(std::vector<IGameEntity *> &entities) const {
	glEnable(GL_BLEND);
	renderFlames(shaderManager.getFlamesShader(), entities);
	meteo->renderRain(shaderManager.getParticlesShader());
	// renderParticles();
}


void	RenderEngine::renderScene(Shader &shader, Map const & map, std::vector<IGameEntity *> &entities) const {
	shader.use();
	glm::vec3 camPos = camera.getPosition();
	shader.setVec3("viewPos", camPos.x, camPos.y, camPos.z);
	meteo->getSun().setShaderVariables(shader);
	
	renderGround(shader, map);
	renderWall(shader, map.getIndestructibleBlocs(), map);
	renderBrick(shader, map.getDestructibleBlocs(), map);
	renderPlayer(shader, entities);
	renderBombs(shader, entities);
	meteo->renderCloud(shader);

	renderAiDebug(shader);
}

void	RenderEngine::renderPlayer(Shader &shader, std::vector<IGameEntity *> const & entities) const {
    std::vector<glm::mat4> data;
	Model &model = modelManager.getModel(ModelManager::PLAYER);
	
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
    model.setInstanceBuffer(data);  
    model.draw(shader, data.size());
}

void	RenderEngine::renderGround(Shader &shader, Map const & map) const {
    std::vector<glm::mat4> data;
	Model &model = modelManager.getModel(ModelManager::GROUND);

	for(float j = 0; j < map.getSize().y ; j++) {
		for(float i = 0; i < map.getSize().x ; i++) {
			glm::mat4 transform = glm::mat4();
			transform = glm::translate(transform, glm::vec3(i, j, 0.f));
			// transform = glm::rotate(transform, glm::radians(180.f), glm::vec3(0.f, 1.f, 0.f));
			data.push_back(transform);
		}
	}
	model.setInstanceBuffer(data);
    model.draw(shader, data.size());
}

void	RenderEngine::renderWall(Shader &shader, const std::vector<IndestructibleBloc> &b, Map const & map) const {
    std::vector<glm::mat4> data;
	Model &model = modelManager.getModel(ModelManager::WALL);
	
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
	model.setInstanceBuffer(data);
    model.draw(shader, data.size());
}

void	RenderEngine::renderBrick(Shader &shader, const std::vector<DestructibleBloc> &blocs, Map const & map) const {
	(void)map; /////////////////////
    std::vector<glm::mat4> data;
	Model &model = modelManager.getModel(ModelManager::BRICK);

	glm::mat4 transform;
	for (auto i = blocs.begin(); i != blocs.end(); i++){
		glm::mat4 transform = glm::mat4();
		transform = glm::mat4(glm::translate(transform, glm::vec3(i->getPosition(), 0.f)));
		data.push_back(transform);
	}
	model.setInstanceBuffer(data);  
    model.draw(shader, data.size());
}

static	float		bombs_animation_scale(Bomb const *b){
	std::chrono::milliseconds time_since_creation(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - b->get_creation_time()));
	float ratio = static_cast<float>(time_since_creation.count()) / static_cast<float>(b->get_ms_before_explode().count());
	return 1.f + cos(ratio * 17.f) * 0.05f;
}

void	RenderEngine::renderBombs(Shader &shader, std::vector<IGameEntity *> const & entities) const {
	std::vector<glm::mat4> data;
	Model &model = modelManager.getModel(ModelManager::BOMB);
	for (auto i = entities.begin(); i != entities.end(); i++ ){
		if ((*i)->getType() == Type::BOMB){
			glm::mat4 transform = glm::mat4();
			transform = glm::mat4(glm::translate(transform, glm::vec3((*i)->getPosition() + glm::vec2(0.5f, 0.5f) , 0.f))) * glm::scale(glm::vec3(bombs_animation_scale(static_cast<Bomb const *>(*i))));
			data.push_back(transform);
		}
	}
    model.setInstanceBuffer(data);  
    model.draw(shader, data.size());
}

static	float		flames_animation_scale(Flame const *f){
	std::chrono::milliseconds time_since_creation(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - f->get_creation_time()));
	float ratio = static_cast<float>(time_since_creation.count()) / static_cast<float>(f->get_ms_before_explode().count());
	return ((std::pow(ratio, 0.10f) - std::pow(ratio, 10.f)) + cos(ratio * 6.f) * 0.15f) * 1.10f ;
}

void	RenderEngine::renderFlames(Shader &shader, std::vector<IGameEntity *> const & entities) const {
	std::vector<glm::mat4> data;
	Model &model = modelManager.getModel(ModelManager::FLAME);
	
	shader.use();
	shader.setInt("core", 1);
	for (auto i = entities.begin(); i != entities.end(); i++ ){
		if ((*i)->getType() == Type::FLAME){
			glm::mat4 transform = glm::mat4();
			transform = glm::mat4(glm::translate(transform, glm::vec3((*i)->getPosition() + glm::vec2(0.5f, 0.5f) , 0.f))) * glm::scale(glm::vec3(flames_animation_scale(static_cast<Flame const *>(*i)) / 2));
			// transform = glm::rotate(transform,glm::radians(static_cast<float>(rand() % 360)), glm::vec3(0.f, 0.f, 1.f));
			data.insert(data.begin(), transform);
		}
	}
    model.setInstanceBuffer(data);  
	model.draw(shader, data.size());

	data.clear();
	
	shader.use();
	shader.setInt("core", 0);
	for (auto i = entities.begin(); i != entities.end(); i++ ){
		if ((*i)->getType() == Type::FLAME){
			glm::mat4 transform = glm::mat4();
			transform = glm::mat4(glm::translate(transform, glm::vec3((*i)->getPosition() + glm::vec2(0.5f, 0.5f) , 0.f))) * glm::scale(glm::vec3(flames_animation_scale(static_cast<Flame const *>(*i))));
			// transform = glm::rotate(transform,glm::radians((rand() % 4) * 90.f), glm::vec3(0.f, 0.f, 1.f));
			data.insert(data.begin(), transform);
		}
	}
    model.setInstanceBuffer(data);  
    model.draw(shader, data.size());
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


void		RenderEngine::getDirectionalShadowMap(Map const & map, std::vector<IGameEntity *> &entities) const {
	glm::mat4 lightSpaceMatrix = meteo->getSun().getDirectionalLightSpaceMatrix();
	
	shaderManager.getDirectionalShadowShader().use();
	glUniformMatrix4fv(glGetUniformLocation(shaderManager.getDirectionalShadowShader().getProgramID(), "lightSpaceMatrix"),
						1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
	shaderManager.getMainShader().use();	
	glUniformMatrix4fv(glGetUniformLocation(shaderManager.getMainShader().getProgramID(), "lightSpaceMatrix"),
						1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
	
	shaderManager.getDirectionalShadowShader().use();
	renderScene(shaderManager.getDirectionalShadowShader(), map, entities);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void	RenderEngine::getOmnidirectionalShadowMap(Map const & map, std::vector<IGameEntity *> &entities) const {
	std::vector<glm::mat4> shadowTransforms = light->getOmnidirectionalLightSpaceMatrix();

	shaderManager.getPointShadowShader().use();
	for (unsigned int i = 0; i < 6; ++i)
		shaderManager.getPointShadowShader().setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
	shaderManager.getPointShadowShader().setFloat("far_plane", 25.f);
	glm::vec3 lightPos = light->getPosition();
	shaderManager.getPointShadowShader().setVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);
	
	shaderManager.getMainShader().use();
	shaderManager.getMainShader().setFloat("far_plane", 25.f);
	shaderManager.getMainShader().setVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);

    shaderManager.getPointShadowShader().use();
			
	renderScene(shaderManager.getPointShadowShader(), map, entities);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void	RenderEngine::setFireLights(std::vector<IGameEntity *> const & entities) {
	fireLights.clear();
	for (auto it = entities.begin(); it != entities.end(); it++) {
		if((*it)->getType() == Type::FLAME)
			fireLights.push_back(glm::vec3((*it)->getPosition(), 1.f));
	}
	shaderManager.getMainShader().use();
	shaderManager.getMainShader().setInt("fireLightNbr", fireLights.size());
	for (unsigned int i = 0; i < fireLights.size(); i++)
		shaderManager.getMainShader().setVec3("fireLightPos[" + std::to_string(i) + "]",
									fireLights[i].x, fireLights[i].y, fireLights[i].z);
}

void	RenderEngine::renderParticles() const {
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

void	RenderEngine::setAiDebugPointer(void* ptr) {
	aiDebugInfo = static_cast<std::vector<glm::vec2> *>(ptr);
}

void	RenderEngine::renderAiDebug(Shader &shader) const {
	if(aiDebugInfo == nullptr || aiDebugInfo->size() == 0)
		return;
	std::vector<glm::mat4> data;
	Model &model = modelManager.getModel(ModelManager::AIDEBUG);

	for (auto it = aiDebugInfo->begin(); it != aiDebugInfo->end(); it++) {
			glm::mat4 transform = glm::mat4();
			transform = glm::translate(transform, glm::vec3((*it).x + 0.4f, (*it).y + 0.5f, 1.f));
			// transform = glm::rotate(transform,glm::radians((rand() % 4) * 90.f), glm::vec3(0.f, 0.f, 1.f));
			data.push_back(transform);
	}
    model.setInstanceBuffer(data);  
    model.draw(shader, data.size());
}