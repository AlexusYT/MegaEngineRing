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

#include "EngineSDK/main/resources/IResourceLoadExecutor.h"
#include "EngineSDK/main/resources/ResourceLoadResult.h"
#include "EngineSDK/main/resources/textures/ITextureResource.h"
#include "EngineUtils/utils/Logger.h"
#include "MaterialResource.h"

namespace mer::sdk::main {
utils::ReportMessagePtr MaterialLoader::load(IResourceLoadExecutor* /*pLoadExecutor*/,
											 std::shared_ptr<std::istream> &pStream,
											 std::shared_ptr<IResource> &pResourceOut) {
	auto resource = MaterialResource::create();
	resource->setResourceUri(readString(pStream));
	/*pLoadExecutor->loadResourceAsync("baseColor.entex", [resource](const std::shared_ptr<ResourceLoadResult> &pResult) {
		if (pResult->isErrored()) {
			utils::Logger::error(pResult->getError());
		} else if (pResult->isReady()) {
			if (auto texture = std::dynamic_pointer_cast<ITextureResource>(pResult->getResource())) {
				if (auto msg = resource->setBaseColorMap(texture)) { utils::Logger::error(msg); }
			}
		}
	});
	pLoadExecutor->loadResourceAsync("normal.entex", [resource](const std::shared_ptr<ResourceLoadResult> &pResult) {
		if (pResult->isErrored()) {
			utils::Logger::error(pResult->getError());
		} else if (pResult->isReady()) {
			if (auto texture = std::dynamic_pointer_cast<ITextureResource>(pResult->getResource())) {
				if (auto msg = resource->setNormalMap(texture)) { utils::Logger::error(msg); }
			}
		}
	});
	pLoadExecutor->loadResourceAsync("metallic.entex", [resource](const std::shared_ptr<ResourceLoadResult> &pResult) {
		if (pResult->isErrored()) {
			utils::Logger::error(pResult->getError());
		} else if (pResult->isReady()) {
			if (auto texture = std::dynamic_pointer_cast<ITextureResource>(pResult->getResource())) {
				if (auto msg = resource->setMetallicMap(texture)) { utils::Logger::error(msg); }
			}
		}
	});
	pLoadExecutor->loadResourceAsync("roughness.entex", [resource](const std::shared_ptr<ResourceLoadResult> &pResult) {
		if (pResult->isErrored()) {
			utils::Logger::error(pResult->getError());
		} else if (pResult->isReady()) {
			if (auto texture = std::dynamic_pointer_cast<ITextureResource>(pResult->getResource())) {
				if (auto msg = resource->setRoughnessMap(texture)) { utils::Logger::error(msg); }
			}
		}
	});
	pLoadExecutor->loadResourceAsync("ao.entex", [resource](const std::shared_ptr<ResourceLoadResult> &pResult) {
		if (pResult->isErrored()) {
			utils::Logger::error(pResult->getError());
		} else if (pResult->isReady()) {
			if (auto texture = std::dynamic_pointer_cast<ITextureResource>(pResult->getResource())) {
				if (auto msg = resource->setAoMap(texture)) { utils::Logger::error(msg); }
			}
		}
	});*/

	pResourceOut = resource;
	return nullptr;
}

utils::ReportMessagePtr MaterialLoader::init(IResourceLoadExecutor* /*pLoadExecutor*/,
											 const std::shared_ptr<IResource> & /*pLoadedResource*/) {
	return nullptr;
}
} // namespace mer::sdk::main