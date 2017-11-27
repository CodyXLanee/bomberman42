/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 10:26:08 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/27 14:31:11 by lfourque         ###   ########.fr       */
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
		glm::vec3	getPosition() const;

		void		update(std::vector<Action::Enum> const &, int const, int const);
		void		updatePosition(std::vector<Action::Enum> const &);
		void		updateRotation(int const, int const);

	private:
		Camera();

		void				setup();
		void				reset();
		
		glm::mat4			matrix;
		glm::vec3			position;
		glm::vec3			front;
		glm::vec3			const up;

		glm::vec3			initialPosition;
		float				initialYaw;
		float				initialPitch;

		float				speed;
		float				sensitivity;
		float				yaw;
		float				pitch;
};

#endif