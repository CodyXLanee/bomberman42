/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AParticleSystem.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/04 13:37:11 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/22 14:18:35 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARTICLESYSTEM_HPP
# define PARTICLESYSTEM_HPP

# include <glm/gtc/matrix_transform.hpp>
# include <OpenGL/gl3.h>
# include <vector>
# include "Shader.hpp"

class AParticleSystem {
	public:
		enum Type { FIRE, BOMB, RAIN };
		

		virtual void	draw(Shader &shader);
		bool	isRunning() const;
		Type 	getType() const;
		
	protected:
		AParticleSystem();
		virtual ~AParticleSystem();

		virtual void update(Shader &shader) = 0;
		void	createBuffers();
		void	setInstanceBuffer();

		bool					running;
		unsigned int 			particleNbr;
		Type					type;
		// glm::vec3	velocity;
		// glm::vec4	color;
		// float life;

		GLuint vbo;
		GLuint vao;
		GLuint ebo;
		GLuint ibo;
};

#endif