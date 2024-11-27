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
// Created by alexus on 25.10.24.
//

#include "MaterialLoader.h"

#include "ColorComponent.h"
#include "EngineSDK/main/resources/IResourceLoadExecutor.h"
#include "EngineSDK/main/resources/ResourceLoadResult.h"
#include "EngineSDK/main/resources/textures/ITextureResource.h"
#include "EngineUtils/utils/Logger.h"
#include "MaterialResource.h"

namespace mer::sdk::main {

std::shared_ptr<IResource> MaterialLoader::createResource() { return MaterialResource::create(); }

utils::ReportMessagePtr MaterialLoader::load(IResourceLoadExecutor* pLoadExecutor,
											 std::shared_ptr<std::istream> &pStream,
											 const std::shared_ptr<IResource> &pResource) {
	auto resource = std::dynamic_pointer_cast<MaterialResource>(pResource);
	readMaterialComponent(pStream, pLoadExecutor, sigc::mem_fun(*resource, &IMaterialResource::setAlbedo));
	readMaterialComponent(pStream, pLoadExecutor, sigc::mem_fun(*resource, &IMaterialResource::setMetallic));
	readMaterialComponent(pStream, pLoadExecutor, sigc::mem_fun(*resource, &IMaterialResource::setNormal));
	readMaterialComponent(pStream, pLoadExecutor, sigc::mem_fun(*resource, &IMaterialResource::setRoughness));
	readMaterialComponent(pStream, pLoadExecutor, sigc::mem_fun(*resource, &IMaterialResource::setAo));

	return nullptr;
}

void MaterialLoader::readMaterialComponent(
	const std::shared_ptr<std::istream> &pStream, IResourceLoadExecutor* pLoadExecutor,
	const sigc::slot<void(const std::shared_ptr<IMaterialComponent> &pComponent)> &pSetter) {
	try {
		uint8_t type = 0;
		readNumber(pStream, type);
		if (type == 0) {
			const auto uri = readString(pStream);

			pLoadExecutor->loadResourceAsync(uri, [pSetter](const std::shared_ptr<ResourceLoadResult> &pResult) {
				if (pResult->isErrored()) {
					utils::Logger::error(pResult->getError());
				} else if (pResult->isReady()) {
					if (const auto texture = std::dynamic_pointer_cast<ITextureResource>(pResult->getResource())) {
						pSetter(texture);
					}
				}
			});
		} else if (type == 1) {
			auto component = ColorComponent::create();
			component->color->r = readNumber<float>(pStream);
			component->color->g = readNumber<float>(pStream);
			component->color->b = readNumber<float>(pStream);
			component->color->a = readNumber<float>(pStream);
			pSetter(component);
		}
	} catch (...) { pSetter(nullptr); }
}
} // namespace mer::sdk::main