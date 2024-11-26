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

#ifndef EDITINGRESOURCELIST_H
#define EDITINGRESOURCELIST_H
#include "EngineSDK/main/resources/IResource.h"
#include "mvp/main/editors/sceneEditor/ResourcesContext.h"

namespace mer::sdk::main {
class IResource;
}

namespace mer::editor::mvp {

class EditingResourceList {
	std::shared_ptr<ResourcesContext> context;
	std::map<std::shared_ptr<UUID>, std::shared_ptr<sdk::main::IResource>> resources;
	sigc::signal<void(const std::shared_ptr<sdk::main::IResource> &pResource)> onResourceRemovedSignal;
	sigc::signal<void(const std::shared_ptr<sdk::main::IResource> &pResource)> onResourceAddedSignal;

public:
	void addResource(const std::shared_ptr<sdk::main::IResource> &pResource);

	sigc::connection connectOnResourceAddedSignal(
		const sigc::slot<void(const std::shared_ptr<sdk::main::IResource> &pResource)> &pSlot) {
		return onResourceAddedSignal.connect(pSlot);
	}

	sigc::connection connectOnResourceRemovedSignal(
		const sigc::slot<void(const std::shared_ptr<sdk::main::IResource> &pResource)> &pSlot) {
		return onResourceRemovedSignal.connect(pSlot);
	}

	[[nodiscard]] const std::map<std::shared_ptr<UUID>, std::shared_ptr<sdk::main::IResource>> &getResources() const {
		return resources;
	}

	[[nodiscard]] const std::shared_ptr<ResourcesContext> &getContext() const { return context; }

	void setupResourcesContext(const std::shared_ptr<ResourcesContext> &pContext) { context = pContext; }

	void loadResource(const std::filesystem::path &pUri);

	void deleteResource(const std::filesystem::path &pUri);
};

} // namespace mer::editor::mvp

#endif //EDITINGRESOURCELIST_H
