/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mathTools.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/12 10:30:21 by tpierron          #+#    #+#             */
/*   Updated: 2017/12/13 11:30:30 by tpierron         ###   ########.fr       */
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
        if (animationTime < static_cast<float>(node->mScalingKeys[i + 1].mTime))
            return i;
    }
    return 0;
}

static glm::mat4 quatToMat(const aiQuaternion &quat) {
    float yy2 = 2.0f * quat.y * quat.y;
    float xy2 = 2.0f * quat.x * quat.y;
    float xz2 = 2.0f * quat.x * quat.z;
    float yz2 = 2.0f * quat.y * quat.z;
    float zz2 = 2.0f * quat.z * quat.z;
    float wz2 = 2.0f * quat.w * quat.z;
    float wy2 = 2.0f * quat.w * quat.y;
    float wx2 = 2.0f * quat.w * quat.x;
    float xx2 = 2.0f * quat.x * quat.x;

    glm::mat4 m;
    m[0][0] = - yy2 - zz2 + 1.0f;
    m[0][1] = xy2 + wz2;
    m[0][2] = xz2 - wy2;
    m[0][3] = 0;
    m[1][0] = xy2 - wz2;
    m[1][1] = - xx2 - zz2 + 1.0f;
    m[1][2] = yz2 + wx2;
    m[1][3] = 0;
    m[2][0] = xz2 + wy2;
    m[2][1] = yz2 - wx2;
    m[2][2] = - xx2 - yy2 + 1.0f;
    m[2][3] = 0.0f;
    m[3][0] = m[3][1] = m[3][2] = 0;
    m[3][3] = 1.0f;

    return m;
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

glm::mat4        calcInterpolatedRotation(float animationTime, const aiNodeAnim *node) {
    if (node->mNumRotationKeys == 1) {
        aiQuaternion ret = node->mRotationKeys[0].mValue;
        return quatToMat(ret);
    }
    unsigned int rotInd = findRotation(animationTime, node);
    unsigned int nextRotInd = rotInd + 1;

    float deltaTime = static_cast<float>(node->mRotationKeys[nextRotInd].mTime - node->mRotationKeys[rotInd].mTime);
    float factor = (animationTime - static_cast<float>(node->mRotationKeys[rotInd].mTime)) / deltaTime;
    aiQuaternion start = node->mRotationKeys[rotInd].mValue;
    aiQuaternion end = node->mRotationKeys[nextRotInd].mValue;

    aiQuaternion quat;
    aiQuaternion::Interpolate(quat, start, end, factor);
    quat = quat.Normalize();

    return quatToMat(quat);
}

aiVector3D          calcInterpolatedPosition(float animationTime, const aiNodeAnim *node) {
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


glm::mat4			assimpToGlmMatrix(aiMatrix4x4 ai) {
		glm::mat4 mat;
		mat[0][0] = ai.a1; mat[1][0] = ai.a2; mat[2][0] = ai.a3; mat[3][0] = ai.a4;
		mat[0][1] = ai.b1; mat[1][1] = ai.b2; mat[2][1] = ai.b3; mat[3][1] = ai.b4;
		mat[0][2] = ai.c1; mat[1][2] = ai.c2; mat[2][2] = ai.c3; mat[3][2] = ai.c4;
		mat[0][3] = ai.d1; mat[1][3] = ai.d2; mat[2][3] = ai.d3; mat[3][3] = ai.d4;
		return mat;
}

void				printMat(glm::mat4 mat) {
		std::cout << mat[0][0] << "\t" << mat[0][1] << "\t" << mat[0][2] << "\t" << mat[0][3] << std::endl;
		std::cout << mat[1][0] << "\t" << mat[1][1] << "\t" << mat[1][2] << "\t" << mat[1][3] << std::endl;
		std::cout << mat[2][0] << "\t" << mat[2][1] << "\t" << mat[2][2] << "\t" << mat[2][3] << std::endl;
		std::cout << mat[3][0] << "\t" << mat[3][1] << "\t" << mat[3][2] << "\t" << mat[3][3] << std::endl << std::endl;
}

void				debugMat(glm::mat4 mat, std::string str) {
        float m[4][4];
        std::string s = str;
		m[0][0] = mat[0][0]; 
        m[0][1] = mat[0][1]; 
        m[0][2] = mat[0][2]; 
        m[0][3] = mat[0][3];
		m[1][0] = mat[1][0]; 
        m[1][1] = mat[1][1]; 
        m[1][2] = mat[1][2]; 
        m[1][3] = mat[1][3];
		m[2][0] = mat[2][0]; 
        m[2][1] = mat[2][1]; 
        m[2][2] = mat[2][2]; 
        m[2][3] = mat[2][3];
		m[3][0] = mat[3][0]; 
        m[3][1] = mat[3][1]; 
        m[3][2] = mat[3][2]; 
        m[3][3] = mat[3][3];
        
}