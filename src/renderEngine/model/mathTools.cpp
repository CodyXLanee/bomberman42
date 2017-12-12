/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mathTools.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/12 10:30:21 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/12 11:39:17 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mathTools.hpp"

static unsigned int findPosition(float animationTime, const aiNodeAnim *node) {
    for (unsigned int i = 0; i < node->mNumPositionKeys - 1; i++) {
        if (animationTime < static_cast<float>(node->mPositionKeys[i + 1].mTime))
            return i;
    }
    return 0;
}

static unsigned int findRotation(float animationTime, const aiNodeAnim *node) {
    for (unsigned int i = 0; i < node->mNumRotationKeys - 1; i++) {
        if (animationTime < static_cast<float>(node->mRotationKeys[i + 1].mTime))
            return i;
    }
    return 0;
}

static unsigned int findScale(float animationTime, const aiNodeAnim *node) {
    for (unsigned int i = 0; i < node->mNumScalingKeys - 1; i++) {
        if (animationTime < static_cast<float>(node->mScalingKays[i + 1].mTime))
            return i;
    }
    return 0;
}

aiVector3D          calcInterpolatedScaling(float animationTime, const aiNodeAnim *node) {
    if(node->mNumScalingKeys == 1) {
        aiVector3D ret = node->mScalingKeys[0].mValue;
        return ret;
    }

    unsigned int scaleInd = findScale(animationTime, node);
    unsigned int nextScaleInd = scaleInd + 1;

    float deltaTime = static_cast<float>(node->mScalingKeys[nextScaleInd].mTime - node->mScalingKeys[scaleInd].mTime);
    float factor = (animationTime - static_cast<float>(node->mScalingKeys[scaleInd].mTime)) / deltaTime;
    aiVector3D start = node->mScalingKeys[scaleInd].mValue;
    aiVector3D end = node->mScalingKeys[nextScaleInd].mValue;

    aiVector3D delta = end - start;

    return start + factor * delta;
}

aiQuaternion        calcInterpolatedRotation(float animationTime, const aiNodeAnim *node) {
    if (node->mRotationKeys == 1) {
        aiQuaternion ret = node->mRotationKeys[0].mValue;
        return ret;
    }
    unsigned int rotInd = findRotation(animationTime, node);
    unsigned int nextRotInd = rotInd + 1;

    float deltaTime = static_cast<float>(node->mRotationKeys[nextRotInd].mTime - node->mRotationKeys[rotInd].mTime);
    float factor = (animationTime - static_cast<float>(node->mRotationKeys[rotInd].mTime)) / deltaTime;
    aiQuaternion start = node->mRotationKeys[rotInd].mValue;
    aiQuaternion end = node->mRotationKeys[nextRotInd].mValue;

    aiQuaternion ret;
    aiQuaternion::Interpolate(ret, start, end, factor);
    ret = ret.Normalize();

    return ret;
}

aiVector3           calcInterpolatedPosition(float animationTime, const aiNodeAnim *node) {
    if(node->mNumPositionKeys == 1) {
        aiVector3D ret = node->mPositionKeys[0].mValue;
        return ret;
    }

    unsigned int posInd = findPosition(animationTime, node);
    unsigned int nextPosInd = posInd + 1;

    float deltaTime = static_cast<float>(node->mPositionKeys[nextPosInd].mTime - node->mPositionKeys[posInd].mTime);
    float factor = (animationTime - static_cast<float>(node->mPositionKeys[posInd].mTime)) / deltaTime;
    aiVector3D start = node->mPositionKeys[posInd].mValue;
    aiVector3D end = node->mPositionKeys[nextPosInd].mValue;

    aiVector3D delta = end - start;

    return start + factor * delta;
}


glm::mat4			Mesh::asssimpToGlmMatrix(aiMatrix4x4 ai) {
		glm::mat4 mat;
		mat[0][0] = ai.a1; mat[1][0] = ai.a2; mat[2][0] = ai.a3; mat[3][0] = ai.a4;
		mat[0][1] = ai.b1; mat[1][1] = ai.b2; mat[2][1] = ai.b3; mat[3][1] = ai.b4;
		mat[0][2] = ai.c1; mat[1][2] = ai.c2; mat[2][2] = ai.c3; mat[3][2] = ai.c4;
		mat[0][3] = ai.d1; mat[1][3] = ai.d2; mat[2][3] = ai.d3; mat[3][3] = ai.d4;
		return mat;
}