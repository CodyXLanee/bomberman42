/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Joint.class.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thibautpierron <thibautpierron@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/27 09:15:17 by tpierron          #+#    #+#             */
/*   Updated: 2017/08/06 22:05:27 by thibautpier      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOINT_CLASS_HPP
# define JOINT_CLASS_HPP

# include <vector>
# include <iostream>
# include "../../libs/glm/gtc/matrix_inverse.hpp"
// # include "../math3d.hpp"

class Joint {
	public:
		Joint(unsigned int index, std::string name, glm::mat4 bindLocalTransform);
		~Joint();

		void		addChild(Joint *child);
		glm::mat4	getAnimatedTransform() const;
		void		setAnimatedTransform(glm::mat4 animationTransform);
		glm::mat4	getFinalTransform() const;
		void		setFinalTransform(glm::mat4 finalTransform);
		glm::mat4	getInverseBindTransform() const;
		void		setInverseBindTransform(glm::mat4 inverseBindTransform);
		void		calcInverseBindTransform(glm::mat4 parentBindTransform);

		
		unsigned int		index;
		std::string			name;
		std::vector<Joint*>	children;
		
	private:
		Joint();

		glm::mat4			animatedTransform;
		glm::mat4			localBindTransform;
		glm::mat4			inverseBindTransform;
		glm::mat4			finalTransform;
};

#endif