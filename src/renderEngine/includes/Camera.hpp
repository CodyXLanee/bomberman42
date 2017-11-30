/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egaborea <egaborea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 10:26:08 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/30 11:14:54 by egaborea         ###   ########.fr       */
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

		void		update(std::vector<Action::Enum> &, int const, int const, glm::vec2 const *);
		void		updatePosition(std::vector<Action::Enum> const &);
		void		updateRotation(int const, int const);

	private:
		Camera();
		void    			updateFront(void);
		void    			followPlayer(glm::vec2 const *, std::vector<Action::Enum> &);
		void				setup();
		void				reset();
		void				wiggle(void);
		void				initWiggle(void);
		
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

		bool						is_wiggling;
		float						wiggle_yaw;
		float						wiggle_pitch;

		std::chrono::milliseconds								wiggle_duration;
		std::chrono::time_point<std::chrono::steady_clock>		wiggle_start;
};

#endif