// MegaEngineRing is a program that can speed up game development.
// Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 26.03.24.
//

#ifndef OBJMODELREQUEST_H
#define OBJMODELREQUEST_H
#include <filesystem>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "EngineSDK/main/resources/ResourceRequest.h"

namespace mer::sdk::main {
class MultipleResource;
}

namespace mer::sdk::main {
class ObjModelLoader : public ResourceLoader {
public:
	utils::ReportMessagePtr load(const std::shared_ptr<ResourceRequest> &pRequest,
								 const std::shared_ptr<Resources> &pDependencies,
								 std::shared_ptr<IResource> &pResourceOut) override;

private:
	static glm::vec3 getVec3(const std::string &pLine);

	static glm::vec2 getVec2(const std::string &pLine);
};

class ModelRequest : public RegularRequest {
	std::string name;

public:
	explicit ModelRequest(const std::string &pName) : name(pName) {}

	using ResourceT = MultipleResource;

	virtual utils::ReportMessagePtr getStream(std::shared_ptr<std::istream> &pStreamOut) = 0;

	[[nodiscard]] std::shared_ptr<ResourceLoader> getLoader() const override {
		return std::make_shared<ObjModelLoader>();
	}

	[[nodiscard]] std::string getName() const override { return name; }
};

class FileModelRequest : public ModelRequest {
	std::filesystem::path filePath;

public:
	explicit FileModelRequest(const std::string &pName, const std::filesystem::path &pFilePath)
		: ModelRequest(pName), filePath(pFilePath) {}

	utils::ReportMessagePtr getStream(std::shared_ptr<std::istream> &pStreamOut) override;

	[[nodiscard]] const std::filesystem::path &getFilePath() const { return filePath; }

	void setFilePath(const std::filesystem::path &pFilePath) { filePath = pFilePath; }
};

} // namespace mer::sdk::main


#endif //OBJMODELREQUEST_H
