//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
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

namespace mer::sdk::main {
class IModel3DResource;
}

namespace mer::editor::project {
class Sdk;
}

namespace mer::editor::mvp {
class ObjFileResourceReader : public FileResourceReader {
	std::shared_ptr<project::Sdk> sdk;

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

	sdk::utils::ReportMessagePtr checkType() override;

	[[nodiscard]] const std::unordered_map<std::string, std::shared_ptr<Obj>> &getObjects() const { return objects; }

	std::shared_ptr<sdk::main::IModel3DResource> generateResource(const std::vector<std::string> &pObjectsToSave) const;

	[[nodiscard]] const std::shared_ptr<project::Sdk> &getSdk() const { return sdk; }

	void setSdk(const std::shared_ptr<project::Sdk> &pSdk) { sdk = pSdk; }

private:
	static glm::vec3 getVec3(const std::string &pLine);

	static glm::vec2 getVec2(const std::string &pLine);
};
} // namespace mer::editor::mvp


#endif //OBJFILERESOURCEREADER_H
