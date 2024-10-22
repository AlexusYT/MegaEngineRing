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

#include "ObjFileResourceReader.h"

#include <regex>

#include "EngineSDK/main/resources/models/IModel3DObject.h"
#include "EngineSDK/main/resources/models/IModel3DResource.h"
#include "project/Sdk.h"

namespace mer::editor::mvp {


struct Model {

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> tCoords;
};

sdk::utils::ReportMessagePtr ObjFileResourceReader::checkType() {

	if (auto msg = open()) {
		msg->setTitle("Unable to prove that the file is Wavefront OBJ File");
		return msg;
	}
	bool hasObject{};
	bool hasVertex{};
	bool hasFace{};

	Model model;
	model.vertices.reserve(65536);
	model.tCoords.reserve(65536);
	model.normals.reserve(65536);
	std::shared_ptr<Obj> object;
	auto streamSelf = getStream();
	while (!streamSelf->eof()) {
		std::string line(1024, 0);
		try {
			streamSelf->getline(line.data(), static_cast<std::streamsize>(line.size()), '\n');
		} catch (...) { continue; }
		if (streamSelf->gcount() <= 0) continue;

		line.resize(static_cast<std::string::size_type>(streamSelf->gcount() - 1));

		if (line.starts_with("o ")) {
			hasObject = true;
			object = std::make_shared<Obj>();
			object->name = line.substr(2);
			objects.emplace(object->name, object);
		} else if (line.starts_with("v ")) {
			hasVertex = true;
			model.vertices.emplace_back(getVec3(line));
		} else if (line.starts_with("vn ")) {
			model.normals.emplace_back(getVec3(line));
		} else if (line.starts_with("vt ")) {
			model.tCoords.emplace_back(getVec2(line));
		} else if (line.starts_with("f ")) {
			hasFace = true;
			std::string s = line;
			//language=regexp
			static std::regex regex(R"((\d+)\/(\d+)\/(\d+)\ ?)");
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
	model.vertices.shrink_to_fit();
	model.tCoords.shrink_to_fit();
	model.normals.shrink_to_fit();

	if (!hasObject || !hasVertex || !hasFace) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Unable to prove that the file is Wavefront OBJ File");
		msg->setMessage("File doesn't contains objects, vertices or faces");
		msg->addInfoLine("Has objects {}", hasObject);
		msg->addInfoLine("Has vertices {}", hasVertex);
		msg->addInfoLine("Has faces {}", hasFace);
		msg->addInfoLine("File path: {}", getPath().string());
		return msg;
	}
	return nullptr;
}

std::shared_ptr<sdk::main::IModel3DResource> ObjFileResourceReader::generateResource(
	const std::vector<std::string> &pObjectsToSave) const {
	auto sdkSelf = getSdk();
	auto resource = sdkSelf->createModel3DResource();
	for (auto objectName: pObjectsToSave) { resource->addModelObject(generateObject(objectName)); }
	return resource;
}

std::shared_ptr<sdk::main::IModel3DObject> ObjFileResourceReader::generateObject(
	const std::string &pObjectToSave) const {
	auto sdkSelf = getSdk();
	const auto obj = objects.at(pObjectToSave);
	std::map<VertexInfo, uint16_t> vertexToOutIndex;
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
				uint16_t newIndex = static_cast<uint16_t>(vertices.size()) - 1;
				indices.emplace_back(newIndex);
				vertexToOutIndex[vertexInfo] = newIndex;
			} else {
				indices.emplace_back(it->second);
			}
		}
	}

	auto o = sdkSelf->createModel3DObject();
	o->setVertices(vertices);
	o->setUvs(uvs);
	o->setNormals(normals);
	o->setIndices(indices);
	o->setName(pObjectToSave);

	return o;
}

glm::vec3 ObjFileResourceReader::getVec3(const std::string &pLine) {
	//language=regexp
	static std::regex regex(R"((-?\d\.\d*) (-?\d\.\d*) (-?\d\.\d*))");
	std::smatch matches;
	if (std::regex_search(pLine, matches, regex)) {
		glm::vec3 vertex{};
		{
			std::string str = matches.str(1);
			/*auto [ptr, ec] =*/std::from_chars(str.data(), str.data() + str.size(), vertex.x);
		}
		{
			std::string str = matches.str(2);
			/*auto [ptr, ec] =*/std::from_chars(str.data(), str.data() + str.size(), vertex.y);
		}
		{
			std::string str = matches.str(3);
			/*auto [ptr, ec] =*/std::from_chars(str.data(), str.data() + str.size(), vertex.z);
		}
		return vertex;
	}
	return {};
}

glm::vec2 ObjFileResourceReader::getVec2(const std::string &pLine) {
	//language=regexp
	static std::regex regex(R"((-?\d\.\d*) (-?\d\.\d*))");
	std::smatch matches;
	if (std::regex_search(pLine, matches, regex)) {
		glm::vec2 vertex;
		{
			std::string str = matches.str(1);
			/*auto [ptr, ec] =*/std::from_chars(str.data(), str.data() + str.size(), vertex.x);
		}
		{
			std::string str = matches.str(2);
			/*auto [ptr, ec] =*/std::from_chars(str.data(), str.data() + str.size(), vertex.y);
		}
		return vertex;
	}
	return {};
}
} // namespace mer::editor::mvp