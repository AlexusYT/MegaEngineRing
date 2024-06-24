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

#include "EngineSDK/main/scene/IScene.h"
#include "EngineUtils/utils/ReportMessage.h"
#include "IResources.h"
#include "ResourceRequest.h"
#include "ResourceRequests.h"
#include "Resources.h"

namespace mer::sdk::main {
LoadedResources::LoadedResources(IResources* pResources) : iResources(pResources) {}

std::shared_ptr<ILoadedResources> LoadedResources::create(IResources* pResources) {
	return std::shared_ptr<ILoadedResources>(new LoadedResources(pResources));
}

std::shared_ptr<Resources> LoadedResources::executeRequests(const std::shared_ptr<ResourceRequests> &pRequests,
															const std::shared_ptr<IScene> &pScene) {
	auto resourcesOut = std::make_shared<Resources>();
	for (auto &requests = pRequests->getRequests(); const auto &[name, request]: requests) {
		processingRequests.clear();
		std::shared_ptr<IResource> res;
		if (auto error = processRequest(request, res)) {
			pScene->onResourceLoadingError(request, error);
		} else
			resourcesOut->resources.emplace(name, res);
	}
	return resourcesOut;
}

utils::ReportMessagePtr LoadedResources::executeRequest(const std::shared_ptr<ResourceRequest> &pRequest,
														std::shared_ptr<IResource> &pResourceOut) {
	processingRequests.clear();
	if (auto error = processRequest(pRequest, pResourceOut)) { return error; }
	return nullptr;
}

utils::ReportMessagePtr LoadedResources::processRequest(const std::shared_ptr<ResourceRequest> &pRequest,
														std::shared_ptr<IResource> &pResourceOut) {

	if (const auto iter = resources.find(pRequest->getName()); iter != resources.end()) {
		pResourceOut = iter->second;
		return nullptr;
	}

	if (std::ranges::find(processingRequests, pRequest->getName()) != processingRequests.end()) {
		auto msg = utils::ReportMessage::create();
		msg->setTitle("Failed to process request");
		msg->setMessage("Cyclic dependencies have been found");
		for (auto reqIter = processingRequests.begin(); reqIter != processingRequests.end(); ++reqIter) {
			auto iterNext = reqIter;
			std::advance(iterNext, 1);
			if (iterNext == processingRequests.end())
				msg->addInfoLine("Request {} required {}", *reqIter, pRequest->getName());
			else
				msg->addInfoLine("Request {} required {}", *reqIter, *iterNext);
		}
		return msg;
	}
	processingRequests.emplace_back(pRequest->getName());

	const auto dependencies = std::make_shared<Resources>();
	for (const auto &resourceRequest: pRequest->getRequired()) {
		std::shared_ptr<IResource> res;
		if (auto error = processRequest(resourceRequest, res)) return error;
		dependencies->resources.emplace(resourceRequest->getName(), res);
		resources.emplace(resourceRequest->getName(), res);
	}

	if (auto error = executeRequest(dependencies, pRequest, pResourceOut)) { return error; }
	resources.emplace(pRequest->getName(), pResourceOut);
	return nullptr;
}

utils::ReportMessagePtr LoadedResources::executeRequest(const std::shared_ptr<Resources> &pDependencies,
														const std::shared_ptr<ResourceRequest> &pRequest,
														std::shared_ptr<IResource> &pResourceOut) const {

	const auto loader = pRequest->getLoader();
	const auto iLoader = std::dynamic_pointer_cast<IResourceLoader>(loader);
	iLoader->setApplication(iResources->getApplication());
	if (auto error = loader->load(pRequest, pDependencies, pResourceOut)) { return error; }
	return nullptr;
}
} // namespace mer::sdk::main