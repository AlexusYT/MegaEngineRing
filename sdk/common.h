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
// Created by alexus on 29.12.23.
//

#ifndef COMMON_H
#define COMMON_H

#include <glm/detail/type_vec1.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vector_relational.hpp>

inline bool operator<(glm::vec3 pLhs, glm::vec3 pRhs) {
	glm::vec3 nequ = glm::notEqual(pLhs, pRhs);
	auto lt = glm::lessThan(pLhs, pRhs);
	if (nequ[0]) return lt[0];
	if (nequ[1]) return lt[1];
	return lt[2];
}

inline bool operator>(glm::vec3 pLhs, glm::vec3 pRhs) { return pRhs < pLhs; }

inline bool operator<(glm::vec2 pLhs, glm::vec2 pRhs) {
	glm::vec2 nequ = glm::notEqual(pLhs, pRhs);
	auto lt = glm::lessThan(pLhs, pRhs);
	if (nequ[0]) return lt[0];
	return lt[1];
}

#include <filesystem>
#include <fstream>
#include <vector>


#define MAIN_SDK SDK_NAMESPACE
#endif //COMMON_H
