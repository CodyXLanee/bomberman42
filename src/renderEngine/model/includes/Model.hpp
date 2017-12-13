/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Model.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 09:43:44 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/13 14:11:21 by tpierron         ###   ########.fr       */
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

class Model {
	public:
		Model(std::string path);
		~Model();

		void	draw(Shader &shader, unsigned int instanceCount);
		void	setInstanceBuffer(std::vector<glm::mat4> const &);
		std::vector<glm::mat4>		getBonesTransforms(float timeInSeconds) const;
		
	private:
		Model();
		void						loadModel(std::string path);

		std::vector<Vertex>			loadVertices(aiMesh *mesh);
		std::vector<unsigned int>	loadIndices(aiMesh *mesh);
		std::vector<Texture>		loadMaterials(aiMesh *mesh, const aiScene *scene);

		void						processNode(aiNode *node, const aiScene *scene);
	
		std::vector<Texture>		loadTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
		unsigned int				textureFromFile(const char* path, const std::string &directory);

		std::string							path;
		std::vector<Mesh*>					meshes;
		std::string 						directory;
		std::vector<Texture>    			texturesLoaded;

		static int i;
};

#endif