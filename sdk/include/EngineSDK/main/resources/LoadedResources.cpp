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
// Created by alexus on 31.01.24.
//

#include "LoadedResources.h"

#include "EngineSDK/main/render/IRenderable.h"
#include "IResource.h"

namespace mer::sdk::main {
LoadedResources::LoadedResources() = default;

std::shared_ptr<ILoadedResources> LoadedResources::create() {
	return std::shared_ptr<ILoadedResources>(new LoadedResources());
}

void LoadedResources::addResource(const std::string &pResourceUri, const std::shared_ptr<IResource> &pResource) {
	std::lock_guard lock(mutex);
	resources.insert_or_assign(pResourceUri, pResource);
	if (auto renderable = std::dynamic_pointer_cast<IRenderable>(pResource)) {
		renderables.insert_or_assign(pResourceUri, std::make_pair(renderable, false));
	}
}

void LoadedResources::render() {
	for (auto &[name, renderable]: renderables) {
		if (!renderable.second) {
			renderable.first->setupRender();
			renderable.second = true;
		}
		renderable.first->render();
	}
}
} // namespace mer::sdk::main