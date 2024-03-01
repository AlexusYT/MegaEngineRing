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

#ifndef LOADEDRESOURCES_H
#define LOADEDRESOURCES_H
#include <unordered_map>

#include "EngineUtils/utils/ReportMessageFwd.h"
#include "Resources.h"

namespace mer::sdk::main {
class IScene;
enum class ResourceLoadingPolicy;
class ResourceRequest;
class IResource;
class ResourceRequests;

class ILoadedResources {
public:
	virtual ~ILoadedResources() = default;
	virtual std::shared_ptr<Resources> executeRequests(const std::shared_ptr<ResourceRequests> &pRequests,
													   const std::shared_ptr<IScene> &pScene) = 0;
	virtual std::shared_ptr<IResource> executeRequest(const std::shared_ptr<ResourceRequest> &pRequest,
													  utils::ReportMessagePtr &pError) = 0;
};

class LoadedResources : public ILoadedResources {
	std::unordered_map<std::string, std::shared_ptr<IResource>> resources;
	std::vector<std::string> processingRequests;


	LoadedResources();

public:
	static std::shared_ptr<ILoadedResources> create();

	~LoadedResources() override = default;
	std::shared_ptr<Resources> executeRequests(const std::shared_ptr<ResourceRequests> &pRequests,
											   const std::shared_ptr<IScene> &pScene) override;

	std::shared_ptr<IResource> executeRequest(const std::shared_ptr<ResourceRequest> &pRequest,
											  utils::ReportMessagePtr &pError) override;

private:
	std::shared_ptr<IResource> getResource(const std::string &pName) const;

	std::shared_ptr<IResource> processRequest(const std::shared_ptr<ResourceRequest> &pRequest,
											  utils::ReportMessagePtr &pError);

	std::shared_ptr<IResource> getLoadedResourceByRequest(const std::shared_ptr<ResourceRequest> &pRequest,
														  utils::ReportMessagePtr &pError) const;

	static std::shared_ptr<IResource> executeRequest(const std::shared_ptr<Resources> &pDependencies,
													 const std::shared_ptr<ResourceRequest> &pRequest,
													 utils::ReportMessagePtr &pError);
};

} // namespace mer::sdk::main


#endif //LOADEDRESOURCES_H
