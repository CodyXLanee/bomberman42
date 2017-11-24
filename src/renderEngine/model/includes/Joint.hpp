/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Joint.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 09:43:16 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/24 09:45:42 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef JOINT_HPP
# define JOINT_HPP

# include <vector>
# include <iostream>
# include <glm/gtc/matrix_inverse.hpp>

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