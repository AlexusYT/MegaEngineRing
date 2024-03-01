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
#include "EngineUtils/utils/Logger.h"
#include "EngineUtils/utils/ReportMessage.h"
#include "LazyResource.h"
#include "ResourceRequest.h"
#include "ResourceRequests.h"
#include "Resources.h"

namespace mer::sdk::main {
LoadedResources::LoadedResources() {}

std::shared_ptr<ILoadedResources> LoadedResources::create() {
	return std::shared_ptr<ILoadedResources>(new LoadedResources);
}

std::shared_ptr<Resources> LoadedResources::executeRequests(const std::shared_ptr<ResourceRequests> &pRequests,
															const std::shared_ptr<IScene> &pScene) {
	auto resourcesOut = std::make_shared<Resources>();
	for (auto &requests = pRequests->getRequests(); const auto &[name, request]: requests) {
		processingRequests.clear();
		utils::ReportMessagePtr error;
		if (auto res = processRequest(request, error)) {
			resourcesOut->resources.emplace(name, res);
		} else
			pScene->onResourceLoadingError(request, error);
	}
	return resourcesOut;
}

std::shared_ptr<IResource> LoadedResources::executeRequest(const std::shared_ptr<ResourceRequest> &pRequest,
														   utils::ReportMessagePtr &pError) {
	processingRequests.clear();
	if (auto res = processRequest(pRequest, pError)) { return res; }
	return nullptr;
}

std::shared_ptr<IResource> LoadedResources::getResource(const std::string &pName) const {
	if (const auto iter = resources.find(pName); iter != resources.end()) return iter->second;
	return nullptr;
}

std::shared_ptr<IResource> LoadedResources::processRequest(const std::shared_ptr<ResourceRequest> &pRequest,
														   utils::ReportMessagePtr &pError) {

	if (auto res = getLoadedResourceByRequest(pRequest, pError)) { return res; }

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
		pError = msg;
		return nullptr;
	}
	processingRequests.emplace_back(pRequest->getName());

	const auto dependencies = std::make_shared<Resources>();
	for (const auto &resourceRequest: pRequest->getRequired()) {
		if (auto result = processRequest(resourceRequest, pError)) {
			dependencies->resources.emplace(resourceRequest->getName(), result);
			resources.emplace(resourceRequest->getName(), result);
		}
	}

	if (std::dynamic_pointer_cast<RegularRequest>(pRequest)) {
		if (auto resource = executeRequest(dependencies, pRequest, pError)) {

			resources.emplace(pRequest->getName(), resource);
			return resource;
		}
		return nullptr;
	}

	return std::shared_ptr<LazyResource>(new LazyResource(pRequest, dependencies));
}

std::shared_ptr<IResource> LoadedResources::getLoadedResourceByRequest(const std::shared_ptr<ResourceRequest> &pRequest,
																	   utils::ReportMessagePtr &pError) const {

	auto res = getResource(pRequest->getName());
	if (!res) return nullptr;
	if (std::dynamic_pointer_cast<LazyResource>(res)) {
		if (std::dynamic_pointer_cast<RegularRequest>(pRequest)) {
			const auto msg = utils::ReportMessage::create();
			msg->setTitle("Resource loading policy mismatch");
			msg->setMessage("Resource previously loaded with LAZY policy, but trying to load with REGULAR");
			msg->addInfoLine("DO NOT CHANGE POLICY FOR SINGLE RESOURCE");
			msg->addInfoLine("Resource added as LAZY");
			msg->addInfoLine("Resource name: {}", pRequest->getName());
			pError = msg;
		}
		return res;
	}
	if (std::dynamic_pointer_cast<LazyRequest>(pRequest)) {
		const auto msg = utils::ReportMessage::create();
		msg->setTitle("Resource loading policy mismatch");
		msg->setMessage("Resource previously loaded with REGULAR policy, but trying to load with LAZY");
		msg->addInfoLine("DO NOT CHANGE POLICY FOR SINGLE RESOURCE");
		msg->addInfoLine("Resource added as IMMEDIATELY");
		msg->addInfoLine("Resource name: {}", pRequest->getName());
		pError = msg;
	}
	return res;
}

std::shared_ptr<IResource> LoadedResources::executeRequest(const std::shared_ptr<Resources> &pDependencies,
														   const std::shared_ptr<ResourceRequest> &pRequest,
														   utils::ReportMessagePtr &pError) {

	auto error = utils::ReportMessage::create();
	if (auto resource = pRequest->getLoader()->load(pRequest, error, pDependencies)) { return resource; }
	pError = error;
	return nullptr;
}
} // namespace mer::sdk::main