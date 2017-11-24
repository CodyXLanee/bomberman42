/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 10:26:08 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/24 15:58:38 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_HPP
# define CAMERA_HPP

# include "bomberman.hpp"
# include <glm/gtc/matrix_transform.hpp>
# include <glm/gtx/rotate_vector.hpp>
# include <glm/gtx/matrix_interpolation.hpp>
# include <iostream>
# include <vector>

class Camera {
	public:
		Camera(glm::vec3, glm::vec3);

		glm::mat4	getMatrix() const;

		void		update(std::vector<Action::Enum> const &);

	private:
		Camera();

		void				setup();
		
		glm::mat4			matrix;
		glm::vec3			position;
		glm::vec3			lookAt;
		glm::vec3			const up;

};

#endif