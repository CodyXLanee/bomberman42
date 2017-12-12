/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mathTools.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/12 10:30:53 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/12 11:11:28 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATHTOOLS_HPP
# define MATHTOOLS_HPP

# include <assimp/scene.h>
# include <glm/gtc/matrix_transform.hpp>

aiVector3D          calcInterpolatedScaling(float animationTime, const aiNodeAnim *node);
aiQuaternion        calcInterpolatedRotation(float animationTime, const aiNodeAnim *node);
aiVector3D          calcInterpolatedPosition(float animationTime, const aiNodeAnim *node);
glm::mat4			assimpToGlmMatrix(aiMatrix4x4 ai);

#endif