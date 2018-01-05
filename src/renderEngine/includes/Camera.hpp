/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 10:26:08 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/08 12:17:10 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_HPP
# define CAMERA_HPP

# include "bomberman.hpp"
# include "SEventManager.hpp"
# include <glm/gtc/matrix_transform.hpp>
# include <glm/gtx/rotate_vector.hpp>
# include <glm/gtx/matrix_interpolation.hpp>
# include <iostream>
# include <vector>

class Camera {
	public:
		Camera(glm::vec3, glm::vec3);

		enum 		Mode { FIXED, FREE, FOLLOW_PLAYER };
		
		glm::mat4	getMatrix() const;
		glm::vec3	getPosition() const;
		glm::vec3   getFront() const;

		void				setMode(void *);
		Camera::Mode		getMode() const;

		void		update(int const, int const, glm::vec2 const *);
		void		updateRotation(int const, int const);


	private:
		Camera();

		void				moveLeft(void *);
		void				moveRight(void *);
		void				moveUp(void *);
		void				moveDown(void *);

		void    			updateFront(void);
		void				setup();
		void				reset();
		void				wiggle(void);
		void				initWiggle(void*);
		void				firstAnimation(glm::vec2 playerPos);
		void				initFirstAnimation();

		void				animations(void *);
		
		glm::mat4			matrix;
		glm::vec3			position;
		glm::vec3			front;
		glm::vec3			const up;
		glm::vec3			_lookAt;

		Camera::Mode		mode;

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
		
		bool						is_first_animation;
		float						first_animation_init_yaw;
		float						first_animation_init_pitch;
		glm::vec3					first_animation_init_pos;

		std::chrono::time_point<std::chrono::steady_clock>		first_animation_start;
};

#endif