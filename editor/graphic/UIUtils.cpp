//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024-2025. Timofeev (Alexus_XX) Alexander
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program; if not, write to the Free Software Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by Alexus on 02.09.2022.
//

#include "UIUtils.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/vec3.hpp>

void UIUtils::getCubeVertices(std::vector<glm::vec3> &vertices, const glm::vec3 &minSize, const glm::vec3 &maxSize) {
	vertices.emplace_back(maxSize.x, maxSize.y, minSize.z); //верхний правый дальний угол
	vertices.emplace_back(maxSize.x, minSize.y, minSize.z); //нижний правый дальний угол
	vertices.emplace_back(maxSize.x, maxSize.y, maxSize.z); //верхний правый ближний угол
	vertices.emplace_back(maxSize.x, minSize.y, maxSize.z); //нижний правый ближний угол

	vertices.emplace_back(minSize.x, maxSize.y, minSize.z); //верхний левый дальний угол
	vertices.emplace_back(minSize.x, minSize.y, minSize.z); //нижний левый дальний угол
	vertices.emplace_back(minSize.x, maxSize.y, maxSize.z); //верхний левый ближний угол
	vertices.emplace_back(minSize.x, minSize.y, maxSize.z); //нижний левый ближний угол
}

void UIUtils::getCubeVertices(std::vector<glm::vec3> &vertices, const glm::vec3 &size) {

	float halfX = size.x / 2;
	float halfY = size.y / 2;
	float halfZ = size.z / 2;
	vertices.emplace_back(glm::vec3(halfX, halfY, -halfZ));	 //верхний правый дальний угол
	vertices.emplace_back(glm::vec3(halfX, -halfY, -halfZ)); //нижний правый дальний угол
	vertices.emplace_back(glm::vec3(halfX, halfY, halfZ));	 //верхний правый ближний угол
	vertices.emplace_back(glm::vec3(halfX, -halfY, halfZ));	 //нижний правый ближний угол

	vertices.emplace_back(glm::vec3(-halfX, halfY, -halfZ));  //верхний левый дальний угол
	vertices.emplace_back(glm::vec3(-halfX, -halfY, -halfZ)); //нижний левый дальний угол
	vertices.emplace_back(glm::vec3(-halfX, halfY, halfZ));	  //верхний левый ближний угол
	vertices.emplace_back(glm::vec3(-halfX, -halfY, halfZ));  //нижний левый ближний угол
}

void UIUtils::getCubeVerticesOffset(std::vector<glm::vec3> &vertices, const glm::vec3 &size, const glm::vec3 &pos) {

	const float halfX = size.x / 2;
	const float halfY = size.y / 2;
	const float halfZ = size.z / 2;
	vertices.emplace_back(pos + glm::vec3(halfX, halfY, -halfZ));  //верхний правый дальний угол
	vertices.emplace_back(pos + glm::vec3(halfX, -halfY, -halfZ)); //нижний правый дальний угол
	vertices.emplace_back(pos + glm::vec3(halfX, halfY, halfZ));   //верхний правый ближний угол
	vertices.emplace_back(pos + glm::vec3(halfX, -halfY, halfZ));  //нижний правый ближний угол

	vertices.emplace_back(pos + glm::vec3(-halfX, halfY, -halfZ));	//верхний левый дальний угол
	vertices.emplace_back(pos + glm::vec3(-halfX, -halfY, -halfZ)); //нижний левый дальний угол
	vertices.emplace_back(pos + glm::vec3(-halfX, halfY, halfZ));	//верхний левый ближний угол
	vertices.emplace_back(pos + glm::vec3(-halfX, -halfY, halfZ));	//нижний левый ближний угол
}

void UIUtils::getCubeIndices(std::vector<uint16_t> &indices, uint16_t offset) {

	indices.push_back(offset + 4);
	indices.push_back(offset + 2);
	indices.push_back(offset);
	indices.push_back(offset + 2);
	indices.push_back(offset + 7);
	indices.push_back(offset + 3);
	indices.push_back(offset + 6);
	indices.push_back(offset + 5);
	indices.push_back(offset + 7);
	indices.push_back(offset + 1);
	indices.push_back(offset + 7);
	indices.push_back(offset + 5);
	indices.push_back(offset + 0);
	indices.push_back(offset + 3);
	indices.push_back(offset + 1);
	indices.push_back(offset + 4);
	indices.push_back(offset + 1);
	indices.push_back(offset + 5);
	indices.push_back(offset + 4);
	indices.push_back(offset + 6);
	indices.push_back(offset + 2);
	indices.push_back(offset + 2);
	indices.push_back(offset + 6);
	indices.push_back(offset + 7);
	indices.push_back(offset + 6);
	indices.push_back(offset + 4);
	indices.push_back(offset + 5);
	indices.push_back(offset + 1);
	indices.push_back(offset + 3);
	indices.push_back(offset + 7);
	indices.push_back(offset + 0);
	indices.push_back(offset + 2);
	indices.push_back(offset + 3);
	indices.push_back(offset + 4);
	indices.push_back(offset + 0);
	indices.push_back(offset + 1);
}
#if 0
glm::mat4 UIUtils::buildModelViewMatrixP(const glm::mat4 &mat, const glm::vec3 &position) {
	return glm::translate(mat, position);
}

glm::mat4 UIUtils::buildModelViewMatrixS(const glm::mat4 &mat, const glm::vec3 &scale) {
	return glm::scale(mat, scale);
}

glm::mat4 UIUtils::buildModelViewMatrixR(const glm::mat4 &mat, const glm::vec3 &rotation) {
	return UIUtils::eulerAngleYXZ(mat, rotation);
}

glm::mat4 UIUtils::buildModelViewMatrixPR(const glm::mat4 &mat, const glm::vec3 &position, const glm::vec3 &rotation) {
	return UIUtils::eulerAngleYXZ(glm::translate(mat, position), rotation);
}

glm::mat4 UIUtils::buildModelViewMatrixPS(const glm::mat4 &mat, const glm::vec3 &position, const glm::vec3 &scale) {
	return glm::scale(glm::translate(mat, position), scale);
}

glm::mat4 UIUtils::buildModelViewMatrixRS(const glm::mat4 &mat, const glm::vec3 &rotation, const glm::vec3 &scale) {
	return UIUtils::eulerAngleYXZ(glm::scale(mat, scale), rotation);
}

glm::mat4 UIUtils::buildModelViewMatrixPRS(const glm::mat4 &mat, const glm::vec3 &position, const glm::vec3 &rotation,
										   const glm::vec3 &scale) {
	return glm::translate(glm::mat4(1), position) * glm::eulerAngleYXZ(rotation.x, rotation.y, rotation.z) *
		   glm::scale(mat, scale);
}
#endif
