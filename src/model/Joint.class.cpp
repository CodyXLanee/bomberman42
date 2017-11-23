/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Joint.class.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thibautpierron <thibautpierron@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/27 09:16:40 by tpierron          #+#    #+#             */
/*   Updated: 2017/09/05 17:11:13 by thibautpier      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Joint.class.hpp"

Joint::Joint(unsigned int index, std::string name, glm::mat4 bindLocalTransform) :
	index(index), name(name), localBindTransform(bindLocalTransform) {
		// std::cout << "Joint created, index: "<< index << " name: " << name << std::endl;
	return;
}

Joint::~Joint() {
	return;
}

void	Joint::addChild(Joint *child) {
	this->children.push_back(child);
}

glm::mat4 Joint::getAnimatedTransform() const {
	return this->animatedTransform;
}

void Joint::setAnimatedTransform(glm::mat4 animationTransform) {
	this->animatedTransform = animationTransform;
}

glm::mat4 Joint::getFinalTransform() const {
	return this->finalTransform;
}

void Joint::setFinalTransform(glm::mat4 finalTransform) {
	this->finalTransform = finalTransform;
}

glm::mat4 Joint::getInverseBindTransform() const {
	return this->inverseBindTransform;
}

void Joint::setInverseBindTransform(glm::mat4 inverseBindTransform) {
	this->inverseBindTransform = inverseBindTransform;
}

void Joint::calcInverseBindTransform(glm::mat4 parentBindTransform) {
	glm::mat4 bindTransform = parentBindTransform * localBindTransform;
	this->inverseBindTransform = glm::inverse(bindTransform);
	// this->inverseBindTransform = bindTransform;
	for (unsigned int i = 0; i < this->children.size(); i++) {
		children[i]->calcInverseBindTransform(bindTransform);
	}
}