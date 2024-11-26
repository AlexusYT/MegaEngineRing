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
// Created by alexus on 14.10.24.
//

#include "EditingResourceList.h"

#include "EngineSDK/main/resources/LoadedResources.h"
#include "EngineSDK/main/resources/ResourceLoadResult.h"

namespace mer::editor::mvp {
void EditingResourceList::addResource(const std::shared_ptr<sdk::main::IResource> &pResource) {
	auto uuid = pResource->getUuid();
	auto iter = resources.find(uuid);
	if (iter != resources.end()) return;
	resources.emplace(uuid, pResource);
	onResourceAddedSignal(pResource);
}

void EditingResourceList::loadResource(const std::filesystem::path &pUri) {
	context->loadResourceAsync(pUri, [this](const std::shared_ptr<sdk::main::ResourceLoadResult> &pResult) {
		if (pResult->isErrored()) {
			sdk::utils::Logger::error(pResult->getError());
			return;
		}
		if (pResult->isReady()) addResource(pResult->getResource());
	});
}

void EditingResourceList::deleteResource(const std::filesystem::path &pUri) {
	context->getResources()->removeResource(pUri);
	for (auto resource: resources) {
		if (resource.second->getResourceUri() == pUri) {
			resources.erase(resource.first);
			onResourceRemovedSignal(resource.second);
			return;
		}
	}
}

} // namespace mer::editor::mvp