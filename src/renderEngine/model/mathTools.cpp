/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mathTools.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/12 10:30:21 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/12 11:11:34 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mathTools.hpp"


aiVector3D          calcInterpolatedScaling(float animationTime, const aiNodeAnim *node) {

}

aiQuaternion        calcInterpolatedRotation(float animationTime, const aiNodeAnim *node) {
    if (node->mRotationKeys == 1) {
        aiQuaternion ret = node->mRotationKeys[0].mValue;
        return ret;
    }
    unsigned int rotInd = findRotation(animationTime, node);
    unsigned int nextRotInd = rotInd + 1;
    float deltaTime = node->mRotationKeys[nextRotInd].mTime - node->mRotationKeys[rotInd].mTime;
    float factor = (animationTime - static_cast<float>(node->mRotationKeys[rotInd].mTime)) / deltaTime;
    aiQuaternion start = node->mRotationKeys[rotInd].mValue;
    aiQuaternion end = node->mRotationKeys[nextRotInd].mValue;

    aiQuaternion ret;
    aiQuaternion::Interpolate(ret, start, end, factor);
    ret = ret.Normalize();

    return ret;
}

aiVector3           calcInterpolatedPosition(float animationTime, const aiNodeAnim *node) {

}


glm::mat4			Mesh::asssimpToGlmMatrix(aiMatrix4x4 ai) {
		glm::mat4 mat;
		mat[0][0] = ai.a1; mat[1][0] = ai.a2; mat[2][0] = ai.a3; mat[3][0] = ai.a4;
		mat[0][1] = ai.b1; mat[1][1] = ai.b2; mat[2][1] = ai.b3; mat[3][1] = ai.b4;
		mat[0][2] = ai.c1; mat[1][2] = ai.c2; mat[2][2] = ai.c3; mat[3][2] = ai.c4;
		mat[0][3] = ai.d1; mat[1][3] = ai.d2; mat[2][3] = ai.d3; mat[3][3] = ai.d4;
		return mat;
}