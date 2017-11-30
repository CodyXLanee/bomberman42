/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 10:26:08 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/29 22:36:16 by egaborea         ###   ########.fr       */
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
		glm::vec3   getFront() const;

		void		update(std::vector<Action::Enum> const &, int const, int const, glm::vec2 const *);
		void		updatePosition(std::vector<Action::Enum> const &);
		void		updateRotation(int const, int const);

	private:
		Camera();
		void    			followPlayer(glm::vec2 const * playerPos);
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