/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Model.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 09:44:16 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/21 15:10:31 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Model.hpp"
# define STB_IMAGE_IMPLEMENTATION
# include <stb_image.h>

int	Model::i = 0;

Model::Model(std::string path) : path(path) {
	hasBumpMap = false;
	hasSpecularMap = false;
	isUnique = false;
	loadModel(path);
	Model::i++;
	return;
}

Model::~Model() {
	for (unsigned int i = 0; i < this->meshes.size(); i++) {
		delete this->meshes[i];
	}
	Model::i--;
	return;
}

void	Model::loadModel(std::string path) {
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "Model loading error: " << importer.GetErrorString() << std::endl;
		return;
	}
	animated = scene->HasAnimations();
	// std::cout << scene->mNumAnimations << std::endl;
	this->directory = path.substr(0, path.find_last_of('/'));

	aiNode *node = scene->mRootNode;
	
	processNode(node, scene);
}

void	Model::processNode(aiNode *node, const aiScene *scene) {

	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		std::vector<Vertex>	vertices = loadVertices(mesh);
		std::vector<unsigned int> indices = loadIndices(mesh);
		std::vector<Texture> materials = loadMaterials(mesh, scene);

		aiColor3D color(0.f,0.f,0.f);
		scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_AMBIENT,color);
		material.ambient = glm::vec3(color.r, color.g, color.b);
		scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE,color);
		material.diffuse = glm::vec3(color.r, color.g, color.b);
		scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_SPECULAR,color);
		material.specular = glm::vec3(color.r, color.g, color.b);


		// aiColor3D color(0.f,0.f,0.f);
		// if (materials.size() == 0)
		// 	scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE,color);

		this->meshes.push_back(new Mesh(vertices, indices, materials, color, mesh, scene, path));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

std::vector<Vertex> Model::loadVertices(aiMesh *mesh) {
	std::vector<Vertex>			vertices;

	for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		glm::vec3 posVector;
		glm::vec3 normalVector;
		glm::vec2 textVector;
		glm::vec3 tangentVector;
		glm::vec3 bitangentVector;

		posVector.x = mesh->mVertices[i].x;
		posVector.y = mesh->mVertices[i].y;
		posVector.z = mesh->mVertices[i].z;

		if(mesh->HasNormals()) {
			normalVector.x = mesh->mNormals[i].x;
			normalVector.y = mesh->mNormals[i].y;
			normalVector.z = mesh->mNormals[i].z;
		} else {
			normalVector.x = 0;
			normalVector.y = 0;
			normalVector.z = 0;
		}

		if (mesh->HasTangentsAndBitangents()) {
			tangentVector.x = mesh->mTangents[i].x;
			tangentVector.y = mesh->mTangents[i].y;
			tangentVector.z = mesh->mTangents[i].z;
			tangentVector.x = mesh->mBitangents[i].x;
			tangentVector.y = mesh->mBitangents[i].y;
			tangentVector.z = mesh->mBitangents[i].z;
		} else {
			tangentVector.x = 0;
			tangentVector.y = 0;
			tangentVector.z = 0;
			bitangentVector.x = 0;
			bitangentVector.y = 0;
			bitangentVector.z = 0;
		}

		if(mesh->mTextureCoords[0]) {
			textVector.x = mesh->mTextureCoords[0][i].x;
			textVector.y = mesh->mTextureCoords[0][i].y;
		} else {
			textVector.x = 0;
			textVector.y = 0;
		}

		vertex.position = posVector;
		vertex.normal = normalVector;
		vertex.texCoords = textVector;
		vertex.tangent = tangentVector;
		vertex.bitangent = bitangentVector;

		vertices.push_back(vertex);
	}

	std::vector<float> boneWeights;
	boneWeights.resize(mesh->mNumVertices * 3);
	for (unsigned int i = 0; i < mesh->mNumBones; i++) {
		aiBone *bone = mesh->mBones[i];
		for (unsigned int j = 0; j < bone->mNumWeights; j++) {
			aiVertexWeight weight = bone->mWeights[j];
			unsigned int vertexStart = weight.mVertexId * 3;
			for (unsigned int k = 0; k < 3; k++) {
				if (boneWeights[vertexStart + k] == 0) {
					boneWeights[vertexStart + k] = weight.mWeight;
					vertices[weight.mVertexId].weigths[k] = weight.mWeight;
					vertices[weight.mVertexId].bonesID[k] = i;
					break;
				}
			}
		}
	}
	
	return vertices;
}

std::vector<unsigned int>	Model::loadIndices(aiMesh *mesh) {
	std::vector<unsigned int>	indices;

	unsigned int n1 = 0;
	unsigned int n2 = 0;
	for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for(unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
			n1 = j;
		}
		n2 = i;
	}

	return indices;
}

std::vector<Texture>	Model::loadMaterials(aiMesh *mesh, const aiScene *scene) {

	std::vector<Texture>		textures;

	aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
	std::vector<Texture> diffuseMaps = loadTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	std::vector<Texture> specularMaps = loadTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	std::vector<Texture> normalMaps = loadTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

	return textures;
}

std::vector<Texture> Model::loadTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
	std::vector<Texture> textures;
	for(unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);

		bool skip = false;
		for(unsigned int j = 0; j < this->texturesLoaded.size(); j++) {
			if (std::strcmp(texturesLoaded[j].path.C_Str(), str.C_Str()) == 0) {
				textures.push_back(texturesLoaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip) {
			Texture texture;
			texture.id = this->textureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
			texturesLoaded.push_back(texture);
			if (type == aiTextureType_HEIGHT)
				hasBumpMap = true;
			if (type == aiTextureType_SPECULAR)
				hasSpecularMap = true;
		}
	}
	return textures;
}

unsigned int		Model::textureFromFile(const char* path, const std::string &directory) {
	std::string filename = std::string(path);
	filename = directory + "/" + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, componentNbr;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &componentNbr, 0);
	if(data) {
		GLenum format;
		switch(componentNbr) {
			case 1: format = GL_RED; break;
			case 3: format = GL_RGB; break;
			case 4: format = GL_RGBA; break;
		}
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "Loading texture failed: " << filename << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}

void	Model::draw(Shader &shader, std::vector<glm::mat4> const & transforms) {
		// meshes[0]->draw(shader, transforms);
	shader.use();
	shader.setInt("hasBumpMap", hasBumpMap);
	shader.setInt("hasSpecularMap", hasSpecularMap);
	
	/////////////
	if (path == "assets/models/obj/player.fbx") {
		material.ambient = glm::vec3(1.f);
		material.diffuse = glm::vec3(1.f);
		material.specular = glm::vec3(1.f);
	}
	///////////////

	shader.setVec3("materialAmbient", material.ambient.x, material.ambient.y, material.ambient.z);
	shader.setVec3("materialDiffuse", material.diffuse.x, material.diffuse.y, material.diffuse.z);
	shader.setVec3("materialSpecular", material.specular.x, material.specular.y, material.specular.z);

	for(unsigned int i = 0; i < this->meshes.size(); i++) {
		meshes[i]->draw(shader, transforms);
	}
}

bool	Model::isAnimated() const {
	return animated;
}

void	Model::setAnimation(unsigned int animation, float timeInSeconds) {
	if (animated) {
		for (unsigned int i = 0; i < meshes.size(); i++) {
			meshes[i]->setAnimation(animation, timeInSeconds);
		}
	}
}

void	Model::setUnique() {
	isUnique = true;
	for(unsigned int i = 0; i < this->meshes.size(); i++) {
		meshes[i]->setUnique();
	}
}