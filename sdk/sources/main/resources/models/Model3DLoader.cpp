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
#include "EngineSDK/main/resources/ResourceLoadResult.h"
#include "EngineSDK/main/resources/models/Model3DObject.h"
#include "EngineSDK/main/resources/models/Model3DResource.h"
#include "EngineUtils/utils/Logger.h"

namespace mer::sdk::main {
std::shared_ptr<IResource> Model3DLoader::createResource() { return Model3DResource::create(); }

utils::ReportMessagePtr Model3DLoader::preload(IResourceLoadExecutor* pResourcesContext,
											   const std::shared_ptr<std::istream> &pStream,
											   const std::shared_ptr<IResource> &pResource) {
	if (auto msg = ResourceLoader::preload(pResourcesContext, pStream, pResource)) { return msg; }
	auto resource = std::dynamic_pointer_cast<Model3DResource>(pResource);


	uint16_t objectCount;
	readNumber(pStream, objectCount);
	for (uint16_t i = 0; i < objectCount; ++i) {

		auto object = Model3DObject::create();
		object->setName(readString(pStream));
		resource->addModelObject(object);
	}
	return nullptr;
}

utils::ReportMessagePtr Model3DLoader::load(IResourceLoadExecutor* pLoadExecutor,
											std::shared_ptr<std::istream> &pStream,
											const std::shared_ptr<IResource> &pResource) {


	auto resource = std::dynamic_pointer_cast<Model3DResource>(pResource);
	while (!pStream->eof()) {

		try {
			if (pStream->get() == std::istream::traits_type::eof()) {
				//
				break;
			}
			pStream->unget();
		} catch (...) { break; }
		auto objectName = readString(pStream);

		auto object = resource->getModelObject(objectName);
		if (!object) {
			object = Model3DObject::create();
			object->setName(objectName);
			resource->addModelObject(object);
		}

		std::string shaderUri = readString(pStream);
		pLoadExecutor->loadResourceAsync(shaderUri, [object](const std::shared_ptr<ResourceLoadResult> &pResult) {
			if (pResult->isErrored()) {
				utils::Logger::error(pResult->getError());
				return;
			}
			if (pResult->isReady()) {
				if (auto shader = std::dynamic_pointer_cast<renderer::ShaderProgram>(pResult->getResource()))
					object->setShader(shader);
			}
		});

		std::vector<float> data;
		readArray(pStream, data);
		object->setData(data);

		std::vector<uint16_t> indices;
		readArray(pStream, indices);
		object->setIndices(indices);
	}

	return nullptr;
}


} // namespace mer::sdk::main