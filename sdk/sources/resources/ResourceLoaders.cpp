//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024-2025. Timofeev (Alexus_XX) Alexander
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

#include "EngineSDK/resources/ResourceLoaders.h"

#include "EngineSDK/resources/materials/MaterialLoader.h"
#include "EngineSDK/resources/models/Model3DLoader.h"
#include "EngineSDK/resources/shaders/ShaderProgramLoader.h"
#include "EngineSDK/resources/textures/TextureLoader.h"

namespace mer::sdk::main {

void ResourceLoaders::addLoader(const std::shared_ptr<IResourceLoader> &pLoader) {
	loaders.emplace(pLoader->getFileExtension(), pLoader);
}

void ResourceLoaders::initLoaders() {
	addLoader(std::make_shared<Model3DLoader>());
	addLoader(std::make_shared<TextureLoader>());
	addLoader(std::make_shared<ShaderProgramLoader>());
	addLoader(std::make_shared<MaterialLoader>());
}

std::shared_ptr<IResourceLoader> ResourceLoaders::getLoader(const std::filesystem::path &pExtension) {
	const auto iter = loaders.find(pExtension.string().substr(1));
	if (iter == loaders.end()) { return nullptr; }
	return iter->second;
}

} // namespace mer::sdk::main