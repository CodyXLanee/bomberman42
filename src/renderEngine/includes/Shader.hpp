/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Shader.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/23 11:44:27 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/22 10:55:56 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SHADER_CLASS_HPP
# define SHADER_CLASS_HPP

# include <iostream>
# include <fstream>
# include <OpenGL/gl3.h>
# include <glm/gtc/matrix_transform.hpp>
# include <glm/gtc/type_ptr.hpp>
# include "bomberman.hpp"

class Shader {

    public:
        Shader(std::string vertexSrc, std::string fragmentSrc);
        Shader(std::string vertexSrc, std::string geometrySrc, std::string fragmentSrc);
        ~Shader();

        Shader(Shader const & src);
        Shader&     operator=(Shader const & rhs);
        
        void            use() const;
        void            setModel(glm::mat4 model) const;
        void            setMat4(const std::string &name, glm::mat4 mat) const;
        void            setFloat(const std::string &name, float value) const;
        void            setInt(const std::string &name, float value) const;
        void            setBool(const std::string &name, bool value) const;
		void			setVec3(const std::string &name, float x, float y, float z) const;
        GLuint          getProgramID() const;
        static void     setCamera(glm::mat4);
        void            setView();
        void            setView(glm::mat4);
        glm::mat4       getViewMatrix();
        void            setOrthoView(float, float);

        void            setForFirstPass();
        bool            isUsedForFirstPass() const;
        
        static glm::mat4    perspective;
        
    private:
        Shader();
        GLuint      load(GLenum, std::string);
        void        compile();

        bool        checkShaderCompilation(GLuint, GLenum);
        bool        checkProgramCompilation(GLuint);

        static glm::mat4    camera;
        GLuint              vertexID;
        GLuint              fragmentID;
        GLuint              geometryID;
        GLuint              programID;
        std::string         vertexSrc;
        std::string         fragmentSrc;
        std::string         geometrySrc;
        bool                forFirstPass;
};

#endif
