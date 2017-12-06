/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ShaderManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/06 11:16:47 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/06 13:52:07 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ShaderManager.hpp"

ShaderManager::ShaderManager() {
	directory = "src/renderEngine/shaders/";

	mainShader = new Shader( directory + "static_model_instanced.glvs",
								 directory + "directionalLighting.glfs");
	// mainShader = new Shader( directory + "static_model_instanced.glvs",
	// 							 directory + "omnidirectionalLighting.glfs");
	flamesShader = new Shader( directory + "flames.glvs",
								 directory + "flames.glfs");
	directionalShadowShader = new Shader( directory + "directionalShadowDepth.glvs",
								 directory + "empty.glfs");
	pointShadowShader = new Shader( directory + "pointShadowDepth.glvs",
								 directory + "pointShadowDepth.glgs",
								 directory + "pointShadowDepth.glfs");
	particlesShader = new Shader( directory + "particles.glvs",
								 directory + "particles.glfs");	
	debugDepthQuad = new Shader( directory + "debugShadow.glvs",
								 directory + "debugShadow.glfs");
}

ShaderManager::~ShaderManager() {
	delete mainShader;
	delete flamesShader;
	delete directionalShadowShader;
	delete pointShadowShader;
	delete particlesShader;
	delete debugDepthQuad;
}

Shader	&ShaderManager::getMainShader() const {
	return *mainShader;
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
	flamesShader->use();
    flamesShader->setCamera(camMat);
    flamesShader->setView();
    particlesShader->use();
    particlesShader->setCamera(camMat);
    particlesShader->setView();
}