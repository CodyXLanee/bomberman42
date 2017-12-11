/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mesh.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 09:44:07 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/11 15:31:18 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Mesh.hpp"

int	Mesh::i = 0;

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
			std::vector<Texture> textures, aiColor3D color, const aiMesh *pMesh, const aiScene *scene)
: vertices(vertices), indices(indices), textures(textures), color(color), pMesh(pMesh), scene(scene) {
	bonesNbr = 0;
	offsetMatrices.resize(pMesh->mNumBones);
	// Mesh::i++;
	// this->rootJoint->calcInverseBindTransform(glm::mat4());
	// printJointMatrices(&rootJoint);
	setupMesh();
	setupBones();
	// if (pMesh->mNumBones != 0) {
	// 	for (unsigned int i = 0; i < vertices.size(); i++) {
	// 		std::cout << "bonesID: " << vertices[i].bonesID[0] << " : " << vertices[i].bonesID[1] << " : " << vertices[i].bonesID[2] << std::endl;
	// 		std::cout << "bonesWeigths: " << vertices[i].weigths[0] << " : " << vertices[i].weigths[1] << " : " << vertices[i].weigths[2] << std::endl;
	// 	}
	// }
	return;
}

Mesh::Mesh(Mesh const & src) {
	*this = src;
}

Mesh::~Mesh() {
	glDeleteBuffers(1, &this->vbo);
	glDeleteBuffers(1, &this->ebo);
	glDeleteBuffers(1, &this->ibo);
	glDeleteVertexArrays(1, &this->vao);
	for (unsigned int i = 0; i < this->textures.size(); i++) {
		glDeleteTextures(1, &textures[i].id);
	}
	// delete this->rootJoint;
	Mesh::i--;
	return;
}

void	Mesh::setupMesh() {
	glGenVertexArrays(1, &this->vao);
	glGenBuffers(1, &this->vbo);
	glGenBuffers(1, &this->ebo);

	glBindVertexArray(this->vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void*>(0));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

	glEnableVertexAttribArray(9);
	glVertexAttribIPointer(9, 3, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, bonesID));

	glEnableVertexAttribArray(10);
	glVertexAttribPointer(10, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, weigths));
	glBindVertexArray(0);

	// std::vector<glm::vec2>      data;
	// data.push_back(glm::vec2(2, 3));
	// data.push_back(glm::vec2(15, 20));

	// unsigned int vbo;
	// glGenBuffers(1, &vbo);
	// glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * data.size(), &data[0], GL_STATIC_DRAW);
	// glBindBuffer(GL_ARRAY_BUFFER, 0);


	// glEnableVertexAttribArray(5);
	// glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), static_cast<void*>(0));
	// glBindBuffer(GL_ARRAY_BUFFER, 0);	
	// glVertexAttribDivisor(5, 1); 


	return;
}

void	Mesh::setInstanceBuffer(std::vector<glm::mat4> const & data) {

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

void	Mesh::draw(Shader &shader, bool animated, unsigned int instanceCount) {

	// unsigned int diffuseNbr = 1;
	// unsigned int normalNbr = 1;
	for(unsigned int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		// std::cout << i << " :: " << this->textures[i].type << std::endl;
		// std::string name = this->textures[i].type;
		// std::string nbr = (name == "texture_diffuse") ? std::to_string(diffuseNbr++) : std::to_string(normalNbr++);
		// glUniform1i(glGetUniformLocation(shader.getProgramID(), ("material." + name + nbr).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);

	if (textures.size() == 0)
		glUniform3f(glGetUniformLocation(shader.getProgramID(), "materialColor"), this->color.r, this->color.g, this->color.b);

	if (animated) {
		// glm::mat4 *jointTransforms = getJointTransforms();
		// glUniformMatrix4fv(glGetUniformLocation(shader.getProgramID(), "jointTransforms"), 16, GL_FALSE, glm::value_ptr(*jointTransforms));
	}

	glBindVertexArray(this->vao);
	glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, instanceCount);
	glBindVertexArray(0);
	
	return;
}

void		Mesh::setupBones() {
	for(unsigned int i = 0; i < pMesh->mNumBones; i++) {
		unsigned int boneIndex = 0;
		std::string boneName(pMesh->mBones[i]->mName.data);

		if (bonesMap.find(boneName) == bonesMap.end()) {
			boneIndex = bonesNbr;
			bonesNbr++;
		} else
			boneIndex = bonesMap[boneName];

		bonesMap[boneName] = boneIndex;
		offsetMatrices[boneIndex] = asssimpToGlmMatrix(pMesh->mBones[i]->mOffsetMatrix);

		for(unsigned int j = 0; j < pMesh->mBones[i]->mNumWeights; j++) {
			unsigned int vertexID = pMesh->mBones[i]->mWeights[j].mVertexId;
			float weight = pMesh->mBones[i]->mWeights[j].mWeight;
			addBoneData(vertexID, boneIndex, weight);
		}
	}
}

void	Mesh::addBoneData(unsigned int vertexID, unsigned int boneID, float weight) {
	for (unsigned int i = 0; i < 3; i++) {
		if (vertices[vertexID].weigths[i] == 0) {
			vertices[vertexID].bonesID[i] = boneID;
			vertices[vertexID].weigths[i] = weight;
			return;
		}
	}
}

std::vector<glm::mat4>	Mesh::getTransforms(float timeInSeconds) {
	std::vector<glm::mat4> transforms;
	glm::mat4 identityMat = glm::mat4(1.0f);
	float ticksPerSecond = scene->mAnimations[0]->mTicksPerSecond;
	float timeInTicks = timeInSeconds * ticksPerSecond;
	float animationTime = fmod(timeInTicks, scene->mAnimations[0]->mDuration);

	readNodeHierarchy(animationTime, scene->mRootNode, identityMat);
}

void	Mesh::readNodeHierarchy(float animationTime, const aiNode *node, const glm::mat4 parentTransform) {
	std::string nodeName(node->mName.data);
	const aiAnimation *animation = scene->mAnimations[0];
	glm::mat4 nodeTransform = assimpToGlmMatrix(node->mTransformation);
	const aiNodeAnim *pNodeAnim = findNodeAnim(animation, nodeName);

	if(pNodeAnim) {
		aiVector3D scaling;
		calcInterpolatedScaling(scaling, animationTime, node);
	}
}


glm::mat4			Mesh::asssimpToGlmMatrix(aiMatrix4x4 ai) const {
		glm::mat4 mat;
		mat[0][0] = ai.a1; mat[1][0] = ai.a2; mat[2][0] = ai.a3; mat[3][0] = ai.a4;
		mat[0][1] = ai.b1; mat[1][1] = ai.b2; mat[2][1] = ai.b3; mat[3][1] = ai.b4;
		mat[0][2] = ai.c1; mat[1][2] = ai.c2; mat[2][2] = ai.c3; mat[3][2] = ai.c4;
		mat[0][3] = ai.d1; mat[1][3] = ai.d2; mat[2][3] = ai.d3; mat[3][3] = ai.d4;
		return mat;
}


// glm::mat4* Mesh::getJointTransforms() const {
// 	glm::mat4 *jointMatrices = new glm::mat4[this->jointNbr];
// 	this->addJointsToArray(this->rootJoint, jointMatrices);

// 	return jointMatrices;
// }

// void	Mesh::addJointsToArray(Joint *head, glm::mat4* jointMatrices) const {
// 	jointMatrices[head->index] = head->getFinalTransform();
// 	for (unsigned int i = 0; i < head->children.size(); i++) {
// 		addJointsToArray(head->children[i], jointMatrices);
// 	}
// }

// Joint *Mesh::getRootJoint() {
// 	return rootJoint;
// }