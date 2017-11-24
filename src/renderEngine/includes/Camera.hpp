/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 10:26:08 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/24 10:26:45 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_HPP
# define CAMERA_HPP

# include "bomberman.hpp"
# include <glm/gtc/matrix_transform.hpp>
# include <glm/gtx/rotate_vector.hpp>
# include <glm/gtx/matrix_interpolation.hpp>
# include <iostream>

class Camera {
	public:
		Camera();

		glm::mat4	getMatrix() const;
		// void		setOrientation(Orientation::Enum);
		void		setCamera(float, float);
		// void		startRotationAnimation(float distance, float gameSpeed, Orientation::Enum way);
		// void		startGetCloserAnimation();
		// bool		getAnimationState() const;

	private:
		// void				computeRotationAnimation(float, float, float, float);
		// void				computeGetCloserAnimation(glm::mat4);

		glm::mat4			matrix;
		float				lookingDistance;
		float				positionDistance;
		float				height;
		// Orientation::Enum	orientation;

		// bool				animationRotationStarted;
		// bool				animationGetCloserStarted;
		// bool				completeAnimationDone;
		// Orientation::Enum	animationWay;
		// float				animationStep;
};

#endif