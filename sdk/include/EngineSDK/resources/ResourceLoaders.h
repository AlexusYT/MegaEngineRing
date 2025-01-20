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

#ifndef RESOURCELOADERS_H
#define RESOURCELOADERS_H
#include <unordered_map>

#include "IResourceLoaders.h"

namespace mer::sdk::main {
class IResourceLoader;

class ResourceLoaders : public IResourceLoaders {
	static inline std::shared_ptr<ResourceLoaders> instance;
	std::unordered_map<std::string /*fileExtension*/, std::shared_ptr<IResourceLoader>> loaders;

public:
	ResourceLoaders() = default;
	~ResourceLoaders() override = default;

	void addLoader(const std::shared_ptr<IResourceLoader> &pLoader) override;

	std::shared_ptr<IResourceLoader> getLoader(const std::filesystem::path &pExtension) override;

	void initLoaders() override;

	static std::shared_ptr<IResourceLoaders> getInstance() {
		if (!instance) {
			instance = std::make_shared<ResourceLoaders>();
			instance->initLoaders();
		}
		return instance;
	}

	static void clearLoaders() { instance.reset(); }
};


} // namespace mer::sdk::main

#endif //RESOURCELOADERS_H
