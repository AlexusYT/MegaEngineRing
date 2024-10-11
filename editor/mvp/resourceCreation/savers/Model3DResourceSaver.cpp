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
// Created by alexus on 02.10.24.
//

#include "Model3DResourceSaver.h"

#include "EngineSDK/main/resources/models/IModel3DObject.h"
#include "EngineSDK/main/resources/models/IModel3DResource.h"

namespace mer::editor::mvp {
sdk::utils::ReportMessagePtr Model3DResourceSaver::saveToFile(
	const std::filesystem::path &pPath, const std::shared_ptr<sdk::main::IModel3DResource> &pModel3DResource) {

	auto resourceUri = pModel3DResource->asResource()->getResourceUri();
	try {
		std::ofstream file(pPath, std::ios::out | std::ios::binary);
		file.exceptions(std::_S_badbit | std::_S_failbit);

		writeString(file, resourceUri);
		for (auto [objName, object]: pModel3DResource->getModelObjects()) {
			writeString(file, objName);
			auto shader = object->getShader();
			std::string shaderName;
			if (shader) {
				shaderName = shader->getResourceUri();
			} else {
				shaderName = "_builtin_/shaders/DefaultInstancedProgram.enshader";
			}
			writeString(file, shaderName);

			writeArray(file, object->getVertices());
			writeArray(file, object->getUvs());
			writeArray(file, object->getNormals());
			writeArray(file, object->getIndices());
		}
	} catch (...) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to save model resource to file");
		msg->setMessage("Exception occurred");
		msg->addInfoLine("Path: {}", pPath.string());
		msg->addInfoLine("Resource URI: {}", resourceUri.string());
		return msg;
	}
	return nullptr;
}

void Model3DResourceSaver::writeString(std::ofstream &pStream, const std::string &pStr) {

	auto nameSize = pStr.size();
	pStream.write(reinterpret_cast<const std::ostream::char_type*>(&nameSize), sizeof(nameSize));
	pStream.write(pStr.data(), static_cast<long int>(nameSize));
}

} // namespace mer::editor::mvp