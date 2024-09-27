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

#include "EngineSDK/main/resources/models/ObjModelRequest.h"

#include <cstring>
#include <fstream>
#include <regex>

#include "EngineSDK/main/IApplication.h"
#include "EngineSDK/main/IApplicationSettings.h"
#include "EngineSDK/main/resources/MultipleResource.h"
#include "EngineSDK/main/resources/models/ModelResource.h"
#include "EngineUtils/utils/Logger.h"

namespace mer::sdk::main {

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

struct Model {

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> tCoords;
};

utils::ReportMessagePtr ObjModelLoader::load(const std::shared_ptr<ResourceRequest> &pRequest,
											 const std::shared_ptr<Resources> & /*pDependencies*/,
											 std::shared_ptr<IResource> &pResourceOut) {
	auto request = std::dynamic_pointer_cast<ModelRequest>(pRequest);
	if (!request) {

		auto msg = utils::ReportMessage::create();
		msg->setStacktrace();
		msg->setTitle("Failed to load an obj model");
		msg->setMessage("Invalid request");
		msg->addInfoLine("Request name: {}", pRequest->getName());
		return msg;
	}
	if (auto fileRequest = std::dynamic_pointer_cast<FileModelRequest>(pRequest)) {
		auto rootPath = getApplication()->getApplicationSettings()->getRunDirectory().getValue();
		fileRequest->setFilePath(rootPath / fileRequest->getFilePath());
	}

	std::shared_ptr<std::istream> stream;
	if (auto error = request->getStream(stream)) { return error; }

	std::vector<std::shared_ptr<Obj>> objects;
	{
		Model model;
		std::shared_ptr<Obj> object;
		while (!stream->eof()) {
			std::string line(1024, 0);
			try {
				stream->getline(line.data(), static_cast<std::streamsize>(line.size()), '\n');
			} catch (...) { continue; }
			if (stream->gcount() <= 0) continue;
			line.resize(static_cast<std::string::size_type>(stream->gcount() - 1));

			if (line.starts_with('#')) continue;
			if (line.starts_with("o ")) {
				object = std::make_shared<Obj>();
				objects.emplace_back(object);
				object->name = line.substr(2);
			} else if (line.starts_with("v ")) {
				model.vertices.emplace_back(getVec3(line));
			} else if (line.starts_with("vn ")) {
				model.normals.emplace_back(getVec3(line));
			} else if (line.starts_with("vt ")) {
				model.tCoords.emplace_back(getVec2(line));
			} else if (line.starts_with("f ")) {
				std::string s = line;
				//language=regexp
				std::regex regex(R"((\d+)\/(\d+)\/(\d+)\ ?)");
				std::smatch matches;
				Face face;
				while (!s.empty()) {
					if (std::regex_search(s, matches, regex)) {
						VertexInfo vertexInfo;
						vertexInfo.vertexCoord = model.vertices.at(std::stoul(matches.str(1)) - 1);
						vertexInfo.texCoord = model.tCoords.at(std::stoul(matches.str(2)) - 1);
						vertexInfo.normalCoord = model.normals.at(std::stoul(matches.str(3)) - 1);
						face.emplace_back(vertexInfo);
						s = matches.suffix();
					}
				}
				object->faces.emplace_back(face);

			} else {
				//utils::Logger::out(line);
			}
		}
	}


	std::map<VertexInfo, uint16_t> vertexToOutIndex;
	auto finalObjects = std::make_shared<MultipleResource>();
	for (auto obj: objects) {
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
		std::vector<uint16_t> indices;
		for (auto face: obj->faces) {
			for (auto vertexInfo: face) {
				auto it = vertexToOutIndex.find(vertexInfo);
				if (it == vertexToOutIndex.end()) {
					vertices.emplace_back(vertexInfo.vertexCoord);
					uvs.emplace_back(vertexInfo.texCoord);
					normals.emplace_back(vertexInfo.normalCoord);
					uint16_t newindex = static_cast<uint16_t>(vertices.size()) - 1;
					indices.emplace_back(newindex);
					vertexToOutIndex[vertexInfo] = newindex;
				} else {
					indices.emplace_back(it->second);
				}
			}
		}
		auto o = std::make_shared<ModelResource>(vertices, uvs, normals, indices, obj->name);
		finalObjects->emplace(obj->name, o);
	}
	pResourceOut = finalObjects;

	return nullptr;
}

glm::vec3 ObjModelLoader::getVec3(const std::string &pLine) {
	//language=regexp
	std::regex regex(R"((-?\d\.\d*) (-?\d\.\d*) (-?\d\.\d*))");
	std::smatch matches;
	if (std::regex_search(pLine, matches, regex)) {
		glm::vec3 vertex;
		vertex.x = std::stof(matches.str(1));
		vertex.y = std::stof(matches.str(2));
		vertex.z = std::stof(matches.str(3));
		return vertex;
	}
	return {};
}

glm::vec2 ObjModelLoader::getVec2(const std::string &pLine) {
	//language=regexp
	std::regex regex(R"((-?\d\.\d*) (-?\d\.\d*))");
	std::smatch matches;
	if (std::regex_search(pLine, matches, regex)) {
		glm::vec2 vertex;
		vertex.x = std::stof(matches.str(1));
		vertex.y = std::stof(matches.str(2));
		return vertex;
	}
	return {};
}

std::shared_ptr<FileModelRequest> FileModelRequest::create(const std::string &pName,
const std::filesystem::path &pFilePath) {
	return std::shared_ptr<FileModelRequest>(new FileModelRequest(pName, pFilePath));
}

utils::ReportMessagePtr FileModelRequest::getStream(std::shared_ptr<std::istream> &pStreamOut) {

	auto file = std::make_shared<std::ifstream>();
	try {
		file->exceptions(std::_S_badbit | std::_S_failbit);
		file->open(filePath);
	} catch (...) {


		auto msg = utils::ReportMessage::create();
		msg->setStacktrace();
		msg->setTitle("Failed to open model file");
		msg->setMessage("Exception thrown");
		msg->addInfoLine("File path: {}", absolute(filePath).string());
		return msg;
	}
	pStreamOut = file;
	return nullptr;
}
} // namespace mer::sdk::main