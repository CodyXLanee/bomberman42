/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RenderEngine.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 16:35:00 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/28 14:51:15 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RenderEngine.hpp"
#include <glm/ext.hpp>

RenderEngine::RenderEngine(SDL_Window *win, Camera & camera) : win(win), camera(camera), gui(win) {
	shader = new Shader("src/renderEngine/shaders/static_model_instanced.glvs",
						"src/renderEngine/shaders/simple_diffuse.glfs");
	textureShader = new Shader("src/renderEngine/shaders/static_model_instanced.glvs",
						"src/renderEngine/shaders/diffuse_texture.glfs");
						
	groundModel = new Model("assets/models/obj/groundTile1.obj", false);
	wallModel = new Model("assets/models/obj/wall.obj", false);
	playerModel = new Model("assets/models/obj/player.obj", false);
	brickModel = new Model("assets/models/obj/brick.obj", false);

}

RenderEngine::~RenderEngine() {}

void	RenderEngine::render(Map const & map, std::vector<IGameEntity *> const & entities) {
	(void)map; //////////
	(void)entities; ////////////
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	setCamera(camera.getMatrix());
	renderMap(map);
	for (std::vector<IGameEntity const *>::const_iterator i = entities.begin(); i != entities.end(); i++ ){
		if ((*i)->getType() == Type::PLAYER)
			renderPlayer(*i);
	}	
}

void	RenderEngine::renderGUI(std::vector<Action::Enum> const & actions) {
	gui.render(actions, camera);
}

void	RenderEngine::renderMap(Map const & map) const {
	renderGround();
	renderWall(map.getIndestructibleBlocs());
	renderBrick(map.getDestructibleBlocs());
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

void	RenderEngine::renderGround() const {
    std::vector<glm::mat4> data;
	
	for(float j = 0; j < 10 ; j++) {
		for(float i = 0; i < 10 ; i++) {
			glm::mat4 transform = glm::mat4();
			transform = glm::translate(transform, glm::vec3(i, j, 0.f));
			// transform = glm::rotate(transform, glm::radians(findHeadOrientation()), glm::vec3(0.f, 1.f, 0.f));
			data.push_back(transform);
		}
	}

	textureShader->use();
	glm::vec3 camPos = camera.getPosition();
	textureShader->setVec3("viewPos", camPos.x, camPos.y, camPos.z);
    textureShader->setView();
    groundModel->setInstanceBuffer(data);  
    groundModel->draw(textureShader, data.size());
}

void	RenderEngine::renderWall(const std::vector<IndestructibleBloc> &b) const {
    std::vector<glm::mat4> data;
	
	for (auto i = b.begin(); i != b.end(); i++){
		glm::mat4 transform = glm::mat4();
		transform = glm::mat4(glm::translate(transform, glm::vec3(i->getPosition(), 0.f)));
		data.push_back(transform);
	}

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

	textureShader->use();
	glm::vec3 camPos = camera.getPosition();
	textureShader->setVec3("viewPos", camPos.x, camPos.y, camPos.z);
    textureShader->setView();
    wallModel->setInstanceBuffer(data);  
    wallModel->draw(textureShader, data.size());
}

void	RenderEngine::renderBrick(const std::vector<DestructibleBloc> &blocs) const {
    std::vector<glm::mat4> data;

	glm::mat4 transform;
	for (auto i = blocs.begin(); i != blocs.end(); i++){
		glm::mat4 transform = glm::mat4();
		transform = glm::mat4(glm::translate(transform, glm::vec3(i->getPosition(), 0.f)));
		data.push_back(transform);
	}

	textureShader->use();
	glm::vec3 camPos = camera.getPosition();
	textureShader->setVec3("viewPos", camPos.x, camPos.y, camPos.z);
    textureShader->setView();
    brickModel->setInstanceBuffer(data);  
    brickModel->draw(textureShader, data.size());
}

void	RenderEngine::setCamera(glm::mat4 const & cameraMatrix) {
	shader->use();
    shader->setCamera(cameraMatrix);
    shader->setView();
}

struct nk_context *	RenderEngine::getGUIContext() const { return gui.getContext(); }