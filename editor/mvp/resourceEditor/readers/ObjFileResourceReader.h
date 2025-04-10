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
// Created by alexus on 01.10.24.
//

#ifndef OBJFILERESOURCEREADER_H
#define OBJFILERESOURCEREADER_H
#include "FileResourceReader.h"

namespace mer::sdk {
class IModel3DObject;
class IModel3DResource;
} // namespace mer::sdk


namespace mer::editor::mvp {
class ObjFileResourceReader : public FileResourceReader {

	struct VertexInfo {
		glm::vec3 vertexCoord;
		glm::vec3 normalCoord;
		glm::vec2 texCoord;

		bool operator<(const VertexInfo pThat) const { return memcmp(this, &pThat, sizeof(VertexInfo)) > 0; }
	};

	using Face = std::vector<VertexInfo>;

	struct Obj {
		std::string name;
		std::vector<Face> faces;
	};

	std::unordered_map<std::string, std::shared_ptr<Obj>> objects;


public:
	explicit ObjFileResourceReader(const std::filesystem::path &pPath) : FileResourceReader(pPath) {}

	sdk::ReportMessagePtr checkType() override;

	[[nodiscard]] const std::unordered_map<std::string, std::shared_ptr<Obj>> &getObjects() const { return objects; }

	std::vector<std::string> getObjectsName() const {

		auto objectsKey = getObjects() | std::views::keys;
		auto vec = std::vector(objectsKey.begin(), objectsKey.end());
		std::ranges::sort(vec);
		return vec;
	}

	std::shared_ptr<sdk::IModel3DResource> generateResource(const std::vector<std::string> &pObjectsToSave) const;

	std::shared_ptr<sdk::IModel3DObject> generateObject(const std::string &pObjectToSave) const;


private:
	static glm::vec3 getVec3(const std::string &pLine);

	static glm::vec2 getVec2(const std::string &pLine);
};
} // namespace mer::editor::mvp


#endif //OBJFILERESOURCEREADER_H
