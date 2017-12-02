/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Light.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thibautpierron <thibautpierron@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/30 09:38:27 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/02 22:02:28 by thibautpier      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIGHT_HPP
# define LIGHT_HPP

# include <glm/gtc/matrix_transform.hpp>
# include "Shader.hpp"
# include "Camera.hpp"
# include "Model.hpp"

class Light {
	public:
		enum type { DIRECTIONAL, POINT };

		Light(glm::vec3 position, glm::vec3 color, type t);
		~Light();

		glm::vec3	getPosition() const;
		void		setPosition(glm::vec3);
		glm::vec3	getColor() const;
		glm::mat4	getDirectionalLightSpaceMatrix() const;
		std::vector<glm::mat4>	getOmnidirectionalLightSpaceMatrix() const;
		void		render(Shader *shader, Camera const &camera) const;
		void		setShaderVariables(Shader *shader) const;

	private:
		Light();

		glm::vec3	position;
		glm::vec3	lookAt;
		glm::vec3	color;
		type		type;

		Model		*model;
};

#endif