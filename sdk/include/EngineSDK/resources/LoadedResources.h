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
// Created by alexus on 31.01.24.
//

#ifndef LOADEDRESOURCES_H
#define LOADEDRESOURCES_H
#include <list>
#include <mutex>
#include <unordered_map>

namespace mer::sdk::main {
class IRenderable;
class IResource;

class ILoadedResources {
public:
	virtual ~ILoadedResources() = default;

	virtual void markResourceComplete(const std::string &pResourceUri) = 0;

	virtual void addResource(const std::string &pResourceUri, const std::shared_ptr<IResource> &pResource) = 0;

	virtual void removeResource(const std::string &pResourceUri) = 0;

	virtual std::shared_ptr<IResource> getResource(const std::string &pResourceUri) = 0;

	[[nodiscard]] virtual const std::unordered_map<std::string, std::shared_ptr<IResource>> &getResources() const = 0;

	virtual void render() = 0;

	virtual void uninit() = 0;
};

class LoadedResources : public ILoadedResources {
	std::unordered_map<std::string, std::shared_ptr<IResource>> resources;
	std::list<std::shared_ptr<IResource>> resourcesToInit;
	std::unordered_map<std::string, std::shared_ptr<IResource>> incompleteResources;
	std::unordered_map<std::string, std::shared_ptr<IResource>> initializedResources;
	std::list<std::shared_ptr<IRenderable>> renderables;
	std::mutex mutex;


	LoadedResources();

public:
	static std::shared_ptr<ILoadedResources> create();

	~LoadedResources() override = default;

	void markResourceComplete(const std::string &pResourceUri) override;

	void addResource(const std::string &pResourceUri, const std::shared_ptr<IResource> &pResource) override;

	void removeResource(const std::string &pResourceUri) override;

	bool hasResource(const std::string &pResourceUri);

	std::shared_ptr<IResource> getResource(const std::string &pResourceUri) override {

		std::lock_guard lock(mutex);
		const auto it = resources.find(pResourceUri);
		if (it == resources.end()) return nullptr;
		return it->second;
	}

	[[nodiscard]] const std::unordered_map<std::string, std::shared_ptr<IResource>> &getResources() const override {
		return resources;
	}

	void render() override;

	void uninit() override;

private:
};

} // namespace mer::sdk::main


#endif //LOADEDRESOURCES_H
