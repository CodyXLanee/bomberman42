/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mesh.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 09:43:34 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/11 14:11:53 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESH_HPP
# define MESH_HPP

# include <vector>
# include <map>
# include <glm/vec3.hpp>
# include <glm/vec2.hpp>
// # include <iostream>
// # include <fstream>
# include <assimp/Importer.hpp>
# include <assimp/scene.h>
# include <OpenGL/gl3.h>

# include "Shader.hpp" 
// # include "Joint.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tangent;
    glm::vec3 bitangent;

	int		bonesID[3];
	float	weigths[3];

	Vertex() {for (unsigned int i = 0; i < 3;i++){bonesID[i] = 0; weigths[i] = 0;}};
};

struct VertexBoneData {
    unsigned int IDs[3];
    float        weigths[3];
	VertexBoneData() {for (unsigned int i = 0; i < 3;i++){IDs[i] = 0; weigths[i] = 0;}};
};

struct Texture {
    unsigned int id;
    std::string type;
    aiString path;
};

class Mesh {
    public:
        Mesh(std::vector<Vertex>, std::vector<unsigned int>,
                std::vector<Texture>, aiColor3D color, const aiMesh *pMesh);
        Mesh(Mesh const & src);
        ~Mesh();
    

		void	draw(Shader &shader, bool animated, unsigned int instanceCount);
        void    setInstanceBuffer(std::vector<glm::mat4> const &);

    private:
        Mesh();
		void	    setupMesh();
        void        setupBones();
        void        addBoneData(unsigned int vertexID, unsigned int boneID, float weight);
        glm::mat4	asssimpToGlmMatrix(aiMatrix4x4 ai) const;

        std::vector<Vertex>     vertices;
        std::vector<unsigned int>     indices;
        std::vector<Texture>    textures;
        aiColor3D               color;

        std::map<std::string, unsigned int> bonesMap;
        std::vector<glm::mat4> offsetMatrices;

        const aiMesh    *pMesh;

        unsigned int            bonesNbr;
        unsigned int            vao;
        unsigned int            vbo;
        unsigned int            ebo;
        unsigned int            ibo;

        static int i;
};

#endif