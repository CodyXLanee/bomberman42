/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ShaderManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/06 11:16:47 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/11 10:22:54 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ShaderManager.hpp"

ShaderManager::ShaderManager() {
	directory = "src/renderEngine/shaders/";

	mainShader = new Shader( directory + "static_model_instanced.glvs",
								 directory + "directionalLighting.glfs");
	// mainShader = new Shader( directory + "static_model_instanced.glvs",
	// 							 directory + "omnidirectionalLighting.glfs");
	animatedShader = new Shader( directory + "animated_model_instanced.glvs",
								 directory + "directionalLighting.glfs");
	flamesShader = new Shader( directory + "flames.glvs",
								 directory + "flames.glfs");
	directionalShadowShader = new Shader( directory + "directionalShadowDepth.glvs",
								 directory + "empty.glfs");
	pointShadowShader = new Shader( directory + "pointShadowDepth.glvs",
								 directory + "pointShadowDepth.glgs",
								 directory + "pointShadowDepth.glfs");
	particlesShader = new Shader( directory + "particles.glvs",
								 directory + "particles.glfs");	

	mainShader->use();
	mainShader->setInt("texture_diffuse", 0);
	mainShader->setInt("texture_normal", 1);
	mainShader->setInt("depthMap", 2);
}

ShaderManager::~ShaderManager() {
	delete mainShader;
	delete animatedShader;
	delete flamesShader;
	delete directionalShadowShader;
	delete pointShadowShader;
	delete particlesShader;
}

Shader	&ShaderManager::getMainShader() const {
	return *mainShader;
}

Shader	&ShaderManager::getAnimatedShader() const {
	return *animatedShader;
}

Shader	&ShaderManager::getDirectionalShadowShader() const {
	return *directionalShadowShader;
}

Shader	&ShaderManager::getPointShadowShader() const {
	return *pointShadowShader;
}

Shader	&ShaderManager::getFlamesShader() const {
	return *flamesShader;
}

Shader	&ShaderManager::getParticlesShader() const {
	return *particlesShader;
}

void	ShaderManager::setCamera(glm::mat4 const & camMat) const {
	mainShader->use();
    mainShader->setCamera(camMat);
    mainShader->setView();
	animatedShader->use();
    animatedShader->setCamera(camMat);
    animatedShader->setView();
	flamesShader->use();
    flamesShader->setCamera(camMat);
    flamesShader->setView();
    particlesShader->use();
    particlesShader->setCamera(camMat);
    particlesShader->setView();
}

void	ShaderManager::setLightingValues(glm::vec3 const &values) const {
	mainShader->use();
	mainShader->setVec3("lightingValues", values.x, values.y, values.z);
	animatedShader->use();
	animatedShader->setVec3("lightingValues", values.x, values.y, values.z);
}