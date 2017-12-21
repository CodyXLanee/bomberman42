/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Model.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 09:43:44 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/21 14:59:34 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODEL_HPP
# define MODEL_HPP

# include <vector>
# include <map>

# include <assimp/Importer.hpp>
# include <assimp/scene.h>
# include <assimp/postprocess.h>

# include "Mesh.hpp"

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

	Material() {ambient = glm::vec3(1.f); diffuse = glm::vec3(1.f); specular = glm::vec3(1.f);};
};

class Model {
	public:
		Model(std::string path);
		~Model();

		void						draw(Shader &shader, std::vector<glm::mat4> const & transforms);
		std::vector<glm::mat4>		getBonesTransforms(float timeInSeconds) const;
		bool						isAnimated() const;
		void						setAnimation(unsigned int animation, float timeInSeconds);
		void						setUnique();
		
	private:
		Model();
		void						loadModel(std::string path);

		std::vector<Vertex>			loadVertices(aiMesh *mesh);
		std::vector<unsigned int>	loadIndices(aiMesh *mesh);
		std::vector<Texture>		loadMaterials(aiMesh *mesh, const aiScene *scene);

		void						processNode(aiNode *node, const aiScene *scene);
	
		std::vector<Texture>		loadTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
		unsigned int				textureFromFile(const char* path, const std::string &directory);

		std::string					path;
		std::vector<Mesh*>			meshes;
		std::string 				directory;
		std::vector<Texture>    	texturesLoaded;
		Material					material;
		bool						animated;
		bool						hasBumpMap;
		bool						hasSpecularMap;
		bool						isUnique;

		static int i;
};

#endif