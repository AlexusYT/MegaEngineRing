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

#include "EngineSDK/main/resources/models/Model3DLoader.h"

#include "EngineSDK/main/resources/IResourceLoadExecutor.h"
#include "EngineSDK/main/resources/models/Model3DObject.h"
#include "EngineSDK/main/resources/models/Model3DResource.h"

namespace mer::sdk::main {

utils::ReportMessagePtr Model3DLoader::load(IResourceLoadExecutor* pLoadExecutor,
											std::shared_ptr<std::istream> &pStream,
											std::shared_ptr<IResource> &pResourceOut) {


	auto resource = Model3DResource::create();
	resource->setResourceUri(readString(pStream));
	while (!pStream->eof()) {

		try {
			if (pStream->get() == std::istream::traits_type::eof()) {
				//
				break;
			}
			pStream->unget();
		} catch (...) { break; }
		auto object = Model3DObject::create();
		object->setName(readString(pStream));
		std::string shaderUri = readString(pStream);
		pLoadExecutor->loadResourceAsync(shaderUri, [object](const std::shared_ptr<IResource> &pResource,
															 const utils::ReportMessagePtr & /*pError*/) {
			if (auto shader = std::dynamic_pointer_cast<renderer::ShaderProgram>(pResource)) object->setShader(shader);
		});

		std::vector<glm::vec3> vertices;
		readArray(pStream, vertices);
		object->setVertices(vertices);

		std::vector<glm::vec2> uvs;
		readArray(pStream, uvs);
		object->setUvs(uvs);

		std::vector<glm::vec3> normals;
		readArray(pStream, normals);
		object->setNormals(normals);

		std::vector<uint16_t> indices;
		readArray(pStream, indices);
		object->setIndices(indices);
		resource->addModelObject(object);
	}
	pResourceOut = resource;

	return nullptr;
}


} // namespace mer::sdk::main